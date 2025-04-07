// Copyright Epic Games, Inc. All Rights Reserved.

#include "MLBasedLODSelector.h"  
#include "NaniteMLViewExtension.h" 
#include "DataLogger.h"

#include "Modules/ModuleManager.h"
#include "Engine/Engine.h"

IMPLEMENT_MODULE(IMLBasedLODSelectorModule, IMLBasedLODSelector)
UNaniteMLManager* IMLBasedLODSelectorModule::NaniteMLManagerInstance = nullptr;

void IMLBasedLODSelectorModule::StartupModule()
{
    // GEngine 초기화 후 모듈 시행
    PostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &IMLBasedLODSelectorModule::OnPostEngineInit);
    NaniteMLManagerInstance = NewObject<UNaniteMLManager>();
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