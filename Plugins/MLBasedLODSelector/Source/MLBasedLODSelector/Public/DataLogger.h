#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataLogger.generated.h"

/**
 * UDataLogger
 * - CSV/JSON 등으로 ML용 데이터 로그
 * - 싱글턴 접근 (static Get())으로 어디서든 호출 가능
 */
UCLASS()
class MLBASEDLODSELECTOR_API UDataLogger : public UObject
{
    GENERATED_BODY()

public:
    // 싱글턴 스타일 접근
    static UDataLogger& Get();

    UFUNCTION(BlueprintCallable, Category="logger")
    void StartLogging();

    // 액터 정보를 기록
    void LogData(
        FString Scene_id,
        float Distance,
        float ScreenBound,
        float NumTriangle,
        float NumMatrial,
        float MemoryUsage,
        float fps,
        float LodBiasPred
    );

private:
    // 로깅에 쓸 파일 경로
    FString LogFilePath;
};