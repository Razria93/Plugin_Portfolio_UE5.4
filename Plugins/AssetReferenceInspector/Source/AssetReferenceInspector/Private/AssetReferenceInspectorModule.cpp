#include "AssetReferenceInspectorModule.h"

#include "Framework/Docking/TabManager.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

static const FName AssetReferenceInspectorTabName(TEXT("AssetReferenceInspector"));

void FAssetReferenceInspectorModule::StartupModule()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AssetReferenceInspectorTabName, FOnSpawnTab::CreateRaw(this, &FAssetReferenceInspectorModule::OnSpawnPluginTab))
		.SetDisplayName(FText::FromString(TEXT("Asset Reference Inspector")))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FAssetReferenceInspectorModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AssetReferenceInspectorTabName);
}

TSharedRef<SDockTab> FAssetReferenceInspectorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Asset Reference Inspector")))
				]
		];
}

IMPLEMENT_MODULE(FAssetReferenceInspectorModule, AssetReferenceInspector)
