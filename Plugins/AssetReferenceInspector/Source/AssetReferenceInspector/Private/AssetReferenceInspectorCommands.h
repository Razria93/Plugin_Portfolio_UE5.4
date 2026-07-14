#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Styling/AppStyle.h"

class FAssetReferenceInspectorCommands : public TCommands<FAssetReferenceInspectorCommands>
{
public:
	FAssetReferenceInspectorCommands();

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> OpenPluginWindow;
};
