# Asset Registry Analysis Flow

이 문서는 `AssetReferenceInspector`에서 선택 Asset의 Dependencies를 Asset Registry로 조회해 Tree View에 표시하는 흐름을 정리한다.

## 현재 범위

현재 구현 범위는 Dependencies Depth 1이다.

```text
선택 Asset
-> 직접 참조하는 Package 목록 조회
-> Tree View 자식 노드로 표시
```

아직 포함하지 않는 범위:

- 재귀 Depth 처리
- Referencers 조회
- Dependencies / Referencers 모드 전환
- Engine / Plugin Content 필터
- 순환 참조 탐지
- CSV Export

## 입력 Asset

분석 입력은 `SAssetReferenceInspectorWidget`이 보관하는 `SelectedAssetData`다.

```cpp
FAssetData SelectedAssetData;
```

`Pick Selected Asset` 버튼을 누르면 Content Browser의 현재 선택 Asset 중 첫 번째 Asset을 저장한다.

Dependencies 조회는 `SelectedAssetData.PackageName`을 기준으로 수행한다.

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
-> BuildDependencyTree
-> RefreshTree
```

선택 Asset이 없으면 `No selected asset` 노드를 표시하고 종료한다. 선택 Asset이 있으면 선택 Asset을 Tree 루트로 만든다.

## Asset Registry 조회

Asset Registry 접근은 `FAssetRegistryModule`을 통해 수행한다.

```cpp
FAssetRegistryModule& AssetRegistryModule =
	FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
```

Depth 1 Dependencies는 다음 입력으로 조회한다.

```cpp
AssetRegistryModule.Get().GetDependencies(
	SelectedAssetData.PackageName,
	DependencyPackageNames);
```

반환되는 값은 Asset 객체 포인터가 아니라 PackageName 목록이다.

```text
/Game/ARI_Demo/BP_Dummy
-> /Game/ARI_Demo/M_Dummy
-> /Engine/... 또는 /Script/... Package
```

따라서 UI에 표시할 이름은 PackageName을 그대로 쓰거나, 해당 Package의 `FAssetData`를 다시 조회해 `AssetName`으로 변환한다.

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

현재 UI는 아직 Dependencies Depth 1만 사용한다. `EAssetReferenceMode`와 `FAssetReferenceAnalysisOptions`는 Phase 4의 Max Depth / Referencers 확장을 위한 기반 타입이다.

루트 노드:

```text
DisplayName = 선택 Asset 이름
PackageName = 선택 Asset PackageName
```

자식 노드:

```text
DisplayName = Dependency AssetName 또는 PackageName
PackageName = Dependency PackageName
```

조회 결과가 없으면 `No dependencies found` 노드를 자식으로 추가한다.

## Tree 갱신

분석 결과를 `TreeRootItems`에 반영한 뒤 Tree View를 갱신한다.

```cpp
TreeView->RequestTreeRefresh();
TreeView->SetItemExpansion(RootItem, true);
```

`RequestTreeRefresh`는 Tree View가 `TreeItemsSource`, `OnGenerateRow`, `OnGetChildren`을 다시 사용해 화면을 갱신하도록 요청한다.

## 검증 기준

현재 Demo Asset 구성:

```text
/Content/ARI_Demo/
  BP_Dummy
  M_Dummy
  T_Dummy_Color
```

기대 참조 관계:

```text
BP_Dummy
  M_Dummy
    T_Dummy_Color
```

Depth 1 검증 기준:

- `BP_Dummy` 선택
- `Pick Selected Asset` 클릭
- `Analyze` 클릭
- Tree 루트에 `BP_Dummy` 표시
- Tree 자식에 `M_Dummy` 포함

Engine 기본 Mesh 또는 Script Package가 함께 표시될 수 있다. 이번 단계에서는 필터링하지 않으므로 정상 범위로 본다.

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

재귀 Depth, Referencers, 필터, 순환 참조 탐지는 Analyzer 분리 이후 확장한다.
