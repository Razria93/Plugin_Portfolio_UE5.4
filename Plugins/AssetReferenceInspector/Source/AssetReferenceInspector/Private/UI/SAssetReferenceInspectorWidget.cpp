#include "UI/SAssetReferenceInspectorWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"

#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

#include "Widgets/Input/SButton.h"
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
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Mode: Dependencies / Referencers")))
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
	BuildDependencyTree();
	RefreshTree();

	return FReply::Handled();
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

void SAssetReferenceInspectorWidget::BuildDependencyTree()
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

	BuildDependencyChildren(RootNode, CurrentPath);

	TreeRootItems.Add(RootNode);
}

void SAssetReferenceInspectorWidget::BuildDependencyChildren(TSharedPtr<FAssetReferenceTreeNode> ParentNode, TArray<FName>& CurrentPath) const
{
	if (!ParentNode.IsValid() || ParentNode->Depth >= AnalysisOptions.MaxDepth)
	{
		return;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	TArray<FName> DependencyPackageNames;
	AssetRegistryModule.Get().GetDependencies(ParentNode->PackageName, DependencyPackageNames);

	if (DependencyPackageNames.Num() == 0)
	{
		ParentNode->Children.Add(MakeShared<FAssetReferenceTreeNode>(TEXT("No dependencies found"), NAME_None, ParentNode->Depth + 1));
	}
	else
	{
		for (const FName DependencyPackageName : DependencyPackageNames)
		{
			if (!ShouldIncludeDependencyPackage(DependencyPackageName))
			{
				continue;
			}

			const bool bIsCircular = CurrentPath.Contains(DependencyPackageName);
			TSharedPtr<FAssetReferenceTreeNode> ChildNode = CreateDependencyNode(DependencyPackageName, ParentNode->Depth + 1, bIsCircular);
			ParentNode->Children.Add(ChildNode);

			if (bIsCircular)
			{
				continue;
			}

			CurrentPath.Add(DependencyPackageName);
			BuildDependencyChildren(ChildNode, CurrentPath);
			CurrentPath.Pop();
		}
	}
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

TSharedPtr<FAssetReferenceTreeNode> SAssetReferenceInspectorWidget::CreateDependencyNode(FName PackageName, int32 Depth, bool bIsCircular) const
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

bool SAssetReferenceInspectorWidget::ShouldIncludeDependencyPackage(FName PackageName) const
{
	return PackageName.ToString().StartsWith(TEXT("/Game/"));
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
