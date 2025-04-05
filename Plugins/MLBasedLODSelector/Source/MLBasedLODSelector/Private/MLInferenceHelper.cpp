#include "MLInferenceHelper.h"
#include "Kismet/GameplayStatics.h"

UMLInferenceHelper& UMLInferenceHelper::Get()
{
    static UMLInferenceHelper* Singleton = NewObject<UMLInferenceHelper>();
    return *Singleton;
}

/*
 액터를 바탕으로 InputData 벡터에 Distace, ScreenBound, NumTriangle, NumMatrial, MemoryUsage를 순서대로 계산하여 넣어줌
*/
std::vector<float> UMLInferenceHelper::PreProcessActor(AActor* Actor, UWorld* World, const FMinimalViewInfo& ViewInfo)
{
    // camera distance
    FVector ActorLocation = Actor->GetActorLocation();
    FVector CameraLocation = ViewInfo.Location;
    float Distance = FVector::Dist(ActorLocation, CameraLocation);

    // bound
    float ScreenBound = GetActorScreenSize(Actor, World);

    // polygon
    float NumTriangle = GetNumTriangle(Actor);

    // material
    float NumMatrial = GetNumMaterial(Actor);

    // memory usage
    float MemoryUsage = GetMemoryUsage(Actor);

    std::vector<float> InputData;
    InputData.push_back(Distance);
    InputData.push_back(ScreenBound);
    InputData.push_back(NumTriangle);
    InputData.push_back(NumMatrial);
    InputData.push_back(MemoryUsage);

    return InputData;
}

std::vector<float> UMLInferenceHelper::PreProcessActor(AActor* Actor, UWorld* World)
{
    // camera distance
    FVector CameraLocation = UGameplayStatics::GetPlayerController(World, 0)->PlayerCameraManager->GetCameraLocation();
    FVector ActorLocation = Actor->GetActorLocation();
    float Distance = FVector::Dist(ActorLocation, CameraLocation);

    // bound
    float ScreenBound = GetActorScreenSize(Actor, World);

    // polygon
    float NumTriangle = GetNumTriangle(Actor);

    // material
    float NumMatrial = GetNumMaterial(Actor);

    // memory usage
    float MemoryUsage = GetMemoryUsage(Actor);

    std::vector<float> InputData;
    InputData.push_back(Distance);
    InputData.push_back(ScreenBound);
    InputData.push_back(NumTriangle);
    InputData.push_back(NumMatrial);
    InputData.push_back(MemoryUsage);

    return InputData;
}

float UMLInferenceHelper::GetActorScreenSize(AActor* TargetActor, UWorld* World)
{
    if (!TargetActor || !GEngine->GetFirstLocalPlayerController(World))
    {
        return 0.0f;
    }

    FVector Origin;
    FVector BoxExtent;

    //액터의 바운딩 박스(경계 상자) 가져오기
    TargetActor->GetActorBounds(true, Origin, BoxExtent);

    //바운딩 박스의 코너 좌표 계산 (8개의 코너)
    TArray<FVector> BoxCorners;
    BoxCorners.Add(Origin + FVector(-BoxExtent.X, -BoxExtent.Y, -BoxExtent.Z));
    BoxCorners.Add(Origin + FVector(BoxExtent.X, -BoxExtent.Y, -BoxExtent.Z));
    BoxCorners.Add(Origin + FVector(-BoxExtent.X, BoxExtent.Y, -BoxExtent.Z));
    BoxCorners.Add(Origin + FVector(BoxExtent.X, BoxExtent.Y, -BoxExtent.Z));
    BoxCorners.Add(Origin + FVector(-BoxExtent.X, -BoxExtent.Y, BoxExtent.Z));
    BoxCorners.Add(Origin + FVector(BoxExtent.X, -BoxExtent.Y, BoxExtent.Z));
    BoxCorners.Add(Origin + FVector(-BoxExtent.X, BoxExtent.Y, BoxExtent.Z));
    BoxCorners.Add(Origin + FVector(BoxExtent.X, BoxExtent.Y, BoxExtent.Z));

    FVector2D MinScreenPos(FLT_MAX, FLT_MAX);
    FVector2D MaxScreenPos(-FLT_MAX, -FLT_MAX);
    FVector2D ScreenPosition;

    //공간 좌표를 화면 좌표로 변환
    for (const FVector& Corner : BoxCorners)
    {
        if (GEngine->GetFirstLocalPlayerController(World)->ProjectWorldLocationToScreen(Corner, ScreenPosition))
        {
            MinScreenPos.X = FMath::Min(MinScreenPos.X, ScreenPosition.X);
            MinScreenPos.Y = FMath::Min(MinScreenPos.Y, ScreenPosition.Y);
            MaxScreenPos.X = FMath::Max(MaxScreenPos.X, ScreenPosition.X);
            MaxScreenPos.Y = FMath::Max(MaxScreenPos.Y, ScreenPosition.Y);
        }
        else
        {
            return 0.0f;
        }
    }
    //화면에서 차지하는 픽셀 크기 계산
    int32 ScreenSizeX = 0;
    int32 ScreenSizeY = 0;
    GEngine->GetFirstLocalPlayerController(World)->GetViewportSize(ScreenSizeX, ScreenSizeY);
    float TotalArea = (float)ScreenSizeX * (float)ScreenSizeY;
    MinScreenPos.X = FMath::Clamp(MinScreenPos.X, 0.f, (float)ScreenSizeX);
    MinScreenPos.Y = FMath::Clamp(MinScreenPos.Y, 0.f, (float)ScreenSizeY);
    MaxScreenPos.X = FMath::Clamp(MaxScreenPos.X, 0.f, (float)ScreenSizeX);
    MaxScreenPos.Y = FMath::Clamp(MaxScreenPos.Y, 0.f, (float)ScreenSizeY);

    float ScreenWidth = MaxScreenPos.X - MinScreenPos.X;
    float ScreenHeight = MaxScreenPos.Y - MinScreenPos.Y;
    float ScreenSize = (ScreenWidth * ScreenHeight);
    float ScreenRatio = ScreenSize / TotalArea;

    return ScreenRatio;
}

float UMLInferenceHelper::GetNumTriangle(AActor* Actor)
{
    float NumTriangle = 0.0f;
    TArray<UStaticMeshComponent*> Components;
    Actor->GetComponents<UStaticMeshComponent>(Components);
    if (Components.IsEmpty())
    {
        return NumTriangle;
    }

    for (int32 i = 0; i < Components.Num(); i++)
    {
        UStaticMeshComponent* StaticMeshComponent = Components[i];
        UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
        NumTriangle += StaticMesh->GetNumTriangles(0);
    }
    return NumTriangle;
}

float UMLInferenceHelper::GetNumMaterial(AActor* Actor)
{
    float NumMaterial = 0.0f;
    TArray<UStaticMeshComponent*> Components;
    Actor->GetComponents<UStaticMeshComponent>(Components);
    if (Components.IsEmpty())
    {
        return NumMaterial;
    }

    for (int32 i = 0; i < Components.Num(); i++)
    {
        UStaticMeshComponent* StaticMeshComponent = Components[i];
        UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
        NumMaterial += StaticMesh->GetStaticMaterials().Num();
    }
    return NumMaterial;
}

float UMLInferenceHelper::GetMemoryUsage(AActor* Actor)
{
    float NumMaterial = 0.0f;
    TArray<UStaticMeshComponent*> Components;
    Actor->GetComponents<UStaticMeshComponent>(Components);
    if (Components.IsEmpty())
    {
        return NumMaterial;
    }

    for (int32 i = 0; i < Components.Num(); i++)
    {
        UStaticMeshComponent* StaticMeshComponent = Components[i];
        UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();
        NumMaterial += StaticMesh->GetResourceSizeBytes(EResourceSizeMode::Type::EstimatedTotal);
    }
    return NumMaterial;
}