// Copyright Epic Games, Inc. All Rights Reserved.

#include "MLBasedLODSelector.h"
#include "NaniteMLManager.h"      
#include "NaniteMLViewExtension.h" 
#include "DataLogger.h"

#include "Modules/ModuleManager.h"
#include "Engine/Engine.h"

IMPLEMENT_MODULE(FMLBasedLODSelectorModule, FMLBasedLODSelector)

void FMLBasedLODSelectorModule::StartupModule()
{
    //GEngine 초기화 후 시행
    PostEngineInitHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &FMLBasedLODSelectorModule::OnPostEngineInit);
}

void FMLBasedLODSelectorModule::ShutdownModule()
{
    ViewExtension.Reset();

    UNaniteMLManager::Get().ShutdownModel();
    FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitHandle);
}

void FMLBasedLODSelectorModule::OnPostEngineInit()
{
    if (GEngine)
    {
        UNaniteMLManager::Get().InitializeModel();
        UDataLogger::Get().StartLogging();

        ViewExtension = FSceneViewExtensions::NewExtension<FNaniteMLViewExtension>();
        UE_LOG(LogTemp, Display, TEXT("MLLOD Initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ERROR: Engine is empty"));
    }
}