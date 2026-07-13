#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FAssetReferenceInspectorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
};
