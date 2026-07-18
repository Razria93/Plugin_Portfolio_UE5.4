# Slate Widget API Catalog

이 문서는 `AssetReferenceInspector`에서 읽게 되는 Slate 위젯과 주요 API를 섹션별로 정리한다. 사용 여부로 나누지 않고, Slate UI를 읽는 순서와 중요도를 기준으로 배치한다.

현재 프로젝트에서 사용하는 항목은 `현재 사용`에 위치만 짧게 적는다. 자세한 구현 흐름은 `Slate_UI_Basics_KR.md`에서 다룬다.

---

## 생성과 구성 문법

### SNew

Slate 위젯 인스턴스를 생성하는 기본 매크로다.

- 핵심 형태: `SNew(SWidgetType)`
- 현재 사용: 대부분의 Slate 위젯 생성

### SAssignNew

위젯을 생성하면서 `TSharedPtr` 멤버 변수에도 저장한다. 생성 이후 해당 위젯 API를 다시 호출해야 할 때 사용한다.

- 핵심 형태: `SAssignNew(TreeView, STreeView<...>)`
- 현재 사용: `TreeView` 저장 후 `RequestTreeRefresh`, `SetItemExpansion` 호출

### SLATE_BEGIN_ARGS / SLATE_END_ARGS

커스텀 Slate 위젯을 `SNew`로 생성할 때 받을 인자 목록을 정의한다.

- 현재 사용: `SAssetReferenceInspectorWidget`

### ChildSlot

`SCompoundWidget`이 가진 단일 루트 슬롯이다. 내부 UI의 최상위 위젯을 넣는다.

- 현재 사용: `SAssetReferenceInspectorWidget::Construct`

### [] Content Slot

단일 content를 받는 위젯에 자식 위젯을 넣는 문법이다.

- 예시: `SBorder [ STextBlock ]`, `SCheckBox [ STextBlock ]`
- 현재 사용: `SBorder`, `SButton`, `SCheckBox`, `STableRow`

### + Slot()

여러 자식을 받는 panel에 slot을 추가하는 문법이다.

- 예시: `+ SVerticalBox::Slot()`, `+ SUniformGridPanel::Slot(0, 0)`
- 현재 사용: UI 세로 배치, 버튼/체크박스 그리드 배치

---

## 기본 표시 위젯

### STextBlock

텍스트를 표시하는 가장 기본적인 leaf widget이다.

- 핵심 API: `Text`, `Font`
- 현재 사용: 제목, 선택 Asset, 옵션 label, Tree row text, 선택된 모드 버튼의 bold 표시

`Font`에는 고정 `FSlateFontInfo` 값을 넣을 수도 있고, 현재 상태를 읽는 getter를 바인딩할 수도 있다. `AssetReferenceInspector`는 선택된 Dependencies / Referencers 버튼의 텍스트를 bold로 표시하기 위해 `Font(this, &SAssetReferenceInspectorWidget::GetDependenciesModeFont)` 형태를 사용한다.

### SImage

이미지를 표시하는 leaf widget이다. 보통 `FSlateBrush` 또는 스타일 리소스와 함께 사용한다.

- 핵심 API: `Image`

### SSeparator

영역을 시각적으로 구분하는 선을 표시한다.

- 현재 사용: 탭 제목 영역과 조작 영역 구분

---

## 입력 위젯

### SButton

클릭 가능한 버튼 위젯이다.

- 핵심 API: `Text`, `OnClicked`
- 관련 타입: `FReply`
- 현재 사용: Pick Selected Asset, Analyze, Dependencies / Referencers 모드 버튼

단순 버튼은 `Text` 속성으로 label을 지정할 수 있다. 버튼 내부 텍스트의 font, color, padding 같은 표현을 직접 제어해야 하면 `SButton [ STextBlock ]`처럼 content slot에 별도 위젯을 넣는다. 현재 모드 버튼은 선택 상태에 따라 `STextBlock::Font`를 바꾸기 위해 이 방식을 사용한다.

### SCheckBox

체크 상태를 가진 입력 위젯이다. `[]` 안의 content는 체크박스 옆 label 또는 custom content로 표시된다.

- 핵심 API: `IsChecked`, `OnCheckStateChanged`
- 관련 타입: `ECheckBoxState`
- 현재 사용: Project Content 기본 분석 범위에 Engine Content / Plugin Content를 추가하는 `Include External Content` 옵션
- 주의: 체크박스 자체의 위치를 label 뒤로 보내려면 `SCheckBox` 내부 content 순서를 바꾸는 것이 아니라, `SHorizontalBox` 같은 부모 레이아웃에서 `STextBlock`과 `SCheckBox`를 별도 slot으로 배치한다.

### SEditableTextBox

한 줄 텍스트 입력 위젯이다.

- 핵심 API: `Text`, `OnTextCommitted`
- 관련 타입: `ETextCommit::Type`
- 현재 사용: Max Depth, Path Filter, Class Filter 입력

`Text`는 화면에 표시할 값을 읽는 바인딩이고, `OnTextCommitted`는 사용자가 입력을 확정했을 때 외부 상태를 갱신하는 callback이다.

```text
외부 상태값
= AnalysisOptions.PathFilter / AnalysisOptions.ClassFilter
= Text binding이 읽어오는 값

편집 중 임시값
= 텍스트 박스 내부 edit buffer
= 사용자가 포커스 인 상태에서 타이핑 중인 값
```

현재 필터 입력은 `OnTextChanged`를 사용하지 않는다. 따라서 사용자가 입력 중인 값은 commit 전까지 `AnalysisOptions`에 저장되지 않는다.

```text
저장값: /Script/
포커스 인 후 /Game/ 입력
-> edit buffer = /Game/
-> AnalysisOptions.PathFilter = /Script/

Enter 또는 마우스 포커스 이동
-> commit
-> AnalysisOptions.PathFilter = /Game/

Esc
-> edit buffer 폐기
-> Text binding이 저장값 /Script/를 다시 표시
```

### SComboBox

여러 선택지 중 하나를 고르는 드롭다운 위젯이다.

- 핵심 API: `OptionsSource`, `OnGenerateWidget`, `OnSelectionChanged`
- 후보 용도: Class Filter를 자유 입력이 아니라 고정 선택형으로 바꿀 때

### SSearchBox

검색 입력에 특화된 텍스트 박스다. 검색 아이콘, clear 동작, 검색 UX를 기본 제공한다.

- 핵심 API: `OnTextChanged`, `OnTextCommitted`
- 후보 용도: Path / Class Filter가 즉시 검색 UX로 바뀔 때

### SSpinBox

숫자 입력과 드래그 조절을 함께 제공한다.

- 핵심 API: `Value`, `OnValueChanged`, `OnValueCommitted`
- 후보 용도: Max Depth 입력

### SSlider

연속 범위 값을 조절하는 슬라이더다.

- 핵심 API: `Value`, `OnValueChanged`

---

## 단일 자식 컨테이너

### SBorder

테두리, 배경, padding을 제공하고 자식 하나를 담는다.

- 핵심 API: `Padding`, `BorderImage`
- 현재 사용: 탭 내부 여백, 결과 영역 테두리

### SBox

자식 하나에 크기 제약을 적용한다.

- 핵심 API: `WidthOverride`, `HeightOverride`, `MinDesiredWidth`, `MinDesiredHeight`

### SExpandableArea

펼치고 접을 수 있는 단일 영역을 만든다.

- 핵심 API: `HeaderContent`, `BodyContent`, `InitiallyCollapsed`
- 후보 용도: 필터 옵션이 늘어날 때 고급 옵션 접기

### SComboButton

버튼을 누르면 메뉴나 패널을 여는 위젯이다.

- 핵심 API: `ButtonContent`, `MenuContent`, `OnGetMenuContent`

### SMenuAnchor

특정 위치에 팝업 메뉴나 floating content를 붙이는 위젯이다.

- 핵심 API: `MenuContent`, `OnGetMenuContent`

---

## 다중 자식 레이아웃

### SVerticalBox

자식을 위에서 아래로 배치한다.

- 핵심 Slot 옵션: `AutoHeight`, `FillHeight`, `Padding`
- 현재 사용: 탭 내부 전체 UI 세로 배치

### SHorizontalBox

자식을 왼쪽에서 오른쪽으로 배치한다.

- 핵심 Slot 옵션: `AutoWidth`, `FillWidth`, `Padding`
- 후보 용도: label과 입력 위젯을 한 줄에 배치

### SUniformGridPanel

같은 크기의 grid cell에 자식을 배치한다.

- 핵심 API: `SlotPadding`
- 핵심 Slot: `SUniformGridPanel::Slot(Column, Row)`
- 현재 사용: 버튼 2개, 체크박스 2개를 같은 폭으로 배치

### SGridPanel

행/열 기반 grid에 자식을 배치한다. 각 cell 크기를 더 세밀하게 다룰 수 있다.

- 핵심 API: `FillColumn`
- 핵심 Slot: `SGridPanel::Slot(Column, Row)`
- 현재 사용: Analysis Options의 Max Depth, Path Filter, Class Filter label/input 정렬

`FillColumn(ColumnIndex, Weight)`는 지정한 column이 남는 가로 공간을 어떤 비율로 가져갈지 정한다. 현재 UI는 label column은 내용 크기에 가깝게 두고, 입력 column만 `FillColumn(1, 1.0f)`로 늘려 텍스트 박스가 창 폭을 따라 확장되도록 구성한다.

### SOverlay

여러 자식을 같은 영역에 겹쳐 배치한다.

- 후보 용도: Tree 영역 위에 loading/empty overlay 표시

### SScrollBox

스크롤 가능한 영역을 만든다.

- 후보 용도: 필터 옵션이나 긴 설명 영역이 늘어날 때

### SSplitter

사용자가 크기를 조절할 수 있는 분할 영역을 만든다.

- 후보 용도: 결과 Tree와 상세 패널을 좌우로 나눌 때

### SWrapBox

자식을 가로로 배치하다가 공간이 부족하면 다음 줄로 넘긴다.

- 후보 용도: filter chip, tag 목록 표시

---

## 스타일과 표현값

### FAppStyle

Unreal Editor가 제공하는 Slate style set에 접근하는 API다. Editor 기본 font, color, brush 같은 스타일 리소스를 가져올 때 사용한다.

- 핵심 API: `FAppStyle::GetFontStyle`
- 현재 사용: section header와 선택된 mode button text에 `NormalFontBold`, `NormalFont` 적용

현재 UI는 별도 custom style set을 만들지 않고, Editor 기본 스타일을 사용한다. 따라서 `Mode`, `Filters`, `Content Scope` 같은 section header와 selected mode text는 Unreal Editor와 시각적으로 어긋나지 않는다.

---

## 반복 표시 / 테이블 계열

### STreeView

계층 데이터를 row 형태로 표시하는 table 계열 위젯이다.

- 핵심 API: `TreeItemsSource`, `OnGenerateRow`, `OnGetChildren`, `RequestTreeRefresh`, `SetItemExpansion`
- 현재 사용: Asset Dependencies / Referencers Tree 표시

### SListView

평면 배열 데이터를 row 목록으로 표시한다.

- 핵심 API: `ListItemsSource`, `OnGenerateRow`, `RequestListRefresh`

### STileView

아이템을 타일/grid 형태로 반복 표시한다.

- 핵심 API: `ListItemsSource`, `OnGenerateTile`

### STableRow

`STreeView`, `SListView`, `STileView` 같은 table 계열 위젯에서 아이템 하나를 표시하는 row 위젯이다.

- 핵심 형태: `SNew(STableRow<ItemType>, OwnerTable)`
- 현재 사용: Tree node 하나를 `STextBlock` row로 표시

### SHeaderRow

테이블 컬럼 헤더를 구성한다.

- 후보 용도: 결과를 이름, 클래스, 크기, Depth 같은 컬럼으로 표시할 때

---

## Editor Window / Docking 계열

### SDockTab

Unreal Editor의 dockable tab content를 담는 위젯이다.

- 현재 사용: `AssetReferenceInspector` Nomad Tab 내부 content

### FGlobalTabmanager

Editor 전역 tab spawner를 등록하고 tab을 여는 manager다.

- 핵심 API: `RegisterNomadTabSpawner`, `UnregisterNomadTabSpawner`, `TryInvokeTab`
- 현재 사용: Asset Reference Inspector 탭 등록과 열기

### RegisterNomadTabSpawner

독립적으로 dock/float 가능한 Nomad Tab 생성 규칙을 등록한다.

- 핵심 인자: Tab ID, `FOnSpawnTab`
- 현재 사용: `AssetReferenceInspector` 탭 생성기 등록

### FOnSpawnTab

TabManager가 탭을 생성해야 할 때 호출하는 delegate 타입이다.

- 현재 사용: `FAssetReferenceInspectorModule::OnSpawnPluginTab` 바인딩

---

## 이벤트와 콜백

### OnClicked

`SButton` 클릭 시 호출되는 callback이다.

- 필요 반환값: `FReply`
- 현재 사용: Pick Selected Asset, Analyze, Mode 전환

### FReply

Slate 입력 이벤트 처리 결과를 나타낸다.

- 주요 값: `FReply::Handled()`, `FReply::Unhandled()`
- 현재 사용: 버튼 클릭 처리 결과 반환

### OnTextCommitted

`SEditableTextBox` 입력이 확정될 때 호출된다.

- 관련 타입: `ETextCommit::Type`
- 현재 사용: Path / Class Filter 값 저장

### ETextCommit::Type

텍스트 입력 commit 원인을 나타낸다.

- 주요 값: `OnEnter`, `OnUserMovedFocus`, `OnCleared`
- 현재 사용: `OnCleared`는 상태 덮어쓰기 방지를 위해 무시

주요 commit 원인은 다음처럼 해석한다.

```text
Default
= 포커스 손실 등 암시적 commit

OnEnter
= Enter 키로 commit

OnUserMovedFocus
= 사용자가 다른 UI로 포커스를 옮겨 commit

OnCleared
= Escape 또는 명시적 focus clear 계열
```

Path Filter 검증 중 Enter를 누르면 `OnEnter`가 먼저 호출되고, 같은 텍스트로 `OnCleared`가 뒤따라 호출되는 사례를 확인했다.

```text
CommitType=1, Text='/Game/'
CommitType=3, Text='/Game/'
```

이는 Enter가 `OnCleared`로 분류된다는 뜻이 아니다. Enter는 먼저 `OnEnter`로 commit된다. 이후 `SEditableTextBox`가 commit 후 키보드 포커스를 명시적으로 clear하면, focus lost 처리 경로에서 `EFocusCause::Cleared`가 `ETextCommit::OnCleared`로 다시 전달될 수 있다.

```text
Enter
-> OnTextCommitted(..., OnEnter)
-> ClearKeyboardFocus(EFocusCause::Cleared)
-> focus lost
-> OnTextCommitted(..., OnCleared)
```

따라서 `OnCleared`는 텍스트 내용이 비었다는 뜻이 아니라, 키보드 포커스가 clear 원인으로 빠졌다는 뜻으로 해석한다. 현재 필터 입력은 `OnCleared`를 무시하고, `OnEnter` 또는 `OnUserMovedFocus` 같은 정상 commit에서만 `AnalysisOptions` 값을 갱신한다.

### IsChecked

`SCheckBox`가 현재 표시할 체크 상태를 가져오는 바인딩이다.

- 반환 타입: `ECheckBoxState`
- 현재 사용: `Include External Content` 옵션 상태 표시

### OnCheckStateChanged

`SCheckBox` 상태가 바뀔 때 호출된다.

- 인자 타입: `ECheckBoxState`
- 현재 사용: `Include External Content` 옵션 상태 저장

### OnGenerateRow

table 계열 위젯이 데이터 아이템 하나를 row 위젯으로 바꿀 때 호출한다.

- 반환 타입: `TSharedRef<ITableRow>`
- 현재 사용: `FAssetReferenceTreeNode`를 `STableRow`로 변환

### OnGetChildren

`STreeView`가 특정 item의 자식 목록을 요청할 때 호출한다.

- 현재 사용: `FAssetReferenceTreeNode::Children` 제공

### OnMouseButtonDoubleClick

table row item을 더블 클릭했을 때 호출된다.

- 현재 사용: Tree node 더블 클릭 시 Content Browser Sync

---

## 데이터 갱신 API

### TreeItemsSource

`STreeView`가 루트 아이템 배열을 읽을 위치를 지정한다.

- 현재 사용: `TreeRootItems`

### RequestTreeRefresh

Tree 데이터가 바뀐 뒤 표시 갱신을 요청한다.

- 현재 사용: Analyze 이후 Tree 결과 갱신

### SetItemExpansion

특정 Tree item의 펼침 상태를 지정한다.

- 현재 사용: 분석 결과 Tree 자동 펼침
