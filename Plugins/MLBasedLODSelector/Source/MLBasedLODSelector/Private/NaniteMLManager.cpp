#include "NaniteMLManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "MLInferenceHelper.h"
#include "EngineUtils.h"

void UNaniteMLManager::InitializeModel()
{
    EnvInstance = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "ONNXModel");
    Ort::SessionOptions session_options;
    //session_options.SetIntraOpNumThreads(1);
    OrtSessionOptionsAppendExecutionProvider_CUDA(session_options, 0);

    //임시 하드코딩
    const wchar_t* model_path = L"D:\\unreal_project\\SimpleShooter\\PythonAPI\\model.onnx";

    try {
        ModelSession = new Ort::Session(*EnvInstance, model_path, session_options);
    }
    catch (const Ort::Exception&) {
        return;
    }

    //프레임(시간) 계산및 모델 inference 자원 업데이트용 Tick
    if (GEngine)
    {
        TickHandle = FTSTicker::GetCoreTicker().AddTicker(
            FTickerDelegate::CreateUObject(this, &UNaniteMLManager::TickInference)
        );
    }
}

void UNaniteMLManager::ShutdownModel()
{
    ModelSession = nullptr;

    FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
    if (EnvInstance)
    {
        delete EnvInstance;
        EnvInstance = nullptr;
    }
}

void UNaniteMLManager::UpdateCameraInfo(UWorld* InWorld, const FMinimalViewInfo& InViewInfo)
{
    CachedWorld = InWorld;
    CachedViewInfo = InViewInfo;
}

/*
  틱마다 시간 계산하여 LOD계산할지말지 정하는 함수
  또한 계산해야하면 액터가 실제로 보이는 액터들만 모아서 함수에 넘겨줌
*/
bool UNaniteMLManager::TickInference(float DeltaTime)
{
    TimeAccumulator += DeltaTime;
    if (TimeAccumulator < InferenceInterval) return true;
    TimeAccumulator = 0.f;
    if (!ModelSession) return true;
    if (bJobRunning) return true;

    // 파괴된 항목 정리 & 시야 테스트
    VisibleSet.Reset();
    CandidateList.RemoveAll([](const TWeakObjectPtr<AActor>& W)
        {
            return !W.IsValid();
        });

    for (const TWeakObjectPtr<AActor>& Weak : CandidateList)
    {
        AActor* Actor = Weak.Get();
        if (!Actor)          continue; 
        if (!Actor->WasRecentlyRendered(0.f)) continue; // 이번 프레임 렌더 x
        VisibleSet.Add(Weak);
    }

    // 부하 제한
    constexpr int32 MaxActorsPerTick = 100;
    TArray<AActor*> ActorsForML;
    ActorsForML.Reserve(FMath::Min(MaxActorsPerTick, VisibleSet.Num()));

    int32 Count = 0;
    for (auto& Weak : VisibleSet)
    {
        if (Count >= MaxActorsPerTick) break;
        if (AActor* A = Weak.Get()) {
            ActorsForML.Add(A);
            ++Count;
        }
    }

    //RunInferenceForActors(ActorsForML);
    StartAsyncInference(ActorsForML);
    return true;
}

/*
 액터들을 검사해서 실제로 보이는 액터들만 모아서 inference
 inference 후 실제로 LOD가 바뀐 액터들만 새로Set
*/
void UNaniteMLManager::RunInferenceForActors(const TArray<AActor*>& Actors)
{
    UE_LOG(LogTemp, Warning, TEXT("LODFunction Called"));
    if (Actors.Num() == 0) return;
    if (ModelSession == nullptr) return;

    //Distance, ScreenBound, NumTriangle, NumMatrial, MemoryUsage 5개feature
    const int32 InputDim = 5;
    std::vector<float> InputData;
    InputData.resize(Actors.Num() * InputDim);

    // 액터별로 PreProcessActor 호출
    for (int32 i = 0; i < Actors.Num(); i++)
    {
        AActor* Actor = Actors[i];
        if (!Actor) continue;

        std::vector<float> OneActorInput = UMLInferenceHelper::Get().PreProcessActor(Actor, CachedWorld, CachedViewInfo);
        // Actor 순서와 모델 input에 맞게 데이터 저장
        for (int32 j = 0; j < InputDim; j++)
        {
            InputData[i * InputDim + j] = OneActorInput[j];
        }
    }

    // input Tensor 생성
    int64_t BatchSize = (int64_t)Actors.Num();
    std::vector<int64_t> input_shape = { BatchSize, InputDim };
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info,
        InputData.data(),
        InputData.size(),
        input_shape.data(),
        input_shape.size()
    );

    const char* input_names[] = { "input" };
    const char* output_names[] = { "output" };

    // inference
    std::vector<Ort::Value> output_tensors;
    try
    {
        output_tensors = ModelSession->Run(
            Ort::RunOptions{ nullptr },
            input_names, &input_tensor, 1,
            output_names, 1
        );
    }
    catch (const Ort::Exception& e)
    {
        UE_LOG(LogTemp, Error, TEXT("ONNX Runtime Exception: %s"), *FString(e.what()));
        return;
    }

    float* output_data = output_tensors.front().GetTensorMutableData<float>();

    // inference 결과 적용
    // output data는 actor순서대로 LOD가 있을것
    for (int32 i = 0; i < Actors.Num(); i++)
    {
        AActor* Actor = Actors[i];
        if (!Actor) continue;

        //액터의 LOD저장테이블 업데이트
        float PredLOD = output_data[i];
        int LODIndex = FMath::Clamp(FMath::RoundToInt(PredLOD), 1, 5);
        int32* FoundLOD = LastLODMap.Find(Actor);
        if (!FoundLOD || *FoundLOD != LODIndex)
        {
            // LOD가 변경되었을 때만 실제 SetForcedLOD 호출
            TArray<UStaticMeshComponent*> StaticMeshComponents;
            Actor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);
            for (UStaticMeshComponent* StaticComp : StaticMeshComponents)
            {
                if (StaticComp)
                {
                    UE_LOG(LogTemp, Warning, TEXT("LODCHANGED!!"));
                    StaticComp->SetForcedLodModel(LODIndex);
                }
            }

            TArray<USkeletalMeshComponent*> SkelMeshComponents;
            Actor->GetComponents<USkeletalMeshComponent>(SkelMeshComponents);
            for (USkeletalMeshComponent* SkelComp : SkelMeshComponents)
            {
                if (SkelComp)
                {
                    UE_LOG(LogTemp, Warning, TEXT("LODCHANGED!!"));
                    SkelComp->SetForcedLOD(LODIndex);
                }
            }

            LastLODMap.Add(Actor, LODIndex);
        }
    }
}

void UNaniteMLManager::StartAsyncInference(const TArray<AActor*>&Actors)
{
    if (Actors.Num() == 0) return;
    bJobRunning = true;

    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask,
        [this, Actors]()
        {
            FInferenceResult Result;
        
            constexpr int32 InputDim = 5;
            std::vector<float> InputData;
            InputData.resize(Actors.Num() * InputDim);
        
            for (int32 i = 0; i < Actors.Num(); ++i)
            {
                AActor * Actor = Actors[i];
                if (!Actor) continue;
            
                FCachedMeshStats & StaticStats = GetCachedStats(Actor);
                FVector ActorLoc = Actor->GetActorLocation();
                float   Distance = FVector::Dist(ActorLoc, CachedViewInfo.Location);
                float   ScreenSize = UMLInferenceHelper::Get().GetActorScreenSize(Actor, CachedWorld);
                InputData[i * InputDim + 0] = Distance;
                InputData[i * InputDim + 1] = ScreenSize;
                InputData[i * InputDim + 2] = StaticStats.NumTriangles;
                InputData[i * InputDim + 3] = StaticStats.NumMaterials;
                InputData[i * InputDim + 4] = StaticStats.MemoryMB;
            }

            int64_t Batch = (int64_t)Actors.Num();
            std::vector<int64_t> Shape = { Batch, InputDim };
            Ort::MemoryInfo MemInfo = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
            Ort::Value InTensor = Ort::Value::CreateTensor<float>(
                MemInfo, InputData.data(), InputData.size(), Shape.data(), Shape.size());

            const char* InNames[] = { "input" };
            const char* OutNames[] = { "output" };
        
            std::vector<Ort::Value> Out;
            try
            {
                Out = ModelSession->Run(Ort::RunOptions{ nullptr }, InNames, &InTensor, 1, OutNames, 1);
            }
            catch (const Ort::Exception& E)
            {
                UE_LOG(LogTemp, Error, TEXT("ONNX Runtime 오류: %s"), *FString(E.what()));
            }

            float* OutData = Out.empty() ? nullptr : Out.front().GetTensorMutableData<float>();
        
            if (OutData)
            {
                for (int32 i = 0; i < Actors.Num(); ++i)
                {
                    if (AActor* Actor = Actors[i])
                    {
                        int32 LODIdx = FMath::Clamp(FMath::RoundToInt(OutData[i]), 1, 5);
                        Result.NewLODMap.Add(Actor, LODIdx);
                    }
                }
            }

            AsyncTask(ENamedThreads::GameThread,
                [this, Result = MoveTemp(Result)]() mutable
                {
                    for (auto& Pair : Result.NewLODMap)
                    {
                        AActor * Actor = Pair.Key.Get();
                        if (!Actor) continue;

                        int32 NewLOD = Pair.Value;
                        int32 PrevLOD = LastLODMap.FindRef(Actor);
                        if (PrevLOD == NewLOD) continue;   // 변동 없으면 스킵
                
                        TArray<UStaticMeshComponent*> SMComps;
                        Actor->GetComponents<UStaticMeshComponent>(SMComps);
                        for (UStaticMeshComponent* C : SMComps)
                        if (C) C->SetForcedLodModel(NewLOD);

                        TArray<USkeletalMeshComponent*> SKComps;
                        Actor->GetComponents<USkeletalMeshComponent>(SKComps);
                        for (USkeletalMeshComponent* C : SKComps)
                        if (C) C->SetForcedLOD(NewLOD);

                        LastLODMap.Add(Actor, NewLOD);
                    }

                    bJobRunning = false;      // 다음 작업 예약 가능
                });
        });
}

FCachedMeshStats & UNaniteMLManager::GetCachedStats(AActor * Actor)
{
    FCachedMeshStats * Found = CachedStats.Find(Actor);
    if (Found) return *Found;

    FCachedMeshStats NewStats;
    TArray<UStaticMeshComponent*> SMComps;
    Actor->GetComponents<UStaticMeshComponent>(SMComps);
    for (UStaticMeshComponent* C : SMComps)
        {
        if (!C) continue;
        UStaticMesh * SM = C->GetStaticMesh();
        if (!SM) continue;
        NewStats.NumTriangles += SM->GetNumTriangles(0);
        NewStats.NumMaterials += SM->GetStaticMaterials().Num();
        NewStats.MemoryMB += SM->GetResourceSizeBytes(EResourceSizeMode::EstimatedTotal) / (1024.f * 1024.f);
        }

    CachedStats.Add(Actor, NewStats);
    return CachedStats[Actor];
}