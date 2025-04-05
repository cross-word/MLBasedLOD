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

    // 액터 순회
    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor)
        {
            continue;
        }

        // ML inference
        UNaniteMLManager::Get().RunInferenceForActor(Actor, ViewInfo, World);
    }
}

/*
TODO : log모드 / 추론모드 구별 , LOD Bias 구현
*/