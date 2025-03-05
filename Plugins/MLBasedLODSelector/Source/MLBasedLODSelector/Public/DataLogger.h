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

    // 로그 시작 (파일 열기 등)
    UFUNCTION(BlueprintCallable, Category="logger")
    void StartLogging();

    // 로그 종료 (파일 닫기 등)
    UFUNCTION(BlueprintCallable, Category = "logger")
    void StopLogging();

    // 매 프레임(or 주기) 액터 정보를 기록
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
    // 실제 로깅에 쓸 파일 경로
    FString LogFilePath;

    // 파일에 한 줄씩 바로 append할지,
    // 메모리에 모아뒀다가 나중에 flush할지 여부
    bool bIsLogging = false;
};