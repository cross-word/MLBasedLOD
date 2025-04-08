#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

/*
 FSceneViewExtensionBase를 상속받아 렌더링 파이프라인의 일부를 수정
 머신러닝 모델의 inference 에 필요한 자료들을 업데이트해줌.
 참고 https://dev.epicgames.com/community/learning/knowledge-base/0ql6/unreal-engine-using-sceneviewextension-to-extend-the-rendering-system?locale=de-de&locale=ko-kr 
*/
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