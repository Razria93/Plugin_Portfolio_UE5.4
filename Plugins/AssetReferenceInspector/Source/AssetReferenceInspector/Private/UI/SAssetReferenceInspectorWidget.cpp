#include "UI/SAssetReferenceInspectorWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"

#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

void SAssetReferenceInspectorWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SBorder)
				.Padding(12.0f)
				[
					SNew(SVerticalBox)

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Asset Reference Inspector")))
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 8.0f, 0.0f, 8.0f)
						[
							SNew(SSeparator)
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 0.0f, 0.0f, 8.0f)
						[
							SNew(STextBlock)
								.Text(this, &SAssetReferenceInspectorWidget::GetSelectedAssetText)
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 0.0f, 0.0f, 8.0f)
						[
							SNew(SUniformGridPanel)
								.SlotPadding(4.0f)

								+ SUniformGridPanel::Slot(0, 0)
								[
									SNew(SButton)
										.Text(FText::FromString(TEXT("Pick Selected Asset")))
										.OnClicked(this, &SAssetReferenceInspectorWidget::OnPickSelectedAssetClicked)
								]

								+ SUniformGridPanel::Slot(1, 0)
								[
									SNew(SButton)
										.Text(FText::FromString(TEXT("Analyze")))
										.OnClicked(this, &SAssetReferenceInspectorWidget::OnAnalyzeClicked)
								]
						]

					+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 0.0f, 0.0f, 8.0f)
						[
							SNew(SVerticalBox)

								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(0.0f, 0.0f, 0.0f, 4.0f)
								[
									SNew(STextBlock)
										.Text(this, &SAssetReferenceInspectorWidget::GetCurrentModeText)
								]

								+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(SUniformGridPanel)
										.SlotPadding(4.0f)

										+ SUniformGridPanel::Slot(0, 0)
										[
											SNew(SButton)
												.Text(FText::FromString(TEXT("Dependencies")))
												.OnClicked(this, &SAssetReferenceInspectorWidget::OnDependenciesModeClicked)
										]

										+ SUniformGridPanel::Slot(1, 0)
										[
											SNew(SButton)
												.Text(FText::FromString(TEXT("Referencers")))
												.OnClicked(this, &SAssetReferenceInspectorWidget::OnReferencersModeClicked)
										]
								]
						]

					+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 0.0f, 0.0f, 8.0f)
						[
							SNew(SVerticalBox)

								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(0.0f, 0.0f, 0.0f, 4.0f)
								[
									SNew(STextBlock)
										.Text(FText::FromString(TEXT("Path Filter")))
								]

								+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(SEditableTextBox)
										.Text(this, &SAssetReferenceInspectorWidget::GetPathFilterText)
										.OnTextCommitted(this, &SAssetReferenceInspectorWidget::OnPathFilterTextCommitted)
								]
						]

						+ SVerticalBox::Slot()
						.FillHeight(1.0f)
						[
							SNew(SBorder)
								.Padding(8.0f)
								[
									SAssignNew(TreeView, STreeView<TSharedPtr<FAssetReferenceTreeNode>>)
										.TreeItemsSource(&TreeRootItems)
										.OnGenerateRow(this, &SAssetReferenceInspectorWidget::OnGenerateTreeRow)
										.OnGetChildren(this, &SAssetReferenceInspectorWidget::OnGetTreeChildren)
										.OnMouseButtonDoubleClick(this, &SAssetReferenceInspectorWidget::OnTreeNodeDoubleClicked)
								]
						]
				]
		];

	if (TreeView.IsValid())
	{
		for (const TSharedPtr<FAssetReferenceTreeNode>& RootItem : TreeRootItems)
		{
			TreeView->SetItemExpansion(RootItem, true);
		}
	}
}

FReply SAssetReferenceInspectorWidget::OnPickSelectedAssetClicked()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	TArray<FAssetData> SelectedAssets;
	ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

	SelectedAssetData = SelectedAssets.Num() > 0 ? SelectedAssets[0] : FAssetData();

	return FReply::Handled();
}

FReply SAssetReferenceInspectorWidget::OnAnalyzeClicked()
{
	BuildRelationTree();
	RefreshTree();

	return FReply::Handled();
}

FReply SAssetReferenceInspectorWidget::OnDependenciesModeClicked()
{
	AnalysisOptions.Mode = EAssetReferenceMode::Dependencies;
	return FReply::Handled();
}

FReply SAssetReferenceInspectorWidget::OnReferencersModeClicked()
{
	AnalysisOptions.Mode = EAssetReferenceMode::Referencers;
	return FReply::Handled();
}

void SAssetReferenceInspectorWidget::OnPathFilterTextCommitted(const FText& InText, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnCleared)
	{
		return;
	}

	AnalysisOptions.PathFilter = InText.ToString().TrimStartAndEnd();
}

void SAssetReferenceInspectorWidget::OnTreeNodeDoubleClicked(TSharedPtr<FAssetReferenceTreeNode> Item) const
{
	if (!Item.IsValid())
	{
		return;
	}

	TrySyncContentBrowserToPackage(Item->PackageName);
}

FText SAssetReferenceInspectorWidget::GetSelectedAssetText() const
{
	if (!SelectedAssetData.IsValid())
	{
		return FText::FromString(TEXT("Selected Asset: None"));
	}

	return FText::FromString(FString::Printf(
		TEXT("Selected Asset: %s (%s)"),
		*SelectedAssetData.AssetName.ToString(),
		*SelectedAssetData.PackageName.ToString()));
}

FText SAssetReferenceInspectorWidget::GetCurrentModeText() const
{
	const FString ModeName = AnalysisOptions.Mode == EAssetReferenceMode::Referencers
		? FString(TEXT("Referencers"))
		: FString(TEXT("Dependencies"));

	return FText::FromString(FString::Printf(TEXT("Mode: %s"), *ModeName));
}

FText SAssetReferenceInspectorWidget::GetPathFilterText() const
{
	return FText::FromString(AnalysisOptions.PathFilter);
}

void SAssetReferenceInspectorWidget::BuildRelationTree()
{
	TreeRootItems.Reset();

	if (!SelectedAssetData.IsValid())
	{
		TreeRootItems.Add(MakeShared<FAssetReferenceTreeNode>(TEXT("No selected asset")));
		return;
	}

	TSharedPtr<FAssetReferenceTreeNode> RootNode = MakeShared<FAssetReferenceTreeNode>(
		SelectedAssetData.AssetName.ToString(),
		SelectedAssetData.PackageName,
		0);

	TArray<FName> CurrentPath;
	CurrentPath.Add(SelectedAssetData.PackageName);

	BuildRelationChildren(RootNode, CurrentPath);

	TreeRootItems.Add(RootNode);
}

void SAssetReferenceInspectorWidget::BuildRelationChildren(TSharedPtr<FAssetReferenceTreeNode> ParentNode, TArray<FName>& CurrentPath) const
{
	if (!ParentNode.IsValid() || ParentNode->Depth >= AnalysisOptions.MaxDepth)
	{
		return;
	}

	TArray<FName> RelatedPackageNames;
	GetRelatedPackageNames(ParentNode->PackageName, RelatedPackageNames);

	bool bAddedChild = false;

	for (const FName RelatedPackageName : RelatedPackageNames)
	{
		if (!ShouldIncludeRelatedPackage(RelatedPackageName))
		{
			continue;
		}

		const bool bIsCircular = CurrentPath.Contains(RelatedPackageName);
		TSharedPtr<FAssetReferenceTreeNode> ChildNode = CreateRelationNode(RelatedPackageName, ParentNode->Depth + 1, bIsCircular);
		ParentNode->Children.Add(ChildNode);
		bAddedChild = true;

		if (bIsCircular)
		{
			continue;
		}

		CurrentPath.Add(RelatedPackageName);
		BuildRelationChildren(ChildNode, CurrentPath);
		CurrentPath.Pop();
	}

	if (!bAddedChild)
	{
		ParentNode->Children.Add(MakeShared<FAssetReferenceTreeNode>(GetEmptyRelationMessage(), NAME_None, ParentNode->Depth + 1));
	}
}

void SAssetReferenceInspectorWidget::GetRelatedPackageNames(FName PackageName, TArray<FName>& OutPackageNames) const
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	if (AnalysisOptions.Mode == EAssetReferenceMode::Referencers)
	{
		AssetRegistryModule.Get().GetReferencers(PackageName, OutPackageNames);
		return;
	}

	AssetRegistryModule.Get().GetDependencies(PackageName, OutPackageNames);
}

void SAssetReferenceInspectorWidget::RefreshTree()
{
	if (!TreeView.IsValid())
	{
		return;
	}

	TreeView->RequestTreeRefresh();

	ExpandTreeItems(TreeRootItems);
}

void SAssetReferenceInspectorWidget::ExpandTreeItems(const TArray<TSharedPtr<FAssetReferenceTreeNode>>& Items)
{
	if (!TreeView.IsValid())
	{
		return;
	}

	for (const TSharedPtr<FAssetReferenceTreeNode>& Item : Items)
	{
		if (Item.IsValid())
		{
			TreeView->SetItemExpansion(Item, true);
			ExpandTreeItems(Item->Children);
		}
	}
}

TSharedPtr<FAssetReferenceTreeNode> SAssetReferenceInspectorWidget::CreateRelationNode(FName PackageName, int32 Depth, bool bIsCircular) const
{
	FString DisplayName = PackageName.ToString();

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	TArray<FAssetData> AssetsInPackage;
	AssetRegistryModule.Get().GetAssetsByPackageName(PackageName, AssetsInPackage);

	if (AssetsInPackage.Num() > 0)
	{
		DisplayName = AssetsInPackage[0].AssetName.ToString();
	}

	return MakeShared<FAssetReferenceTreeNode>(DisplayName, PackageName, Depth, bIsCircular);
}

bool SAssetReferenceInspectorWidget::ShouldIncludeRelatedPackage(FName PackageName) const
{
	const FString PathFilter = AnalysisOptions.PathFilter.TrimStartAndEnd();

	if (PathFilter.IsEmpty())
	{
		return true;
	}

	return PackageName.ToString().StartsWith(PathFilter);
}

FString SAssetReferenceInspectorWidget::GetEmptyRelationMessage() const
{
	return AnalysisOptions.Mode == EAssetReferenceMode::Referencers
		? FString(TEXT("No referencers found"))
		: FString(TEXT("No dependencies found"));
}

TSharedRef<ITableRow> SAssetReferenceInspectorWidget::OnGenerateTreeRow(TSharedPtr<FAssetReferenceTreeNode> Item, const TSharedRef<STableViewBase>& OwnerTable) const
{
	const FString DisplayName = Item.IsValid() ? Item->DisplayName : FString(TEXT("Invalid Node"));

	return SNew(STableRow<TSharedPtr<FAssetReferenceTreeNode>>, OwnerTable)
		[
			SNew(STextBlock)
				.Text(FText::FromString(DisplayName))
		];
}

void SAssetReferenceInspectorWidget::OnGetTreeChildren(TSharedPtr<FAssetReferenceTreeNode> Item, TArray<TSharedPtr<FAssetReferenceTreeNode>>& OutChildren) const
{
	if (Item.IsValid())
	{
		OutChildren.Append(Item->Children);
	}
}

bool SAssetReferenceInspectorWidget::TrySyncContentBrowserToPackage(FName PackageName) const
{
	if (PackageName.IsNone())
	{
		return false;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	TArray<FAssetData> AssetsInPackage;
	AssetRegistryModule.Get().GetAssetsByPackageName(PackageName, AssetsInPackage);

	if (AssetsInPackage.Num() == 0 || !AssetsInPackage[0].IsValid())
	{
		return false;
	}

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	TArray<FAssetData> AssetsToSync;
	AssetsToSync.Add(AssetsInPackage[0]);
	ContentBrowserModule.Get().SyncBrowserToAssets(AssetsToSync);

	return true;
}
