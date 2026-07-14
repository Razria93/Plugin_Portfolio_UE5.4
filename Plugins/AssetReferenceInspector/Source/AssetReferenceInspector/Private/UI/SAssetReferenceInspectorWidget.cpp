#include "UI/SAssetReferenceInspectorWidget.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

FAssetReferenceDummyNode::FAssetReferenceDummyNode(const FString& InName)
	: Name(InName)
{
}

void SAssetReferenceInspectorWidget::Construct(const FArguments& InArgs)
{
	BuildDummyTree();

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
								.Text(FText::FromString(TEXT("Selected Asset: None")))
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
								]

								+ SUniformGridPanel::Slot(1, 0)
								[
									SNew(SButton)
										.Text(FText::FromString(TEXT("Analyze")))
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
									SAssignNew(TreeView, STreeView<TSharedPtr<FAssetReferenceDummyNode>>)
										.TreeItemsSource(&TreeRootItems)
										.OnGenerateRow(this, &SAssetReferenceInspectorWidget::OnGenerateTreeRow)
										.OnGetChildren(this, &SAssetReferenceInspectorWidget::OnGetTreeChildren)
								]
						]
				]
		];

	if (TreeView.IsValid())
	{
		for (const TSharedPtr<FAssetReferenceDummyNode>& RootItem : TreeRootItems)
		{
			TreeView->SetItemExpansion(RootItem, true);
		}
	}
}

void SAssetReferenceInspectorWidget::BuildDummyTree()
{
	TreeRootItems.Reset();

	TSharedPtr<FAssetReferenceDummyNode> RootNode = MakeShared<FAssetReferenceDummyNode>(TEXT("BP_Player"));
	RootNode->Children.Add(MakeShared<FAssetReferenceDummyNode>(TEXT("SK_Player")));
	RootNode->Children.Add(MakeShared<FAssetReferenceDummyNode>(TEXT("ABP_Player")));

	TSharedPtr<FAssetReferenceDummyNode> MaterialNode = MakeShared<FAssetReferenceDummyNode>(TEXT("M_Player"));
	MaterialNode->Children.Add(MakeShared<FAssetReferenceDummyNode>(TEXT("Player_D")));
	MaterialNode->Children.Add(MakeShared<FAssetReferenceDummyNode>(TEXT("Player_L")));
	RootNode->Children.Add(MaterialNode);

	RootNode->Children.Add(MakeShared<FAssetReferenceDummyNode>(TEXT("PlayerConfig")));

	TreeRootItems.Add(RootNode);
}

TSharedRef<ITableRow> SAssetReferenceInspectorWidget::OnGenerateTreeRow(TSharedPtr<FAssetReferenceDummyNode> Item, const TSharedRef<STableViewBase>& OwnerTable) const
{
	const FString DisplayName = Item.IsValid() ? Item->Name : FString(TEXT("Invalid Node"));

	return SNew(STableRow<TSharedPtr<FAssetReferenceDummyNode>>, OwnerTable)
		[
			SNew(STextBlock)
				.Text(FText::FromString(DisplayName))
		];
}

void SAssetReferenceInspectorWidget::OnGetTreeChildren(TSharedPtr<FAssetReferenceDummyNode> Item, TArray<TSharedPtr<FAssetReferenceDummyNode>>& OutChildren) const
{
	if (Item.IsValid())
	{
		OutChildren.Append(Item->Children);
	}
}
