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
    UWorld* World = InViewFamily.Scene ? InViewFamily.Scene->GetWorld() : nullptr;
    if (!World) return;

    if (InViewFamily.Views.Num() == 0)
    {
        return;
    }

    const FSceneView* SceneView = InViewFamily.Views[0];
    // FMinimalViewInfo 변환
    FMinimalViewInfo ViewInfo;
    {
        ViewInfo.Location = SceneView->ViewLocation;
        ViewInfo.Rotation = SceneView->ViewRotation;
        ViewInfo.FOV = SceneView->FOV;
    }

    float DeltaTime = InViewFamily.Time.GetDeltaRealTimeSeconds();
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

            // 액터 순회
            for (TActorIterator<AActor> It(World); It; ++It)
            {
                AActor* Actor = *It;
                if (!Actor)
                {
                    continue;
                }

                int32 LodBias = 0;
                // ML inference
                UNaniteMLManager::Get().RunInferenceForActor(
                    Actor,
                    ViewInfo,
                    World,
                    LodBias,
                    TotalTime
                );
                // LOD bias 아직 미구현
                TArray<UPrimitiveComponent*> PrimComps;
                Actor->GetComponents(PrimComps);

                for (UPrimitiveComponent* Comp : PrimComps)
                {
                    if (UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(Comp))
                    {
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
TODO : log모드 / 추론모드 구별 , LOD Bias 구현
*/