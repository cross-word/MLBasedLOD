#include "NaniteMLViewExtension.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "MLBasedLODSelector.h"

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
        IMLBasedLODSelectorModule::GetMLManager()->UpdateCameraInfo(World, ViewInfo);
    }
}