// Copyright Raioix & Raiocore. All Rights Reserved 2026.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDACharactersModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

