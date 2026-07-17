# Code Organization

이 문서는 `AssetReferenceInspector` 플러그인의 C++ 파일 배치, include 순서, 헤더 섹션 구성 기준을 정리한다.

목표는 Public API를 작게 유지하고, Unreal Build Tool 의존성 범위와 코드 구조가 어긋나지 않게 하는 것이다.

## Public / Private 배치

`Public`에는 외부 모듈이 실제로 include해야 하는 타입만 둔다.

현재 기준:

```text
Public/
  AssetReferenceInspectorModule.h
```

`Private`에는 플러그인 내부 구현 세부 타입을 둔다.

현재 기준:

```text
Private/
  AssetReferenceInspectorCommands.h
  AssetReferenceInspectorCommands.cpp
  AssetReferenceInspectorModule.cpp
  UI/SAssetReferenceInspectorWidget.h
  UI/SAssetReferenceInspectorWidget.cpp
```

기본 판단:

- 외부 모듈이 직접 include할 이유가 없으면 `Private`에 둔다.
- Command, Menu, ToolMenus, Slate 구현 세부 타입은 기본적으로 `Private`에 둔다.
- Public 헤더에 노출된 타입이 다른 Unreal 모듈 타입을 필요로 하면 해당 의존성은 `PublicDependencyModuleNames`로 올린다.
- 구현 파일에서만 쓰는 의존성은 `PrivateDependencyModuleNames`에 둔다.

예를 들어 `FAssetReferenceInspectorCommands`는 메뉴 등록을 위한 내부 타입이다. 외부 모듈이 사용할 API가 아니며 Slate / Commands 관련 타입을 포함하므로 `Private`에 둔다.

`SAssetReferenceInspectorWidget`도 플러그인 내부 탭 UI 구현체이므로 `Private/UI`에 둔다. 외부 모듈이 직접 include할 Public API가 아니라 `AssetReferenceInspector` 탭 내부에서만 사용한다.

## Include 순서

`.cpp` 파일의 include는 다음 순서로 그룹화한다.

```cpp
#include "ThisFile.h"

#include "SameModulePrivateHeader.h"

#include "Framework/..."
#include "Modules/..."

#include "ToolMenus.h"

#include "Widgets/..."
```

기준:

- 첫 include는 항상 자기 헤더로 둔다.
- 같은 모듈의 내부 헤더를 그 다음에 둔다.
- Unreal Framework와 Modules 계열을 다음 그룹으로 둔다.
- ToolMenus처럼 독립적인 에디터 시스템 헤더를 별도 그룹으로 둔다.
- Slate / Widgets 계열은 마지막 그룹에 둔다.

include 그룹은 과하게 세분화하지 않는다. 단, 다른 성격의 시스템이 섞여 읽기 어려워질 때는 빈 줄로 구분한다.

## 헤더 섹션 구성

헤더에서는 외부 호출 API, 내부 helper, 상태 필드를 구분한다.

기본 순서:

```cpp
class FExampleModule : public IModuleInterface
{
public:
	// IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	// Feature Area
	void DoSomething();

private:
	// State
	TSharedPtr<class FSomeType> SomeState;
};
```

기준:

- `public`에는 엔진 생명주기 또는 외부 호출이 필요한 API만 둔다.
- override 함수는 해당 섹션의 가장 위에 둔다.
- `private` helper 함수는 기능 영역별로 묶는다.
- 상태 필드는 함수보다 아래에 둔다.
- 필드가 늘어나면 기능 그룹 주석으로 묶는다.
- 단순한 클래스에는 과한 주석을 넣지 않고, 확장 지점이 보일 때만 짧은 섹션 주석을 사용한다.

## 현재 모듈 기준

`FAssetReferenceInspectorModule`은 모듈 생명주기와 에디터 확장 등록을 담당한다.

현재 헤더 구성은 다음 의도를 따른다.

```text
public
- StartupModule
- ShutdownModule

private Tab
- OnSpawnPluginTab
- OpenPluginWindow

private Menu
- RegisterMenus

private Commands
- PluginCommands
```

이 구조는 다음 작업인 `SAssetReferenceInspectorWidget` 분리 이후에도 유지한다. 모듈은 탭과 메뉴 등록의 진입점으로 남기고, 실제 UI 구성은 별도 Slate 위젯으로 옮긴다.

`SAssetReferenceInspectorWidget` 분리 이후에도 `FAssetReferenceInspectorModule`은 탭 생성 시 UI 루트 위젯을 배치하는 역할만 맡는다. 버튼, 선택 Asset 표시, Tree View 같은 실제 화면 구성은 `Private/UI` 아래 Slate Widget에서 확장한다.

`SAssetReferenceInspectorWidget`은 UI 이벤트, 분석 데이터 생성, Tree View 표시가 한 클래스 안에 공존한다. 이 경우 `private` helper를 하나의 `Tree data` 섹션에 모두 넣지 않고 다음 기준으로 나눈다.

```text
private Button callbacks
- OnPickSelectedAssetClicked
- OnAnalyzeClicked

private Filter callbacks
- OnPathFilterTextCommitted

private Tree callbacks
- OnTreeNodeDoubleClicked

private UI text
- GetSelectedAssetText
- GetCurrentModeText
- GetPathFilterText

private Analysis
- BuildDependencyTree
- BuildDependencyChildren
- CreateDependencyNode
- ShouldIncludeDependencyPackage

private Tree view
- RefreshTree
- ExpandTreeItems
- OnGenerateTreeRow
- OnGetTreeChildren

private Content Browser
- TrySyncContentBrowserToPackage

private Analysis state
- AnalysisOptions
- SelectedAssetData

private Tree view state
- TreeRootItems
- TreeView
```

기준:

- 버튼 클릭처럼 `SButton::OnClicked`에서 직접 호출되는 함수는 `Button callbacks`에 둔다.
- 입력값 commit처럼 필터 UI에서 직접 호출되는 함수는 `Filter callbacks`에 둔다.
- Tree item 더블 클릭처럼 `STreeView` 상호작용에서 직접 호출되는 함수는 `Tree callbacks`에 둔다.
- Slate text binding 함수는 `UI text`에 둔다.
- Asset Registry 조회, 노드 생성, 필터 판단은 `Analysis`에 둔다.
- `STreeView` 갱신, expand, row 생성, children 제공은 `Tree view`에 둔다.
- Content Browser 선택 동기화처럼 에디터 UI 외부 시스템과 연결되는 helper는 `Content Browser`에 둔다.
- 분석 입력과 옵션은 `Analysis state`, Tree가 참조하는 데이터와 Slate 위젯 포인터는 `Tree view state`에 둔다.
- 더 세부적인 `Dependency Query`, `Node Factory`, `Filter` 섹션은 실제 함수 수가 늘어난 뒤에만 추가한다.
