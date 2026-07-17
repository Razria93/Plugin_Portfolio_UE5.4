# Asset Registry Analysis Flow

이 문서는 `AssetReferenceInspector`에서 선택 Asset의 Dependencies / Referencers 관계를 Asset Registry로 조회해 Tree View에 표시하는 흐름을 정리한다.

## 현재 범위

현재 구현 범위는 Dependencies / Referencers 모드 전환과 Max Depth 기반 재귀 Tree 생성이다.

```text
선택 Asset
-> 현재 모드에 맞는 관련 Package 목록 조회
-> Tree View 자식 노드로 표시
```

아직 포함하지 않는 범위:

- Engine / Plugin Content 필터
- 순환 참조 시각 강조
- CSV Export

## 입력 Asset

분석 입력은 `SAssetReferenceInspectorWidget`이 보관하는 `SelectedAssetData`다.

```cpp
FAssetData SelectedAssetData;
```

`Pick Selected Asset` 버튼을 누르면 Content Browser의 현재 선택 Asset 중 첫 번째 Asset을 저장한다.

Asset Registry 조회는 `SelectedAssetData.PackageName`을 기준으로 수행한다.

```text
SelectedAssetData.AssetName
= UI 표시용 이름

SelectedAssetData.PackageName
= Asset Registry 조회 기준
```

## Analyze 흐름

`Analyze` 버튼은 `OnAnalyzeClicked`에 연결된다.

```text
Analyze 클릭
-> OnAnalyzeClicked
-> BuildRelationTree
-> RefreshTree
```

선택 Asset이 없으면 `No selected asset` 노드를 표시하고 종료한다. 선택 Asset이 있으면 선택 Asset을 Tree 루트로 만든다.

## Asset Registry 조회

Asset Registry 접근은 `FAssetRegistryModule`을 통해 수행한다.

```cpp
FAssetRegistryModule& AssetRegistryModule =
	FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
```

현재 모드는 `FAssetReferenceAnalysisOptions::Mode`가 결정한다.

```cpp
enum class EAssetReferenceMode : uint8
{
	Dependencies,
	Referencers
};
```

실제 Asset Registry 조회는 `GetRelatedPackageNames`에서 분기한다.

```cpp
if (AnalysisOptions.Mode == EAssetReferenceMode::Referencers)
{
	AssetRegistryModule.Get().GetReferencers(PackageName, OutPackageNames);
	return;
}

AssetRegistryModule.Get().GetDependencies(PackageName, OutPackageNames);
```

반환되는 값은 Asset 객체 포인터가 아니라 PackageName 목록이다.

```text
Dependencies:
/Game/ARI_Demo/BP_Dummy
-> /Game/ARI_Demo/M_Dummy

Referencers:
/Game/ARI_Demo/M_Dummy
<- /Game/ARI_Demo/BP_Dummy
```

따라서 UI에 표시할 이름은 PackageName을 그대로 쓰거나, 해당 Package의 `FAssetData`를 다시 조회해 `AssetName`으로 변환한다.

플러그인 이름의 `Reference`는 Unreal Editor에서 통용되는 Asset 참조 관계 전체를 뜻한다. 내부 처리 함수는 Dependencies / Referencers 양방향 조회를 모두 다루므로 `Relation` 계열 이름을 사용한다.

## Tree Node 구성

현재 Tree View 노드는 `FAssetReferenceTreeNode`다.

이 타입은 UI 위젯 내부 구조체가 아니라 `Private/Analysis/AssetReferenceTypes.h`에 둔다. 외부 모듈에 공개할 필요가 없는 내부 분석 데이터이므로 `Public` API로 노출하지 않는다.

```cpp
struct FAssetReferenceTreeNode
{
	FString DisplayName;
	FName PackageName;
	int32 Depth;
	TArray<TSharedPtr<FAssetReferenceTreeNode>> Children;
};
```

분석 방향과 실행 옵션도 같은 Analysis 영역의 내부 타입으로 둔다.

```cpp
enum class EAssetReferenceMode : uint8
{
	Dependencies,
	Referencers
};

struct FAssetReferenceAnalysisOptions
{
	EAssetReferenceMode Mode;
	int32 MaxDepth;
};
```

`FAssetReferenceAnalysisOptions::MaxDepth`는 재귀 Tree 생성의 깊이 제한으로 사용한다.

```text
Depth 0 = 선택 Asset root
Depth 1 = root의 직접 관련 Package
Depth 2 = Depth 1 노드의 관련 Package
```

루트 노드:

```text
DisplayName = 선택 Asset 이름
PackageName = 선택 Asset PackageName
```

자식 노드:

```text
DisplayName = 관련 AssetName 또는 PackageName
PackageName = 관련 PackageName
```

조회 결과가 없으면 현재 모드에 따라 `No dependencies found` 또는 `No referencers found` 노드를 자식으로 추가한다.

## Max Depth 재귀 생성

관계 Tree는 `BuildRelationChildren`에서 DFS 방식으로 생성한다.

```text
BuildRelationTree
-> root node 생성
-> CurrentPath에 root PackageName 추가
-> BuildRelationChildren(root, CurrentPath)
```

`BuildRelationChildren`은 다음 조건에서 자식 조회를 중단한다.

- ParentNode가 유효하지 않음
- ParentNode의 Depth가 `MaxDepth` 이상
- 현재 DFS 경로에 이미 같은 PackageName이 있음

현재 경로 중복 검사는 전체 방문 여부가 아니라 DFS 경로 기준이다. 같은 Asset이 다른 경로에서 다시 나타나는 것은 별도 경로의 결과로 볼 수 있으므로 전역 visited로 막지 않는다.

### 순환 후보 노드 처리 정책

현재 DFS 경로에 이미 포함된 PackageName을 다시 만나면 순환 후보로 본다.

이 경우 해당 Package는 Parent와 실제 관계가 있으므로 Tree에는 추가한다. 다만 그 아래로 다시 확장하면 무한 재귀가 될 수 있으므로 children 조회는 중단한다.

```text
A
  B
    A
```

위 구조에서 마지막 `A`는 표시하지만, 다시 `B`로 확장하지 않는다.

이 정책은 순환 참조 정보를 숨기지 않기 위한 것이다. 후속 순환 참조 탐지 단계에서는 `bIsCircular` 상태를 UI에 표시해 순환 후보 노드임을 명확히 보여준다.

## 임시 Project Content 필터

Phase 4-1 이후에는 재귀 Tree 검증 범위를 통제하기 위해 `/Game` Package만 표시한다.

```cpp
PackageName.ToString().StartsWith(TEXT("/Game/"))
```

이 정책은 정식 필터 UI가 아니다. `/Script`, `/Engine` 같은 Engine / Script dependency가 재귀 결과를 과도하게 복잡하게 만들면, Demo Asset 간의 참조 관계를 검증하기 어렵다. 따라서 이번 단계에서는 Project Content만 남겨 Max Depth 재귀 구조를 확인한다.

Engine Content / Plugin Content 표시 옵션은 후속 필터 작업에서 별도 UI와 옵션으로 다룬다.

검증 기준으로 사용한 Before / After는 다음과 같다.

```text
Before
BP_Dummy
  /Script/NavigationSystem
  Cube
    /Script/NavigationSystem
    WorldGridMaterial
  M_Dummy
    T_Dummy_Color

After
BP_Dummy
  M_Dummy
    T_Dummy_Color
```

Before는 Asset Registry dependency를 그대로 재귀 표시한 결과다. After는 `/Game` Package만 표시해 Demo Asset 간 참조 관계만 남긴 결과다.

## Tree 갱신

분석 결과를 `TreeRootItems`에 반영한 뒤 Tree View를 갱신한다.

```cpp
TreeView->RequestTreeRefresh();
TreeView->SetItemExpansion(RootItem, true);
```

`RequestTreeRefresh`는 Tree View가 `TreeItemsSource`, `OnGenerateRow`, `OnGetChildren`을 다시 사용해 화면을 갱신하도록 요청한다.

## Content Browser Sync

Tree View의 노드를 더블 클릭하면 해당 노드가 가진 PackageName을 기준으로 Asset Registry에서 `FAssetData`를 다시 조회한다.

```text
Tree Node double click
  -> PackageName 확인
  -> GetAssetsByPackageName
  -> 유효한 FAssetData 확인
  -> SyncBrowserToAssets
```

Tree Node는 표시용 이름과 PackageName을 들고 있고, Content Browser Sync에는 `FAssetData`가 필요하다. 따라서 더블 클릭 시점에 PackageName으로 AssetData를 재조회한다.

`NAME_None` 또는 `/Script` Package처럼 Content Browser에서 선택할 수 있는 AssetData가 없는 노드는 에디터 오류 없이 무시한다. 이 정책은 분석 결과 Tree에 placeholder나 비-Content Package가 포함되더라도 UI 조작이 실패로 이어지지 않게 하기 위한 최소 방어다.

## 검증 기준

현재 Demo Asset 구성:

```text
/Content/ARI_Demo/
  BP_Dummy
  M_Dummy
  T_Dummy_Color
  Validation/
    BP_CycleA
    BP_CycleB
```

기대 참조 관계:

```text
BP_Dummy
  M_Dummy
    T_Dummy_Color
```

Dependencies 검증 기준:

- `BP_Dummy` 선택
- `Pick Selected Asset` 클릭
- `Dependencies` 모드 선택
- `Analyze` 클릭
- Tree 루트에 `BP_Dummy` 표시
- Tree 자식에 `M_Dummy` 표시
- `M_Dummy` 하위에 `T_Dummy_Color` 표시

Referencers 검증 기준:

- `M_Dummy` 선택
- `Pick Selected Asset` 클릭
- `Referencers` 모드 선택
- `Analyze` 클릭
- Tree 루트에 `M_Dummy` 표시
- Tree 자식에 `BP_Dummy` 표시
- `T_Dummy_Color` 선택 시 `T_Dummy_Color -> M_Dummy -> BP_Dummy` 표시

현재는 `/Game` Package만 표시하므로 Engine 기본 Mesh 또는 Script Package는 결과 Tree에서 제외된다.

순환 방어 검증 기준:

- `BP_CycleA` 선택
- `Pick Selected Asset` 클릭
- `Analyze` 클릭
- `BP_CycleA -> BP_CycleB -> BP_CycleA` 표시
- 마지막 `BP_CycleA` 아래로 다시 `BP_CycleB`가 확장되지 않음

Content Browser Sync 검증 기준:

- 분석 결과 Tree에서 `/Game` Asset 노드를 더블 클릭
- Content Browser가 해당 Asset 위치로 이동하고 Asset을 선택
- `M_Dummy`, `T_Dummy_Color` 같은 child node에서도 동일하게 동작
- `No dependencies found` 또는 `/Script`처럼 AssetData가 없는 노드는 오류 없이 무시

## 후속 확장

다음 단계에서는 이 흐름을 유지하되 다음 책임을 분리한다.

```text
UI Widget
= 버튼 이벤트와 Tree 표시

Analyzer
= Asset Registry 조회와 Tree Node 생성

Options
= Mode, Max Depth, 필터 조건
```

정식 필터 UI, 순환 참조 표시, Analyzer 클래스 분리는 후속 작업에서 확장한다.
