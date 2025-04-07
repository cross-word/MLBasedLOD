// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "NaniteMLManager.h"    
#include "Modules/ModuleManager.h"

class IMLBasedLODSelectorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    static UNaniteMLManager* NaniteMLManagerInstance;
    inline static UNaniteMLManager* GetMLManager()
    {
        return NaniteMLManagerInstance;
    }

private:
    TSharedPtr<class FNaniteMLViewExtension, ESPMode::ThreadSafe> ViewExtension;
    FDelegateHandle PostEngineInitHandle;

    void OnPostEngineInit();
};