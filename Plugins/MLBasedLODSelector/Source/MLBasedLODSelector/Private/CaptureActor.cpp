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
#include "MLInferenceHelper.h"

ACaptureActor& ACaptureActor::Get()
{
    static ACaptureActor* Singleton = NewObject<ACaptureActor>();
    Singleton->AddToRoot();
    return *Singleton;
}

ACaptureActor::ACaptureActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
    RootComponent = SceneCapture;
}

// 기본설정
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

// 5초마다 캡쳐 진행
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

/*
각 액터를 순회하면서 LOD를 강제하면서 데이터 저장
액터 순회 후 render명령을 보낸 후 화면 캡쳐
LOD를 변경하고 액터 순회 재개
*/
void ACaptureActor::CaptureAndLogMultipleLOD()
{
    UWorld* World = GetWorld();
    if (!World || !SceneCapture)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CaptureAndLogMultipleLOD] Setup not complete."));
        return;
    }
    float OriginalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(World);
    UGameplayStatics::SetGlobalTimeDilation(World, 0.0f);
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
                        TArray<UStaticMeshComponent*> StaticMeshComponents;
                        Actor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);
                        for (UStaticMeshComponent* StaticComp : StaticMeshComponents)
                        {
                            if (StaticComp)
                            {
                                StaticComp->SetForcedLodModel(0);
                            }
                        }
                    }
                    if (USkeletalMeshComponent* SkMC = Actor->FindComponentByClass<USkeletalMeshComponent>())
                    {
                        TArray<USkeletalMeshComponent*> SkelMeshComponents;
                        Actor->GetComponents<USkeletalMeshComponent>(SkelMeshComponents);
                        for (USkeletalMeshComponent* SkelComp : SkelMeshComponents)
                        {
                            if (SkelComp)
                            {
                                SkelComp->SetForcedLOD(0);
                            }
                        }
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
                    TArray<UStaticMeshComponent*> StaticMeshComponents;
                    Actor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);
                    for (UStaticMeshComponent* StaticComp : StaticMeshComponents)
                    {
                        if (StaticComp)
                        {
                            StaticComp->SetForcedLodModel(LODIndex);
                            StaticComp->MarkRenderStateDirty();
                        }
                    }
                }
                if (USkeletalMeshComponent* SkMC = Actor->FindComponentByClass<USkeletalMeshComponent>())
                {
                    TArray<USkeletalMeshComponent*> SkelMeshComponents;
                    Actor->GetComponents<USkeletalMeshComponent>(SkelMeshComponents);
                    for (USkeletalMeshComponent* SkelComp : SkelMeshComponents)
                    {
                        if (SkelComp)
                        {
                            SkelComp->SetForcedLOD(LODIndex);
                            SkelComp->MarkRenderStateDirty();
                            int32 CurrentLOD = SkelComp->GetPredictedLODLevel();
                        }
                    }
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

                    FString Scene_id = ProjectName + FString::SanitizeFloat(this->SceneTime);
                    FString DirName = FString::Printf(TEXT("LOD_%d"), LODIndex);
                    FString FullFilePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("MLScreenshots"), DirName, Scene_id + TEXT(".png"));

                    SaveRenderTargetToDisk(SceneCapture->TextureTarget, FullFilePath);
                    // LOD0의 경우 로깅 패스
                    if (LODIndex != 0)
                    {
                        for (AActor* Actor : AllActors)
                        {
                            if (Actor->FindComponentByClass<UStaticMeshComponent>() or Actor->FindComponentByClass<USkeletalMeshComponent>())
                            {
                                std::vector<float>* InputData = {};
                                UMLInferenceHelper::Get().PreProcessActor(InputData, Actor, World);

                                Logger.LogData(
                                    Scene_id,
                                    InputData->at(0),
                                    InputData->at(1),
                                    InputData->at(2),
                                    InputData->at(3),
                                    InputData->at(4),
                                    ms,
                                    LODIndex
                                );
                            }
                        }
                    }

                    // 다음 LOD level을 재귀 호출
                    (*CaptureLODRecursive)(LODIndex + 1);
                });
        };
    SceneTime = World->GetTimeSeconds();
    (*CaptureLODRecursive)(0);
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