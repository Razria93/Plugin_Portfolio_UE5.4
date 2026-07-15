#pragma once

#include "CoreMinimal.h"
#include "AssetRegistry/AssetData.h"
#include "Input/Reply.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STreeView.h"

struct FAssetReferenceDummyNode
{
	explicit FAssetReferenceDummyNode(const FString& InName);

	FString Name;
	TArray<TSharedPtr<FAssetReferenceDummyNode>> Children;
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

	// UI text
	FText GetSelectedAssetText() const;

	// Dummy tree data
	void BuildDummyTree();
	TSharedRef<ITableRow> OnGenerateTreeRow(TSharedPtr<FAssetReferenceDummyNode> Item, const TSharedRef<STableViewBase>& OwnerTable) const;
	void OnGetTreeChildren(TSharedPtr<FAssetReferenceDummyNode> Item, TArray<TSharedPtr<FAssetReferenceDummyNode>>& OutChildren) const;

private:
	// State
	FAssetData SelectedAssetData;
	TArray<TSharedPtr<FAssetReferenceDummyNode>> TreeRootItems;
	TSharedPtr<STreeView<TSharedPtr<FAssetReferenceDummyNode>>> TreeView;
};
