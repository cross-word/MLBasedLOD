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
    // 싱글턴 접근
    static UNaniteMLManager& Get();

    // 모델 초기화 / 종료
    void InitializeModel();
    void ShutdownModel();

    // 모델 적용
    void RunInferenceForActor(
        class AActor* Actor,
        const struct FMinimalViewInfo& ViewInfo,
        UWorld* World,
        int32& OutLodBias,
        float fps
    );

private:
    // 실제 ML 라이브러리 세션 or 모듈 포인터
    void* MLModelHandle;
};