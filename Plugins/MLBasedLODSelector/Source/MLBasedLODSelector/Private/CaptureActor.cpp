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

ACaptureActor::ACaptureActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
    RootComponent = SceneCapture;
}

void ACaptureActor::BeginPlay()
{
    Super::BeginPlay();

    // SceneCapture 기본 설정
    if (SceneCapture)
    {
        SceneCapture->bCaptureEveryFrame = false;
        SceneCapture->bCaptureOnMovement = false;

        if (RenderTargetAsset)
        {
            SceneCapture->TextureTarget = RenderTargetAsset;
        }
    }
}

void ACaptureActor::CaptureAndLogMultipleLOD()
{
    UWorld* MyWorld = GetWorld();
    if (!MyWorld || !SceneCapture || !RenderTargetAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CaptureAndLogMultipleLOD] Setup not complete."));
        return;
    }
    if (int(MyWorld->TimeSeconds) % 5 != 4)
    {
        return;
    }

    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(MyWorld, AActor::StaticClass(), AllActors);

    FString BaseDir = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("MultiLOD"));
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    PlatformFile.CreateDirectoryTree(*BaseDir);

    for (int32 LODIndex = 1; LODIndex <= MaxLOD; LODIndex++)
    {
        for (AActor* Actor : AllActors)
        {
            if (UStaticMeshComponent* SMC = Actor->FindComponentByClass<UStaticMeshComponent>())
            {
                SMC->SetForcedLodModel(LODIndex);
            }
        }

        SceneCapture->CaptureScene();
        FlushRenderingCommands();

        FString DirName = FString::Printf(TEXT("LOD_%d"), LODIndex);
        FString GuidString = FGuid::NewGuid().ToString(EGuidFormats::Short);
        FString FileName = FString::Printf(TEXT("save_%s.png"),*GuidString);
        FString FullFilePath = FPaths::Combine(FPaths::ProjectSavedDir(),
            TEXT("MLScreenshots"),
            DirName,
            FileName);
        SaveRenderTargetToDisk(RenderTargetAsset, FullFilePath);

        UE_LOG(LogTemp, Log, TEXT("[CaptureAndLogMultipleLOD] Captured LOD=%d -> %s"), LODIndex, *FullFilePath);

        float ExampleDistance = 1000.f + LODIndex * 500.f;
        float ExampleScreenBound = 50.f + LODIndex * 10.f;
        float ExampleNumTriangles = 2000.f + LODIndex * 500.f;
        float ExampleNumMaterials = 2.f + LODIndex;
        float ExampleMemoryUsage = 100.f + LODIndex * 10.f;
        float ExampleFPS = 60.f;
        float ExampleLodBiasPred = LODIndex; // 임시

        LogDataForLOD(
            ExampleDistance,
            ExampleScreenBound,
            ExampleNumTriangles,
            ExampleNumMaterials,
            ExampleMemoryUsage,
            ExampleFPS,
            ExampleLodBiasPred
        );
    }

    for (AActor* Actor : AllActors)
    {
        if (UStaticMeshComponent* SMC = Actor->FindComponentByClass<UStaticMeshComponent>())
        {
            SMC->SetForcedLodModel(0); // 해제
        }
    }
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

void ACaptureActor::LogDataForLOD(float Distance, float ScreenBound, float NumTriangles, float NumMaterials, float MemoryUsage, float Fps, float LodBiasPred)
{
    UDataLogger& Logger = UDataLogger::Get();

    Logger.LogData(
        Distance,
        ScreenBound,
        NumTriangles,
        NumMaterials,
        MemoryUsage,
        Fps,
        LodBiasPred
    );
}
