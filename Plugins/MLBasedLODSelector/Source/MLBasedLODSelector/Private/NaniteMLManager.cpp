#include "NaniteMLManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraTypes.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "DataLogger.h"
UNaniteMLManager& UNaniteMLManager::Get()
{
    static UNaniteMLManager* Singleton = NewObject<UNaniteMLManager>();
    return *Singleton;
}

void UNaniteMLManager::InitializeModel()
{
    // 모델 로드용
}

void UNaniteMLManager::ShutdownModel()
{
    // 모델 자원 해제용
}
/*
float GetActorScreenSize(AActor* TargetActor, UWorld* World)
{
    if (!TargetActor || !GEngine->GetFirstLocalPlayerController(World))
    {
        return 0.0f;
    }

    FVector Origin;
    FVector BoxExtent;

    // 1. 액터의 바운딩 박스(경계 상자) 가져오기
    TargetActor->GetActorBounds(true, Origin, BoxExtent);

    // 2. 바운딩 박스의 코너 좌표 계산 (8개의 코너)
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

    // 3. 3D 공간 좌표를 화면 좌표로 변환
    for (const FVector& Corner : BoxCorners)
    {
        if (GEngine->GetFirstLocalPlayerController(World)->ProjectWorldLocationToScreen(Corner, ScreenPosition))
        {
            MinScreenPos.X = FMath::Min(MinScreenPos.X, ScreenPosition.X);
            MinScreenPos.Y = FMath::Min(MinScreenPos.Y, ScreenPosition.Y);
            MaxScreenPos.X = FMath::Max(MaxScreenPos.X, ScreenPosition.X);
            MaxScreenPos.Y = FMath::Max(MaxScreenPos.Y, ScreenPosition.Y);
        }
    }
    // 4. 화면에서 차지하는 픽셀 크기 계산
    float ScreenWidth = MaxScreenPos.X - MinScreenPos.X;
    float ScreenHeight = MaxScreenPos.Y - MinScreenPos.Y;
    float ScreenSize = (ScreenWidth * ScreenHeight);
    //FVector2D ScreenViewPoint;
    //GEngine->GameViewport->GetViewportSize(ScreenViewPoint);
    //float ScreenRatio = (ScreenWidth * ScreenHeight) / (ScreenViewPoint.X * ScreenViewPoint.Y);

    return ScreenSize;
}

float GetNumTriangle(AActor* Actor)
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
        NumTriangle += StaticMesh->GetNumTriangles(2);
    }
    return NumTriangle;
}

float GetNumMaterial(AActor* Actor)
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

float GetMemoryUsage(AActor* Actor)
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
*/

/*
  입력 Actor를 바탕으로 모델 inference
  모델 input : Distance, ScreenBound, NumTriangle, NumMatrial, MemoryUsage
  모델 output : LOD bias
*/
void UNaniteMLManager::RunInferenceForActor(
    AActor* Actor,
    const FMinimalViewInfo& ViewInfo,
    UWorld* World,
    int32& LodBias,
    float fps
)
{
    if (!Actor || !GEngine->GetFirstLocalPlayerController(World))
    {
        return;
    }

    TArray<UPrimitiveComponent*> PrimitiveComps;
    Actor->GetComponents<UPrimitiveComponent>(PrimitiveComps);
    bool NoVisibleComponent = true;

    for (UPrimitiveComponent* Comp : PrimitiveComps)
    {
        if (!Comp->bHiddenInGame && Comp->IsVisible())
        {
            NoVisibleComponent = false;
            break;
        }
    }
    if (NoVisibleComponent)
    {
        return;
    }


    // camera distance
    FVector ActorLocation = Actor->GetActorLocation();
    FVector CameraLocation = ViewInfo.Location;
    float Distance = FVector::Dist(ActorLocation, CameraLocation);

    //학습후 재개
    /*
    // bound
    float ScreenBound = GetActorScreenSize(Actor, World);

    // polygon
    float NumTriangle = GetNumTriangle(Actor);

    // material
    float NumMatrial = GetNumMaterial(Actor);

    // memory usage
    float MemoryUsage = GetMemoryUsage(Actor);
    
    //mesh name (일단 취소)
    //FString ActorName = Actor->FindComponentByClass<UPrimitiveComponent>()->GetFNameForStatID().ToString();

    //input vector
    TArray<float> InputData;
    InputData.Add(Distance);
    InputData.Add(ScreenBound);
    InputData.Add(NumTriangle);
    InputData.Add(NumMatrial);
    InputData.Add(MemoryUsage);
    */

    //model inference 아직 미구현
    float LodBiasPred = 0.0f; // -2 ~ +2 사이 예측값

    float CullThreshold = 0.5f;
    LodBias = FMath::Clamp(FMath::RoundToInt(LodBiasPred), -2, 2);
}


/*
TODO : 모델 학습 후 inference구현
*/