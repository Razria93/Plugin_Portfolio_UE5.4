
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
	int32 MaxDepth = 2;
	FString PathFilter = TEXT("/Game/");
};

struct FAssetReferenceTreeNode
{
	explicit FAssetReferenceTreeNode(FString InDisplayName, FName InPackageName = NAME_None, int32 InDepth = 0, bool bInIsCircular = false)
		: DisplayName(MoveTemp(InDisplayName))
		, PackageName(InPackageName)
		, Depth(InDepth)
		, bIsCircular(bInIsCircular)
	{
	}

	FString DisplayName;
	FName PackageName;
	int32 Depth = 0;
	bool bIsCircular = false;
	TArray<TSharedPtr<FAssetReferenceTreeNode>> Children;
};
