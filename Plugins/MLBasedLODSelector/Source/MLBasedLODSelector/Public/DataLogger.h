#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataLogger.generated.h"

/**
  UDataLogger
  CSV파일에 ML 학습을 위한 데이터 로그
 */
UCLASS()
class MLBASEDLODSELECTOR_API UDataLogger : public UObject
{
    GENERATED_BODY()

public:
    static UDataLogger& Get();

    UFUNCTION(BlueprintCallable, Category="logger")
    void StartLogging();

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
    FString LogFilePath;
};