#include "NaniteMLViewExtension.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "NaniteMLManager.h"
#include "Camera/CameraTypes.h"
#include "DataLogger.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Object.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "CaptureActor.h"

void FNaniteMLViewExtension::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{
    // 월드
    UWorld* World = InViewFamily.Scene ? InViewFamily.Scene->GetWorld() : nullptr;
    if (!World) return;

    if (InViewFamily.Views.Num() == 0)
    {
        return;
    }
    //ACaptureActor CaptureActor;
    //CaptureActor.BeginPlay();

    const FSceneView* SceneView = InViewFamily.Views[0];
    // FMinimalViewInfo 변환 (수작업)
    FMinimalViewInfo ViewInfo;
    {
        ViewInfo.Location = SceneView->ViewLocation;
        ViewInfo.Rotation = SceneView->ViewRotation;
        ViewInfo.FOV = SceneView->FOV;
    }

    // FPS / GPU 변수

    float DeltaTime = InViewFamily.Time.GetDeltaRealTimeSeconds();
    float FPS = (DeltaTime > 0.f) ? (1.f / DeltaTime) : 60.f;
    float GPUUsage = 0.f; // 실제 GPU 통계 구현은 생략
    float TotalTime = InViewFamily.Time.GetRealTimeSeconds();

    if (int(TotalTime) % 5 == 1)
    {
        //CaptureActor.CaptureAndLogMultipleLOD();
        // 다중 LOD로 화면 출력 저장
        TArray<AActor*> AllActors;
        UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), AllActors);

        int32 MaxLOD = 5;
        for (int32 LODIndex = 0; LODIndex < MaxLOD; LODIndex++)
        {
            for (AActor* Actor : AllActors)
            {
                if (UStaticMeshComponent* SMC = Actor->FindComponentByClass<UStaticMeshComponent>())
                {
                    SMC->SetForcedLodModel(LODIndex + 1);
                }
            }

            FString foldername = "LOD_" + FString::FromInt(LODIndex+1);
            FString filename = FString::Printf(
                TEXT("save_L%02d_%06d.png"), // "save_L01_000123.png" 같은 형태
                LODIndex + 1,
                FPlatformTime::Seconds() * 1000 // 타임스탬프 (1000 밀리초 단위 변환)
            );
            FString InFilename = FPaths::Combine(
                FPaths::ProjectSavedDir(),
                TEXT("MLScreenshots"),
                foldername,
                filename
            );

            FlushRenderingCommands();
            // 폴더가 없으면 생성
            IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
            PlatformFile.CreateDirectoryTree(*FPaths::GetPath(InFilename));
            //FScreenshotRequest::RequestScreenshot(InFilename, true, true, false);

            // 액터 순회
            for (TActorIterator<AActor> It(World); It; ++It)
            {
                AActor* Actor = *It;
                if (!Actor)
                {
                    continue;
                }

                /*
                // 특정 태그로 필터링
                if (!Actor->ActorHasTag(FName("NaniteCandidate")))
                {
                    continue;
                }
                */

                int32 LodBias = 0;
                // ML inference
                UNaniteMLManager::Get().RunInferenceForActor(
                    Actor,
                    ViewInfo,
                    World,
                    LodBias,
                    TotalTime
                );
                // LOD bias
                TArray<UPrimitiveComponent*> PrimComps;
                Actor->GetComponents(PrimComps);

                for (UPrimitiveComponent* Comp : PrimComps)
                {
                    if (UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(Comp))
                    {
                        // UE5에서 "SetForcedLOD"는 전통 StaticMesh LOD에 한정.
                        // Nanite 전용 LOD Bias는 별도 기능 or Screen Size 파라미터 조절이 필요할 수 있음.
                        if (LodBias > 0)
                        {
                            SMC->ForcedLodModel = LodBias;
                        }
                        else
                        {
                            SMC->ForcedLodModel = 0; // 0 = no force
                        }
                    }
                }
            }
        }
    }
}

/*
TODO : log모드 / 추론모드 구별
TODO : log시 LODBias 알고리즘 구현
*/