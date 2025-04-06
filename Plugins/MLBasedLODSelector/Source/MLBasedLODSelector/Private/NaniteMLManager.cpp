#include "NaniteMLManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraTypes.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "DataLogger.h"
#include "CaptureActor.h"
#include "MLInferenceHelper.h"

UNaniteMLManager& UNaniteMLManager::Get()
{
    static UNaniteMLManager* Singleton = NewObject<UNaniteMLManager>();
    Singleton->AddToRoot();
    return *Singleton;
}

void UNaniteMLManager::InitializeModel()
{
    EnvInstance = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "ONNXModel");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);

    //임시 하드코딩
    const wchar_t* model_path = L"D:\\unreal_project\\SimpleShooter\\PythonAPI\\model.onnx";

    try {
        ModelSession = new Ort::Session(*EnvInstance, model_path, session_options);
    }
    catch (const Ort::Exception&) {
        return;
    }
}

void UNaniteMLManager::ShutdownModel()
{
    ModelSession = nullptr;
    if (EnvInstance != nullptr) delete EnvInstance;
}

/*
  입력 Actor를 바탕으로 모델 inference
  모델 input : Distance, ScreenBound, NumTriangle, NumMatrial, MemoryUsage
  모델 output : LOD Level
*/
void UNaniteMLManager::RunInferenceForActor(
    AActor* Actor,
    const FMinimalViewInfo& ViewInfo,
    UWorld* World
)
{
    if (!Actor || !GEngine->GetFirstLocalPlayerController(World))
    {
        return;
    }
    if (ModelSession == nullptr)
    {
        return;
    }

    TArray<UPrimitiveComponent*> PrimitiveComps;
    Actor->GetComponents<UPrimitiveComponent>(PrimitiveComps);
    bool NoVisibleComponent = true;

    for (UPrimitiveComponent* Comp : PrimitiveComps)
    {
        if (!Comp->bHiddenInGame && Comp->IsVisible())
        {
            NoVisibleComponent = false;
            break;
        }
    }
    if (NoVisibleComponent)
    {
        return;
    }

    //input 데이터 준비
    std::vector<float> InputData;
    InputData = UMLInferenceHelper::Get().PreProcessActor(Actor, World, ViewInfo);
    
    std::vector<int64_t> input_shape = { 1, 5 };
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
    float* output_data;

    //model inference
    try {
        auto output_tensors = ModelSession->Run(Ort::RunOptions{ nullptr },
            input_names, &input_tensor, 1,
            output_names, 1);

        output_data = output_tensors.front().GetTensorMutableData<float>();
    }
    catch (const Ort::Exception&) {
        return;
    }

    int LODIndex = FMath::Clamp(FMath::RoundToInt(*output_data), 1, 5);
    if (UStaticMeshComponent* SMC = Actor->FindComponentByClass<UStaticMeshComponent>())
    {
        TArray<UStaticMeshComponent*> StaticMeshComponents;
        Actor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);
        for (UStaticMeshComponent* StaticComp : StaticMeshComponents)
        {
            if (StaticComp)
            {
                UE_LOG(LogTemp, Warning, TEXT("MODEL CHANGED LOD!!"));
                StaticComp->SetForcedLodModel(LODIndex);
            }
        }
    }
    if (USkeletalMeshComponent* SkMC = Actor->FindComponentByClass<USkeletalMeshComponent>())
    {
        TArray<USkeletalMeshComponent*> SkelMeshComponents;
        Actor->GetComponents<USkeletalMeshComponent>(SkelMeshComponents);
        for (USkeletalMeshComponent* SkelComp : SkelMeshComponents)
        {
            if (SkelComp)
            {
                UE_LOG(LogTemp, Warning, TEXT("MODEL CHANGED LOD!!"));
                SkelComp->SetForcedLOD(LODIndex);
            }
        }
    }
}