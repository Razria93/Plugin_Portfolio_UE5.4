#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SAssetReferenceInspectorWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAssetReferenceInspectorWidget)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};
