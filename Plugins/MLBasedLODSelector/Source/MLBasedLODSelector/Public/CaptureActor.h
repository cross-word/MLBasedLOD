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

    // 캡처 결과를 렌더링할 RenderTarget
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture")
    UTextureRenderTarget2D* RenderTargetAsset;

    // 최대 LOD 레벨 (1=LOD0 ~ MaxLOD)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capture")
    int32 MaxLOD = 5;

    // 실제 캡처 + 로깅 작업 함수
    UFUNCTION(BlueprintCallable, Category = "Capture")
    void CaptureAndLogMultipleLOD();

    virtual void BeginPlay() override;

private:
    // 내부 유틸: RenderTarget → PNG 저장
    void SaveRenderTargetToDisk(UTextureRenderTarget2D* RT, const FString& FullFilePath);

    // 로그를 남길 때, UDataLogger::Get()을 이용
    void LogDataForLOD(float Distance, float ScreenBound, float NumTriangles, float NumMaterials, float MemoryUsage, float Fps, float LodBiasPred);
};