#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NaniteMLManager.generated.h"

/**
 * 간단한 UObject 싱글턴 스타일 매니저:
 * - ML 모델 로드/언로드
 * - 매 프레임 인퍼런스(Stub or Real)
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
        UWorld* World,
        int32& OutLodBias,
        float fps
    );

private:
    void* MLModelHandle;
};