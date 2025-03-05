#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataLogger.generated.h"

/**
 * UDataLogger
 * - CSV/JSON ������ ML�� ������ �α�
 * - �̱��� ���� (static Get())���� ��𼭵� ȣ�� ����
 */
UCLASS()
class MLBASEDLODSELECTOR_API UDataLogger : public UObject
{
    GENERATED_BODY()

public:
    // �̱��� ��Ÿ�� ����
    static UDataLogger& Get();

    // �α� ���� (���� ���� ��)
    UFUNCTION(BlueprintCallable, Category="logger")
    void StartLogging();

    // �α� ���� (���� �ݱ� ��)
    UFUNCTION(BlueprintCallable, Category = "logger")
    void StopLogging();

    // �� ������(or �ֱ�) ���� ������ ���
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
    // ���� �α뿡 �� ���� ���
    FString LogFilePath;

    // ���Ͽ� �� �پ� �ٷ� append����,
    // �޸𸮿� ��Ƶ״ٰ� ���߿� flush���� ����
    bool bIsLogging = false;
};