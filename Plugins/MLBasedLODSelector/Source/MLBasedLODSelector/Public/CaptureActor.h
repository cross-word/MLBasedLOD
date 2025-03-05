#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaptureActor.generated.h"

class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

/**
 * ACaptureActor
 * - SceneCaptureComponent2D를 통해 LOD별 화면 캡처
 * - 동시에 UDataLogger로 CSV 로깅
 */
UCLASS()
class MLBASEDLODSELECTOR_API ACaptureActor : public AActor
{
    GENERATED_BODY()

public:
    ACaptureActor();

    // SceneCapture
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture")
    USceneCaptureComponent2D* SceneCapture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture")
    int32 MaxLOD = 5;

    UFUNCTION(BlueprintCallable, Category = "Capture")
    void CaptureAndLogMultipleLOD();

    virtual void BeginPlay() override;

protected:
    virtual void Tick(float DeltaSeconds) override;

private:
    void SaveRenderTargetToDisk(UTextureRenderTarget2D* RT, const FString& FullFilePath);
    TSharedPtr<TFunction<void(int32)>> CaptureLODRecursive;
    float TotalTime = 0.0f;
    float SceneTime = 0.0f;
};