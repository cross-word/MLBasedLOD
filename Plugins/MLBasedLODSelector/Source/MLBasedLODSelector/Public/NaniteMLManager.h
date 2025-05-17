#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Containers/Ticker.h"
#include <onnxruntime_cxx_api.h>
#include "Async/Async.h"
#include "HAL/ThreadSafeBool.h"
#include "Containers/Queue.h"

#include "NaniteMLManager.generated.h"

/*
  onnxruntime으로 ML모델 로드/언로드
  N프레임(조정중)마다 모델에 inference하여 액터마다 LOD를 계산 후 LOD강제
 */

struct FInferenceResult
{
    TMap<TWeakObjectPtr<AActor>, int32> NewLODMap;
};

struct FCachedMeshStats   //전처리 값 캐싱
{
   int32 NumTriangles = 0;
   int32 NumMaterials = 0;
   float MemoryMB = 0.f;
};


UCLASS()
class MLBASEDLODSELECTOR_API UNaniteMLManager : public UObject
{
    GENERATED_BODY()

public:
    void InitializeModel();
    void ShutdownModel();
    TArray<TWeakObjectPtr<AActor>> CandidateList;   // 레벨에 들어온 액터
    TSet<TWeakObjectPtr<AActor>>   VisibleSet;     // 이번 프레임에 보인 액터

    void UpdateCameraInfo(UWorld* InWorld, const FMinimalViewInfo& InViewInfo);
    bool TickInference(float DeltaTime);

    void RegisterCandidate(AActor* Actor) { CandidateList.Add(Actor); }
    void UnregisterCandidate(AActor* Actor) { CandidateList.Remove(Actor); }

    FCriticalSection ResultCS;
    TQueue<FInferenceResult, EQueueMode::Mpsc> ResultQueue;
    TAtomic<bool> bJobRunning{ false };

private:
    Ort::Env* EnvInstance = nullptr;
    Ort::Session* ModelSession = nullptr;

    FTSTicker::FDelegateHandle TickHandle;
    float TimeAccumulator = 0.0f;
    float InferenceInterval = 0.5f; // n초마다 LOD갱신

    TMap<TWeakObjectPtr<AActor>, FCachedMeshStats> CachedStats;
    UWorld* CachedWorld = nullptr;
    FMinimalViewInfo CachedViewInfo;
    TMap<TWeakObjectPtr<AActor>, int32> LastLODMap;

    void RunInferenceForActors(const TArray<AActor*>& Actors);
    void RunInference_BgThread(FInferenceResult& Out);
    void ApplyResults();

    void StartAsyncInference(const TArray<AActor*>&Actors);
    FCachedMeshStats & GetCachedStats(AActor * Actor);
};