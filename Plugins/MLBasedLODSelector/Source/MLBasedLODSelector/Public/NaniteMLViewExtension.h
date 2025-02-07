#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

class FNaniteMLViewExtension : public FSceneViewExtensionBase
{
public:
    FNaniteMLViewExtension(const FAutoRegister& AutoReg) 
        : FSceneViewExtensionBase(AutoReg) {}

    // ISceneViewExtension 인터페이스
    virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
    virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}
    virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}
    virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& ViewFamily) override {}
    virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override {}
};