#pragma once

#include "CoreMinimal.h"

enum class EAssetReferenceMode : uint8
{
	Dependencies,
	Referencers
};

struct FAssetReferenceAnalysisOptions
{
	EAssetReferenceMode Mode = EAssetReferenceMode::Dependencies;
	int32 MaxDepth = 1;
};

struct FAssetReferenceTreeNode
{
	explicit FAssetReferenceTreeNode(FString InDisplayName, FName InPackageName = NAME_None, int32 InDepth = 0)
		: DisplayName(MoveTemp(InDisplayName))
		, PackageName(InPackageName)
		, Depth(InDepth)
	{
	}

	FString DisplayName;
	FName PackageName;
	int32 Depth = 0;
	TArray<TSharedPtr<FAssetReferenceTreeNode>> Children;
};
