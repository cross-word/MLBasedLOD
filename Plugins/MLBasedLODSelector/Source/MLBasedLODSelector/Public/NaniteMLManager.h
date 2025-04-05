#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include <onnxruntime_cxx_api.h>
#include "NaniteMLManager.generated.h"

/*
  onnxruntime으로 ML모델 로드/언로드
  매 프레임 모델에 inference하여 액터마다 LOD를 계산 후 LOD강제
 */
UCLASS()
class MLBASEDLODSELECTOR_API UNaniteMLManager : public UObject
{
    GENERATED_BODY()

public:
    static UNaniteMLManager& Get();
    void InitializeModel();
    void ShutdownModel();

    void RunInferenceForActor(
        class AActor* Actor,
        const struct FMinimalViewInfo& ViewInfo,
        UWorld* World
    );

private:
    Ort::Env* EnvInstance = nullptr;
    Ort::Session* ModelSession = nullptr;
};