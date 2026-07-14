#include "AssetReferenceInspectorModule.h"

#include "AssetReferenceInspectorCommands.h"

#include "Framework/Commands/UICommandList.h"
#include "Framework/Docking/TabManager.h"
#include "Modules/ModuleManager.h"

#include "ToolMenus.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

static const FName AssetReferenceInspectorTabName(TEXT("AssetReferenceInspector"));

void FAssetReferenceInspectorModule::StartupModule()
{
	FAssetReferenceInspectorCommands::Register();

	PluginCommands = MakeShared<FUICommandList>();

	PluginCommands->MapAction(
		FAssetReferenceInspectorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FAssetReferenceInspectorModule::OpenPluginWindow),
		FCanExecuteAction());

	// Create Delegate Object and Regist to Normad Tab Spawner
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AssetReferenceInspectorTabName, FOnSpawnTab::CreateRaw(this, &FAssetReferenceInspectorModule::OnSpawnPluginTab))
		.SetDisplayName(FText::FromString(TEXT("Asset Reference Inspector")))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAssetReferenceInspectorModule::RegisterMenus));
}

void FAssetReferenceInspectorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AssetReferenceInspectorTabName);

	FAssetReferenceInspectorCommands::Unregister();
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

void FAssetReferenceInspectorModule::OpenPluginWindow()
{
	FGlobalTabmanager::Get()->TryInvokeTab(AssetReferenceInspectorTabName);
}

void FAssetReferenceInspectorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.MainMenu.Window"));
	FToolMenuSection& Section = Menu->FindOrAddSection(TEXT("WindowLayout"));
	Section.AddMenuEntryWithCommandList(FAssetReferenceInspectorCommands::Get().OpenPluginWindow, PluginCommands);
}

IMPLEMENT_MODULE(FAssetReferenceInspectorModule, AssetReferenceInspector)
