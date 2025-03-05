#include "CaptureActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Modules/ModuleManager.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "ImageUtils.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "DataLogger.h"
#include "Camera/CameraComponent.h"
#include "EditorSupportDelegates.h"
#include "RenderCommandFence.h"

ACaptureActor::ACaptureActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
    RootComponent = SceneCapture;
}

void ACaptureActor::BeginPlay()
{
    Super::BeginPlay();
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return;

    APawn* PlayerPawn = PC->GetPawn();
    if (!PlayerPawn) return;

    UCameraComponent* CamComp = PlayerPawn->FindComponentByClass<UCameraComponent>();
    if (CamComp)
    {
        AttachToComponent(
            CamComp,
            FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true)
        );
    }
    else
    {
        AttachToActor(
            PlayerPawn,
            FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true)
        );
    }

    // SceneCapture 기본 설정
    if (SceneCapture)
    {
        SceneCapture->bCaptureEveryFrame = false;
        SceneCapture->bCaptureOnMovement = false;
    }
}

void ACaptureActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    TotalTime += DeltaSeconds;
    if (TotalTime > 5.0f)
    {
        TotalTime = 0.0f;
        CaptureAndLogMultipleLOD();
    }
}

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
        else
        {
            return 0.0f;
        }
    }
    // 4. 화면에서 차지하는 픽셀 크기 계산
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
    //FVector2D ScreenViewPoint;
    //GEngine->GameViewport->GetViewportSize(ScreenViewPoint);
    //float ScreenRatio = (ScreenWidth * ScreenHeight) / (ScreenViewPoint.X * ScreenViewPoint.Y);

    return ScreenRatio;
}

float GetNumTriangle(AActor* Actor, int32 LOD)
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
        NumTriangle += StaticMesh->GetNumTriangles(FMath::Max(LOD-1, 0));
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

void WaitForGPUFinish()
{
    FRenderCommandFence RenderFence;
    ENQUEUE_RENDER_COMMAND(WaitForGpu)(
        [](FRHICommandListImmediate& RHICmdList)
        {
            RHICmdList.SubmitCommandsAndFlushGPU();
            RHICmdList.BlockUntilGPUIdle();
        });
    RenderFence.BeginFence();
    RenderFence.Wait();
}

void ACaptureActor::CaptureAndLogMultipleLOD()
{
    UWorld* World = GetWorld();
    if (!World || !SceneCapture)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CaptureAndLogMultipleLOD] Setup not complete."));
        return;
    }
    float OriginalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(World);
    //components to log
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), AllActors);

    FString BaseDir = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("MultiLOD"));
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    PlatformFile.CreateDirectoryTree(*BaseDir);

    CaptureLODRecursive = MakeShared<TFunction<void(int32)>>();
    *CaptureLODRecursive = [this, World, AllActors, OriginalTimeDilation](int32 LODIndex)
    {
        if (LODIndex > MaxLOD)
        {
            for (AActor* Actor : AllActors)
            {
                if (UStaticMeshComponent* SMC = Actor->FindComponentByClass<UStaticMeshComponent>())
                {
                    SMC->SetForcedLodModel(0);
                }
                if (USkeletalMeshComponent* SkMC = Actor->FindComponentByClass<USkeletalMeshComponent>())
                {
                    SkMC->SetForcedLOD(0);
                }
            }
            UGameplayStatics::SetGlobalTimeDilation(World, OriginalTimeDilation);
            CaptureLODRecursive.Reset();
            return;
        }
        FlushRenderingCommands();
        for (AActor* Actor : AllActors)
        {
            if (UStaticMeshComponent* SMC = Actor->FindComponentByClass<UStaticMeshComponent>())
            {
                SMC->SetForcedLodModel(LODIndex);
                SMC->MarkRenderStateDirty();
            }
            else if (USkeletalMeshComponent* SkMC = Actor->FindComponentByClass<USkeletalMeshComponent>())
            {
                SkMC->SetForcedLOD(LODIndex);
                SkMC->MarkRenderStateDirty();
            }
        }
        double StartTime = FPlatformTime::Seconds();
        FlushRenderingCommands();
        double EndTime = FPlatformTime::Seconds();
        double ms = (EndTime - StartTime) * 1000.0;
        GetWorld()->GetTimerManager().SetTimerForNextTick([this, LODIndex, World, AllActors, ms]()
        {
            SceneCapture->CaptureScene();

            FString ProjectName = FApp::GetProjectName();
            UDataLogger& Logger = UDataLogger::Get();
            FVector CameraLocation = UGameplayStatics::GetPlayerController(this, 0)->PlayerCameraManager->GetCameraLocation();

            FString Scene_id = ProjectName + FString::SanitizeFloat(this->SceneTime);
            FString DirName = FString::Printf(TEXT("LOD_%d"), LODIndex);
            FString FullFilePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("MLScreenshots"), DirName, Scene_id + TEXT(".png"));

            SaveRenderTargetToDisk(SceneCapture->TextureTarget, FullFilePath);

            for (AActor* Actor : AllActors)
            {
                if (Actor->FindComponentByClass<UStaticMeshComponent>() or Actor->FindComponentByClass<USkeletalMeshComponent>())
                {
                    // camera distance
                    FVector ActorLocation = Actor->GetActorLocation();
                    float Distance = FVector::Dist(ActorLocation, CameraLocation);

                    // bound
                    float ScreenBound = GetActorScreenSize(Actor, World);

                    // polygon
                    float NumTriangle = GetNumTriangle(Actor, LODIndex);

                    // material
                    float NumMatrial = GetNumMaterial(Actor);

                    // memory usage
                    float MemoryUsage = GetMemoryUsage(Actor);

                    Logger.LogData(
                        Scene_id,
                        Distance,
                        ScreenBound,
                        NumTriangle,
                        NumMatrial,
                        MemoryUsage,
                        ms,
                        LODIndex
                    );
                }
            }

            // 다음 LOD를 재귀 호출
            (*CaptureLODRecursive)(LODIndex + 1);
        });
    };
    SceneTime = World->GetTimeSeconds();
    (*CaptureLODRecursive)(1);
    for (AActor* Actor : AllActors)
    {
        if (UStaticMeshComponent* SMC = Actor->FindComponentByClass<UStaticMeshComponent>())
        {
            SMC->SetForcedLodModel(0); // 해제
        }
        if (USkeletalMeshComponent* SkMC = Actor->FindComponentByClass<USkeletalMeshComponent>())
        {
            SkMC->SetForcedLOD(0);
        }
    }
    UGameplayStatics::SetGlobalTimeDilation(World, OriginalTimeDilation);
    return;
}

void ACaptureActor::SaveRenderTargetToDisk(UTextureRenderTarget2D* RT, const FString& FullFilePath)
{
    if (!RT) return;

    FRenderTarget* RenderTargetResource = RT->GameThread_GetRenderTargetResource();
    if (!RenderTargetResource) return;

    FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm, CubeFace_MAX);
    ReadPixelFlags.SetLinearToGamma(false);

    TArray<FColor> OutBMP;
    RenderTargetResource->ReadPixels(OutBMP, ReadPixelFlags);

    int32 Width = RT->SizeX;
    int32 Height = RT->SizeY;

    IImageWrapperModule* ImageWrapperModule = FModuleManager::LoadModulePtr<IImageWrapperModule>(TEXT("ImageWrapper"));
    if (ImageWrapperModule)
    {
        TSharedPtr<IImageWrapper> PNGWriter = ImageWrapperModule->CreateImageWrapper(EImageFormat::PNG);
        PNGWriter->SetRaw(OutBMP.GetData(), OutBMP.GetAllocatedSize(), Width, Height, ERGBFormat::BGRA, 8);

        const TArray64<uint8>& PNGData = PNGWriter->GetCompressed(100);
        FFileHelper::SaveArrayToFile(PNGData, *FullFilePath);
    }
}


/*
TODO: bounding box 수정
*/