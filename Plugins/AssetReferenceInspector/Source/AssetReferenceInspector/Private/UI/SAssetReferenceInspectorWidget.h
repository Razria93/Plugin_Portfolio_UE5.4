#pragma once

#include "CoreMinimal.h"
#include "AssetRegistry/AssetData.h"
#include "Input/Reply.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STreeView.h"

struct FAssetReferenceTreeNode
{
	FAssetReferenceTreeNode(const FString& InDisplayName, FName InPackageName = NAME_None);

	FString DisplayName;
	FName PackageName;
	TArray<TSharedPtr<FAssetReferenceTreeNode>> Children;
};

class SAssetReferenceInspectorWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAssetReferenceInspectorWidget)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	// UI callbacks
	FReply OnPickSelectedAssetClicked();
	FReply OnAnalyzeClicked();

	// UI text
	FText GetSelectedAssetText() const;

	// Tree data
	void BuildDependencyTree();
	void RefreshTree();
	TSharedPtr<FAssetReferenceTreeNode> CreateDependencyNode(FName PackageName) const;
	TSharedRef<ITableRow> OnGenerateTreeRow(TSharedPtr<FAssetReferenceTreeNode> Item, const TSharedRef<STableViewBase>& OwnerTable) const;
	void OnGetTreeChildren(TSharedPtr<FAssetReferenceTreeNode> Item, TArray<TSharedPtr<FAssetReferenceTreeNode>>& OutChildren) const;

private:
	// State
	FAssetData SelectedAssetData;
	TArray<TSharedPtr<FAssetReferenceTreeNode>> TreeRootItems;
	TSharedPtr<STreeView<TSharedPtr<FAssetReferenceTreeNode>>> TreeView;
};
