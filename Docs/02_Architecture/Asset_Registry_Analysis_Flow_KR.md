# Asset Registry Analysis Flow

이 문서는 `AssetReferenceInspector`에서 선택 Asset의 Dependencies / Referencers 관계를 Asset Registry로 조회해 Tree View에 표시하는 흐름을 정리한다.

## 현재 범위

현재 구현 범위는 Dependencies / Referencers 모드 전환, Max Depth 기반 재귀 Tree 생성, Path / Class / Content Scope 필터, 순환 후보 노드 표시, Asset 디스크 크기 표시, Unused Candidate 전체 스캔이다.

```text
선택 Asset
-> 현재 모드에 맞는 관련 Package 목록 조회
-> Tree View 자식 노드로 표시
```

아직 포함하지 않는 범위:

- CSV Export
- Unused Candidate 표시

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
	bool bIsCircular;
	FString ClassName;
	int64 SizeBytes;
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
	FString PathFilter;
	FString ClassFilter;
};
```

`FAssetReferenceAnalysisOptions::MaxDepth`는 재귀 Tree 생성의 깊이 제한으로 사용한다.

`FAssetReferenceAnalysisOptions::PathFilter`는 표시할 PackageName 경로 prefix를 결정한다. 기본값은 `/Game/`이며, 입력값이 비어 있으면 Engine / Script / Plugin Package를 포함한 전체 관계를 표시할 수 있다.

`FAssetReferenceAnalysisOptions::ClassFilter`는 `FAssetData::AssetClassPath`를 기준으로 표시할 Asset Class를 제한한다. 입력값이 비어 있으면 모든 Asset Class를 통과시킨다.

```text
Depth 0 = 선택 Asset root
Depth 1 = root의 직접 관련 Package
Depth 2 = Depth 1 노드의 관련 Package
```

루트 노드:

```text
DisplayName = 선택 Asset 이름
PackageName = 선택 Asset PackageName
ClassName = 선택 Asset Class 이름
SizeBytes = 선택 Asset package 파일 크기 합산값
```

자식 노드:

```text
DisplayName = 관련 AssetName 또는 PackageName
PackageName = 관련 PackageName
ClassName = 관련 Asset Class 이름. AssetData를 찾을 수 없으면 비어 있음
SizeBytes = 관련 Asset package 파일 크기 합산값. 파일 경로를 찾을 수 없으면 0
```

조회 결과가 없으면 현재 모드에 따라 `No dependencies found` 또는 `No referencers found` 노드를 자식으로 추가한다.

## Asset 디스크 크기 표시

Phase 6-2 이후에는 PackageName 기준으로 local package filename을 찾아 Asset 디스크 크기를 표시한다. 크기는 runtime memory size나 cooked size가 아니라 editor project의 파일 크기 기준 추정값이다.

```text
PackageName
-> TryConvertLongPackageNameToFilename
-> .uasset 또는 .umap file size
-> 같은 base의 .uexp, .ubulk가 있으면 합산
```

1차 기준은 `.uasset`이다. Asset package 파일을 찾지 못하면 `.umap`도 확인한다. 이후 같은 base filename의 `.uexp`, `.ubulk`가 존재하면 파일 크기를 합산한다.

파일 크기를 찾을 수 없는 경우 `SizeBytes`는 0으로 둔다. 따라서 `/Script` Package, placeholder node, 아직 디스크에 저장되지 않은 Package는 Tree row에 size suffix를 표시하지 않는다.

Tree row 표시 순서는 다음과 같다.

```text
DisplayName [ClassName] (Size) [Circular] [Unused Candidate]
```

`ClassName`, `Size`, `[Circular]`, `[Unused Candidate]`는 조건부 suffix다. 예를 들어 AssetData와 파일 크기를 찾을 수 있는 순환 후보 노드는 `BP_CycleA [Blueprint] (12.34 KB) [Circular]`처럼 표시된다. Unused Candidate 전체 스캔 결과의 후보 노드는 `T_Unused_Color [Texture2D] (10.13 KB) [Unused Candidate]`처럼 표시된다.

## Unused Candidate 전체 스캔

Phase 6-3 이후에는 선택 Asset 관계 Tree와 별도로 `/Game` 전체 Asset을 대상으로 Unused Candidate 후보를 스캔한다.

Dependencies / Referencers Tree는 선택 Asset 기준 관계 분석이다. Dependencies child node는 부모 Asset에게 이미 참조받고 있고, Referencers child node는 선택 Asset을 참조하는 Asset이다. 따라서 관계 Tree의 child node에 Unused Candidate를 섞으면 의미가 흐려진다. Unused Candidate는 별도 스캔 버튼으로 전체 Project Content를 조회해 표시한다.

```text
Scan Unused Candidates
-> AssetRegistry.GetAssetsByPath("/Game", recursive=true)
-> Path Filter / Class Filter 적용
-> Map, Redirector, 외부 Content 제외
-> GetReferencers(PackageName)
-> Referencer 수 0이면 후보 node 추가
```

후보 결과는 `Unused Candidates` root node 아래에 표시한다. 후보가 없으면 `No unused candidates found` placeholder를 추가한다.

판정 기준:

- `/Game` 아래 Project Content Asset만 검사한다.
- `GetReferencers(PackageName)` 결과가 0개이면 후보로 본다.
- `World`, `ObjectRedirector`는 제외한다.
- `/Script`, `/Engine`, Plugin Content, placeholder node는 후보로 표시하지 않는다.
- Path Filter와 Class Filter를 통과한 Asset만 검사한다.

이 기능은 삭제 가능 판정이 아니다. Soft Reference, Asset Manager, 동적 로딩, 코드 기반 경로 로딩은 Asset Registry의 referencer 목록만으로 완전히 확인할 수 없다. 따라서 UI와 문서에서는 `Unused Asset`이 아니라 `Unused Candidate`로만 표현한다.

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

이 정책은 순환 참조 정보를 숨기지 않기 위한 것이다. 순환 후보 노드는 `bIsCircular` 상태를 가진 채 Tree에 남기고, row text에는 `[Circular]` suffix를 붙여 순환 후보임을 표시한다.

## Path Filter

Phase 5-1 이후에는 재귀 Tree 검증 범위를 통제하기 위해 사용자가 입력한 Path Filter 값을 기준으로 PackageName을 표시한다.

```cpp
PackageName.ToString().StartsWith(AnalysisOptions.PathFilter)
```

기본값은 `/Game/`이다. `/Script`, `/Engine` 같은 Engine / Script dependency가 재귀 결과를 과도하게 복잡하게 만들면 Demo Asset 간의 참조 관계를 검증하기 어렵기 때문에, 기본 상태에서는 Project Content만 남긴다.

Path Filter 입력값을 비우면 모든 PackageName을 통과시킨다. 이 상태는 `/Script` Package나 Engine 기본 Asset이 어떻게 조회되는지 디버깅할 때 사용한다.

Engine Content / Plugin Content 표시 옵션은 Project Content 기본 분석 범위를 외부 Content로 확장하는 옵션이다. 기본 상태에서는 `/Game/` 중심으로 표시하고, 체크 시 `/Engine/` 또는 Content를 가진 활성 플러그인의 mount path 아래 PackageName도 표시한다.

### Plugin Content 판정 정책

Engine Content는 `/Engine/` root로 판정할 수 있다. 반면 Plugin Content는 플러그인마다 Content mount root가 다르므로 `/Plugin/` 같은 고정 prefix로 판정하지 않는다.

`AssetReferenceInspector`는 `IPluginManager::Get().GetEnabledPluginsWithContent()`로 Content를 가진 활성 플러그인 목록을 가져오고, 각 플러그인의 `GetMountedAssetPath()`와 PackageName prefix를 비교한다.

`GetMountedAssetPath()`는 플러그인 mount root를 trailing slash가 붙은 형태로 반환할 수 있으므로, 비교 전 끝의 `/`를 제거해 root path를 정규화한다.

```cpp
MountedAssetPath.RemoveFromEnd(TEXT("/"));
PackageNameString.StartsWith(MountedAssetPath + TEXT("/"))
```

`PackageNameString == MountedAssetPath` 조건은 사용하지 않는다. 현재 분석 단위는 실제 Asset Package이며, 플러그인 mount root 자체는 일반적인 Asset PackageName이 아니기 때문이다. 나중에 플러그인 root를 그룹 노드로 표시해야 한다면 Asset Package 판정 함수가 아니라 별도 node type으로 처리한다.

Path Filter 적용 후 실제 Asset Registry 조회 결과는 있지만 표시 가능한 자식이 모두 걸러질 수 있다. 이 경우에도 Tree가 빈 것처럼 보이지 않도록 현재 모드에 맞는 placeholder 노드를 추가한다.

```text
Dependencies mode = No dependencies found
Referencers mode = No referencers found
```

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

## Asset Class Filter

Phase 5-2 이후에는 Asset Class Filter 입력값을 기준으로 표시할 Asset 종류를 제한한다.

Class Filter는 PackageName 자체가 아니라 PackageName으로 다시 조회한 `FAssetData::AssetClassPath`를 사용한다.

```text
PackageName
-> GetAssetsByPackageName
-> FAssetData
-> AssetClassPath
-> ClassPath / ClassName 비교
```

현재 판정 기준은 다음과 같다.

```cpp
ClassPath.Contains(ClassFilter, ESearchCase::IgnoreCase)
|| ClassName.Contains(ClassFilter, ESearchCase::IgnoreCase)
```

따라서 `Material`, `Texture2D`, `Blueprint`처럼 Class 이름 일부를 입력해도 대소문자와 관계없이 매칭된다.

Path Filter, Class Filter, Engine Content 옵션, Plugin Content 옵션은 `FAssetReferenceFilter::ShouldPassRelationFilters`에서 함께 적용한다.

```text
FAssetReferenceFilter::ShouldPassRelationFilters
-> DoesContentSourcePassFilter
-> DoesPathPassFilter
-> DoesAssetClassPassFilter
```

필터 조건 중 하나라도 통과하지 못하면 해당 Package는 Tree에 추가하지 않는다. `SAssetReferenceInspectorWidget`은 Tree 생성 중 filter API만 호출하고, 세부 predicate는 `Private/Analysis/AssetReferenceFilter`가 담당한다.

### 현재 필터 정책

현재 필터는 노드를 Tree에 추가하기 전에 적용한다. 즉 어떤 중간 노드가 필터를 통과하지 못하면, 그 노드의 하위 관계도 더 이상 탐색하지 않는다.

```text
Root
  A
  B
    C
```

위 구조에서 `C`는 Class Filter에 맞지만 `B`가 필터를 통과하지 못하면, 현재 구현은 `C`까지 내려가지 않는다.

현재 정책:

```text
필터를 통과한 노드만 표시하고,
통과한 노드만 계속 재귀 탐색한다.
```

이 정책은 MVP 단계에서 결과를 단순하게 유지하기 위한 선택이다.

향후 필터 UX를 확장한다면 `Context-Preserving Filter Mode`를 고려한다. 이 방식은 하위에 매칭되는 노드가 있으면 부모 경로를 유지하고, 직접 매칭되지 않은 부모 노드는 흐리게 표시한다.

```text
Root
  B   // dimmed, context only
    C // matched
```

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

기본 Path Filter는 `/Game/`이므로 Engine 기본 Mesh 또는 Script Package는 결과 Tree에서 제외된다.

Path Filter 검증 기준:

- 기본값 `/Game/` 상태에서 `BP_Dummy -> M_Dummy -> T_Dummy_Color`만 표시
- Path Filter를 비운 상태에서 `/Script`, Engine 기본 Asset 등 비-Project Package도 표시
- Path Filter에 매칭되는 자식이 없으면 현재 모드에 맞는 placeholder 표시
- Path Filter 변경 후 `Analyze`를 다시 클릭하면 변경된 조건으로 Tree 갱신

순환 방어 검증 기준:

- `BP_CycleA` 선택
- `Pick Selected Asset` 클릭
- `Analyze` 클릭
- `BP_CycleA -> BP_CycleB -> BP_CycleA` 표시
- 마지막 `BP_CycleA` row에 `[Circular]` 표시
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

Analyzer 클래스 분리와 CSV Export는 후속 작업에서 확장한다. Path / Class / Engine / Plugin Content 필터 UI, 순환 후보 row 표시, Asset 디스크 크기 표시, Unused Candidate 전체 스캔은 현재 `SAssetReferenceInspectorWidget`에서 직접 관리한다.
