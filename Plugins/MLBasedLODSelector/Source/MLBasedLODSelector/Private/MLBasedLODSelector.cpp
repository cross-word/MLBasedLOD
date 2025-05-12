// Copyright Epic Games, Inc. All Rights Reserved.

#include "MLBasedLODSelector.h"  
#include "NaniteMLViewExtension.h" 
#include "DataLogger.h"
#include "EngineUtils.h"
#include "Modules/ModuleManager.h"
#include "Engine/Engine.h"
#include "Delegates/Delegate.h"
#include "Engine/World.h"

IMPLEMENT_MODULE(IMLBasedLODSelectorModule, IMLBasedLODSelector)
UNaniteMLManager* IMLBasedLODSelectorModule::NaniteMLManagerInstance = nullptr;

void IMLBasedLODSelectorModule::StartupModule()
{
    // GEngine 초기화 후 모듈 시행
    PostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &IMLBasedLODSelectorModule::OnPostEngineInit);
    NaniteMLManagerInstance = NewObject<UNaniteMLManager>();

    // 2) 월드가 만들어질 때마다 후보 리스트 구축
    FWorldDelegates::OnPostWorldInitialization.AddLambda(
        [](UWorld* World, const UWorld::InitializationValues&)
        {
            auto* Manager = IMLBasedLODSelectorModule::GetMLManager();
            if (!Manager) return;

            // (A) 월드에 이미 존재하는 액터 등록 1회
            for (TActorIterator<AActor> It(World); It; ++It)
                Manager->RegisterCandidate(*It);

            // (B) 앞으로 스폰되는 액터 등록
            World->AddOnActorSpawnedHandler(
                FOnActorSpawned::FDelegate::CreateLambda(
                    [Manager](AActor* NewActor)
                    {
                        Manager->RegisterCandidate(NewActor);
                    }));
        });
}

void IMLBasedLODSelectorModule::ShutdownModule()
{
    // 모듈 퇴장시 관련된 설정 및 메모리 초기화 
    ViewExtension.Reset();
    if (NaniteMLManagerInstance)
    {
        NaniteMLManagerInstance->ShutdownModel();
        NaniteMLManagerInstance = nullptr;
    }
    FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);
}

void IMLBasedLODSelectorModule::OnPostEngineInit()
{
    if (GEngine)
    {
        NaniteMLManagerInstance->InitializeModel();
        UDataLogger::Get().StartLogging();

        ViewExtension = FSceneViewExtensions::NewExtension<FNaniteMLViewExtension>();
        UE_LOG(LogTemp, Display, TEXT("MLLOD Initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ERROR: Engine is empty"));
    }
}