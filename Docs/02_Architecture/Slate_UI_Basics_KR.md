# Slate UI Basics

이 문서는 `AssetReferenceInspector` Editor Plugin에서 사용하는 Slate UI 구조와 주요 Slate API 사용 기준을 정리한다.

Slate는 Unreal Engine의 C++ 기반 UI 프레임워크다. Editor Plugin UI는 UMG보다 Slate를 직접 사용하는 경우가 많으며, 이 플러그인의 탭 내부 UI도 Slate 위젯으로 구성한다.

---

## 문서 범위

이 문서는 파일명을 유지하되 내부 책임을 두 영역으로 나눈다.

```text
UI 구조
= SAssetReferenceInspectorWidget이 화면을 어떻게 구성하는지 설명한다.

Slate API 사용 노트
= SNew, SAssignNew, Text binding, OnClicked, OnTextCommitted 같은 API 사용 방식과 주의사항을 설명한다.
```

위젯과 API의 목록형 참조는 `Slate_Widget_API_Catalog_KR.md`에서 다룬다. 이 문서는 현재 플러그인 UI 구조와 핵심 사용 흐름을 설명하는 데 집중한다.

---

## UI 구조

### 기본 위젯

Slate 위젯 클래스는 보통 `S` 접두사를 사용한다.

```text
SButton = 버튼
STextBlock = 텍스트
SBorder = 테두리 / 배경 / 패딩 컨테이너
SBox = 크기 제약 컨테이너
SCheckBox = 체크 상태 입력 위젯
SVerticalBox = 세로 배치 컨테이너
SHorizontalBox = 가로 배치 컨테이너
SUniformGridPanel = 같은 크기 셀 그리드
SScrollBox = 스크롤 가능한 컨테이너
SSeparator = 구분선
STreeView = 트리 표시 위젯
SDockTab = 에디터 도킹 탭
```

### Slate 위젯 분류와 구조

Slate 위젯을 이해할 때는 먼저 "이 위젯이 자식을 받는가", "자식을 받는다면 하나인가 여러 개인가", "직접 받는가 callback으로 생성하는가"를 구분한다.

`SNew(SomeWidget)`은 위젯 인스턴스를 생성한다. 그 뒤의 `[]`는 해당 위젯이 제공하는 content slot에 자식 위젯을 넣는 문법이고, `+ SomePanel::Slot()`은 여러 자식을 받는 panel에 slot을 하나 추가하는 문법이다.

#### Leaf Widget

Leaf Widget은 자식 위젯을 받지 않고 자기 자신을 그리는 위젯이다.

```text
STextBlock
= 텍스트를 표시하는 leaf widget
```

보통 leaf widget은 다음처럼 속성만 지정한다.

```cpp
SNew(STextBlock)
	.Text(FText::FromString(TEXT("Asset Reference Inspector")))
```

#### Single Child / Compound Widget

Single Child 또는 Compound 계열 위젯은 하나의 content slot을 가진다.

```text
SBorder
= 테두리, 배경, padding을 제공하고 자식 하나를 담는 컨테이너

SCheckBox
= 체크 상태를 표시하고, content slot에 label 또는 custom content를 담는 위젯

SBox
= 크기 제약을 적용하고 자식 하나를 담는 컨테이너
```

예를 들어 `SBorder [ STextBlock ]`은 Border 내부 content slot에 TextBlock을 넣는 구조다.

```cpp
SNew(SBorder)
[
	SNew(STextBlock)
]
```

`SCheckBox [ STextBlock ]`도 같은 방식으로 이해한다. 이 경우 `STextBlock`은 체크박스 오른쪽에 붙는 label/content로 표시된다.

```cpp
SNew(SCheckBox)
[
	SNew(STextBlock)
		.Text(FText::FromString(TEXT("Engine Content")))
]
```

#### Panel / Multi Slot Widget

Panel 계열 위젯은 여러 자식을 slot collection으로 받는다.

```text
SVerticalBox
= 자식을 세로 방향 slot으로 배치

SHorizontalBox
= 자식을 가로 방향 slot으로 배치

SUniformGridPanel
= 자식을 같은 크기의 grid cell에 배치

SScrollBox
= 자식을 스크롤 가능한 slot으로 배치
```

Panel 계열은 `[]` 하나에 자식을 넣는 대신 `+ Slot()`으로 자식 slot을 계속 추가한다.

```cpp
SNew(SHorizontalBox)

+ SHorizontalBox::Slot()
[
	SNew(STextBlock)
]

+ SHorizontalBox::Slot()
[
	SNew(SCheckBox)
]
```

`SUniformGridPanel`은 slot에 좌표를 지정한다.

```cpp
SNew(SUniformGridPanel)

+ SUniformGridPanel::Slot(0, 0)
[
	SNew(SButton)
]

+ SUniformGridPanel::Slot(1, 0)
[
	SNew(SButton)
]
```

각 slot은 `Padding`, `AutoHeight`, `FillWidth`, `HAlign`, `VAlign` 같은 배치 규칙을 가질 수 있다.

#### Table / Generated Row Widget

Table 계열 위젯은 데이터를 반복 표시한다. 자식 위젯을 직접 나열하기보다, 데이터 소스와 callback을 연결해 필요한 row를 생성한다.

```text
STreeView
= 계층 데이터 표시

SListView
= 목록 데이터 표시

STileView
= tile 형태 데이터 표시
```

`STreeView`는 `TreeItemsSource`, `OnGenerateRow`, `OnGetChildren` 같은 callback을 통해 "데이터 모델 -> Row 위젯" 변환을 수행한다.

```text
Tree data node
-> OnGenerateRow
-> STableRow
```

#### Dock / Editor Container Widget

Editor 확장에서는 탭이나 창을 담당하는 컨테이너도 사용한다.

```text
SDockTab
= Editor tab 하나를 나타내는 Dock widget
```

`AssetReferenceInspector`는 `SDockTab` 안에 `SAssetReferenceInspectorWidget`을 넣고, 그 내부에서 실제 도구 UI를 구성한다.

### SCompoundWidget

`SAssetReferenceInspectorWidget`은 `SCompoundWidget`을 상속한다.

`SCompoundWidget`은 여러 Slate 위젯을 조합해서 하나의 커스텀 위젯을 만들 때 사용한다.

```text
SCompoundWidget
= 커스텀 Slate 위젯의 기본 형태

ChildSlot
= 커스텀 위젯 내부에 들어갈 루트 위젯 한 칸

Construct
= SNew로 위젯이 생성될 때 내부 UI 트리를 구성하는 함수

SLATE_BEGIN_ARGS / SLATE_END_ARGS
= SNew로 생성할 때 외부에서 받을 옵션 목록
```

`SCompoundWidget`은 `ChildSlot`이라는 하나의 루트 슬롯을 가진다. 여러 위젯을 직접 나열하려면 `ChildSlot`에 `SVerticalBox`, `SBorder` 같은 컨테이너를 하나 넣고, 그 안에 다시 자식 위젯들을 배치한다.

```cpp
void SAssetReferenceInspectorWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(STextBlock)
		]
	];
}
```

위 예시는 다음 구조를 만든다.

```text
SAssetReferenceInspectorWidget
  ChildSlot
    SBorder
      STextBlock
```

현재 `SLATE_BEGIN_ARGS` / `SLATE_END_ARGS`는 비어 있다. 즉, `SNew(SAssetReferenceInspectorWidget)`으로 생성할 때 외부에서 넘길 설정값이 아직 없다는 뜻이다. 이후 초기 선택 Asset, 표시 옵션, 콜백 등을 외부에서 넘겨야 할 때 Slate 인자를 추가한다.

### Construct

`Construct`는 위젯 인스턴스가 생성될 때 한 번 호출되는 초기 구성 함수다. UI가 갱신될 때마다 `Construct`가 다시 위에서 아래로 실행되는 것은 아니다.

`Construct`에서 주로 하는 일:

```text
- UI 트리 생성
- Slot과 레이아웃 구성
- 이벤트 콜백 바인딩
- 동적 속성 바인딩
```

### Slot과 배치

Slate 레이아웃 컨테이너는 자식 위젯을 `Slot` 단위로 받는다.

`SVerticalBox`는 위에서 아래로 자식을 배치한다.

```cpp
SNew(SVerticalBox)

+ SVerticalBox::Slot()
	.AutoHeight()
	[
		SNew(STextBlock)
	]

+ SVerticalBox::Slot()
	.FillHeight(1.0f)
	[
		SNew(SBorder)
	]
```

자주 쓰는 높이 옵션:

```text
AutoHeight()
= 자식 위젯이 필요한 높이만 차지한다.

FillHeight(Value)
= AutoHeight 슬롯들이 차지하고 남은 높이를 FillHeight 슬롯끼리 Value 비율로 나눠 가진다.
```

현재 UI Shell에서는 상단 텍스트와 버튼 영역은 `AutoHeight()`를 사용하고, 결과 영역은 `FillHeight(1.0f)`를 사용한다. 결과 영역이 탭의 남은 세로 공간을 크게 차지해야 하기 때문이다.

`SHorizontalBox`에서는 같은 개념으로 `AutoWidth()`와 `FillWidth(Value)`를 사용한다.

`Slot`은 부모 컨테이너가 자식 위젯을 배치하기 위해 제공하는 자리와 배치 규칙이다. `STreeView`의 `Row`와는 구분한다.

```text
Slot
= `SVerticalBox`, `SHorizontalBox` 같은 레이아웃 컨테이너 안에서 자식 위젯을 놓는 자리

Row
= `SListView`, `STreeView` 같은 반복 표시 위젯에서 데이터 아이템 하나를 표현하는 화면 줄
```

### Slot 옵션

`Padding`은 슬롯 또는 위젯 내부 여백을 지정한다.

```cpp
.Padding(0.0f, 0.0f, 0.0f, 8.0f)
```

순서는 다음과 같다.

```text
Left, Top, Right, Bottom
```

`HAlign` / `VAlign`은 슬롯 안에서 자식 위젯을 정렬한다.

```cpp
.HAlign(HAlign_Center)
.VAlign(VAlign_Center)
```

자주 쓰는 값:

```text
HAlign_Left
HAlign_Center
HAlign_Right
HAlign_Fill

VAlign_Top
VAlign_Center
VAlign_Bottom
VAlign_Fill
```

`SlotPadding`은 패널 내부 모든 셀 또는 슬롯 사이의 간격을 지정할 때 쓴다.

```cpp
SNew(SUniformGridPanel)
	.SlotPadding(4.0f)
```

### 위젯 트리 갱신과 Attribute 호출 메커니즘

이 섹션의 초점은 `Font(this, &...)`처럼 위젯에 바인딩한 getter가 "언제, 어떤 과정을 거쳐 호출되는가"이다.

`Construct` 시점에는 위젯을 한 번 그리는 것이 아니라 위젯 트리와 호출 정보를 구성한다.

```text
SButton
  OnClicked delegate = OnDependenciesModeClicked
  Child = STextBlock
    Text attribute = "Dependencies"
    Font attribute = GetDependenciesModeFont
```

위 구조에서 `SButton`은 자기 click delegate와 child widget을 가진다. `STextBlock`은 자기 `Text`, `Font`, `ColorAndOpacity` 같은 attribute를 가진다. 바인딩 정보는 중앙 목록 하나에 모이는 것이 아니라 각 위젯 객체의 attribute / delegate에 저장된다.

현재 Mode 버튼 코드는 다음 구조를 만든다.

```cpp
SNew(SButton)
	.OnClicked(this, &SAssetReferenceInspectorWidget::OnDependenciesModeClicked)
	[
		SNew(STextBlock)
			.Text(FText::FromString(TEXT("Dependencies")))
			.Font(this, &SAssetReferenceInspectorWidget::GetDependenciesModeFont)
	]
```

호출 흐름은 다음과 같다.

```text
1. 사용자가 Dependencies 버튼을 클릭한다.

2. Slate 입력 처리 과정에서 해당 SButton이 click을 처리한다.

3. SButton에 저장된 OnClicked delegate가 호출된다.
   -> OnDependenciesModeClicked()

4. callback 안에서 상태값이 바뀐다.
   -> AnalysisOptions.Mode = Dependencies

5. 이후 Slate 갱신 / paint 과정에서 필요한 위젯 트리 영역을 다시 처리한다.
   이때 Construct를 다시 실행하는 것이 아니라, 이미 생성된 위젯 트리를 순회한다.

6. SButton을 그리는 과정에서 child인 STextBlock도 처리된다.

7. STextBlock은 자기 표현에 필요한 attribute 값을 평가한다.
   Text attribute가 상수면 그대로 사용한다.
   Font attribute가 바인딩이면 저장된 getter를 호출한다.
   -> GetDependenciesModeFont()

8. getter는 현재 상태값을 읽고 font 값을 반환한다.
   Dependencies 모드면 NormalFontBold, 아니면 NormalFont를 반환한다.

9. STextBlock은 반환된 font로 텍스트를 그린다.
```

즉 클릭 이벤트가 `GetDependenciesModeFont`를 직접 호출하는 구조는 아니다. 클릭 이벤트는 상태를 바꾸고, 이후 Slate의 위젯 트리 갱신 / paint 과정에서 `STextBlock`이 자기 `Font` 값을 필요로 할 때 바인딩된 getter가 호출된다.

```cpp
FReply SAssetReferenceInspectorWidget::OnDependenciesModeClicked()
{
	AnalysisOptions.Mode = EAssetReferenceMode::Dependencies;
	return FReply::Handled();
}
```

```cpp
FSlateFontInfo SAssetReferenceInspectorWidget::GetDependenciesModeFont() const
{
	return AnalysisOptions.Mode == EAssetReferenceMode::Dependencies
		? FAppStyle::GetFontStyle(TEXT("NormalFontBold"))
		: FAppStyle::GetFontStyle(TEXT("NormalFont"));
}
```

정리:

```text
이벤트 callback
= 사용자 입력에 반응해 상태를 변경한다.

위젯 트리 갱신 / paint
= Slate가 필요한 위젯 영역을 순회하며 각 위젯의 표현값을 평가한다.

Attribute getter
= 특정 위젯이 자기 표현값을 필요로 할 때 호출된다.
```

상수 attribute와 바인딩 attribute는 독립적으로 평가된다.

```cpp
SNew(STextBlock)
	.Text(FText::FromString(TEXT("Dependencies")))
	.Font(this, &SAssetReferenceInspectorWidget::GetDependenciesModeFont)
```

위 예시에서 `Text`는 상수이므로 getter 호출 없이 그대로 사용한다. `Font`는 바인딩이므로 `STextBlock`이 font 값을 평가할 때 `GetDependenciesModeFont`를 호출한다.

### 현재 UI Shell 구조

현재 `SAssetReferenceInspectorWidget`은 다음 구조를 가진다.

```text
SAssetReferenceInspectorWidget
  SBorder
    SVerticalBox
      STextBlock
      SSeparator
      STextBlock
      SUniformGridPanel
        SButton
        SButton
      SVerticalBox
        STextBlock
        SBorder
          SVerticalBox
            STextBlock
            STextBlock
            SUniformGridPanel
              SButton
                STextBlock
              SButton
                STextBlock
        SBorder
          SVerticalBox
            STextBlock
            SGridPanel
              STextBlock
              SEditableTextBox
              STextBlock
              SEditableTextBox
              STextBlock
              SEditableTextBox
        SBorder
          SVerticalBox
            STextBlock
            SUniformGridPanel
              SCheckBox
                STextBlock
              SCheckBox
                STextBlock
      SBorder
        STreeView
          STableRow
            STextBlock
```

각 요소의 역할:

```text
SBorder
= 탭 내부 전체 여백과 결과 영역 테두리 구성

SVerticalBox
= 제목, 선택 Asset, 버튼, 분석 옵션, 결과 영역을 세로로 배치

STextBlock
= 제목, 선택 Asset, 섹션 제목, 모드, 필터 label, 체크박스 label, Tree row text 표시

SSeparator
= 제목과 조작 영역 구분

SUniformGridPanel
= 버튼 또는 체크박스를 같은 크기로 배치

SGridPanel
= Filters 섹션에서 label과 입력칸을 2열로 배치

SButton
= Pick Selected Asset / Analyze / Mode 전환 실행 버튼. Mode 버튼은 child `STextBlock`의 font attribute binding으로 선택 상태를 표시

SEditableTextBox
= Max Depth / Path Filter / Class Filter 입력

SCheckBox
= Project Content 기본 분석 범위에 Engine Content / Plugin Content를 추가로 포함할지 입력

STreeView
= Asset 참조 관계를 계층 구조로 표시할 결과 영역
```

### STreeView 구조

`STreeView`는 여러 데이터 아이템을 계층 row 형태로 반복 표시하는 Slate 위젯이다.

현재 위젯 흐름은 다음 기준으로 이해한다.

```text
Construct
= 위젯 인스턴스 생성 시 1회 호출되어 UI 트리, 콜백 함수, 데이터 소스 주소를 연결한다.

TreeRootItems
= STreeView가 루트 데이터 소스로 참조하는 배열이다.

RequestTreeRefresh
= TreeRootItems가 바뀐 뒤 TreeView에 다시 표시하라고 요청한다.

OnGenerateTreeRow
= 데이터 노드 하나를 STableRow 위젯 하나로 변환한다.

OnGetTreeChildren
= 특정 노드가 펼쳐질 때 Children 배열을 제공한다.

CreateRelationNode
= 관련 PackageName 하나를 Tree 표시용 노드 하나로 변환한다.
```

현재 Tree View는 다음 타입을 사용한다.

```cpp
TSharedPtr<STreeView<TSharedPtr<FAssetReferenceTreeNode>>> TreeView;
```

안쪽 타입인 `TSharedPtr<FAssetReferenceTreeNode>`는 Tree View가 표시할 아이템 하나의 타입이다. 바깥쪽 `TSharedPtr<STreeView<...>>`는 실제 Slate 위젯 인스턴스를 보관하는 포인터다.

Tree 노드는 현재 선택 Asset과 Asset Registry 조회 결과를 표시하기 위한 UI 데이터 모델이다.

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

`DisplayName`은 row에 표시할 이름이고, `PackageName`은 Asset Registry 조회 기준이 되는 Package 이름이다. `ClassName`은 AssetData를 찾을 수 있는 Asset row에 함께 표시할 Class 이름이다. `SizeBytes`는 PackageName 기준으로 찾은 `.uasset`, `.uexp`, `.ubulk` 파일 크기 합산값이다. `Children`은 노드를 펼쳤을 때 보여줄 자식 노드 목록이다.

Tree View 연결의 핵심은 세 가지다.

```cpp
SAssignNew(TreeView, STreeView<TSharedPtr<FAssetReferenceTreeNode>>)
	.TreeItemsSource(&TreeRootItems)
	.OnGenerateRow(this, &SAssetReferenceInspectorWidget::OnGenerateTreeRow)
	.OnGetChildren(this, &SAssetReferenceInspectorWidget::OnGetTreeChildren)
```

`TreeItemsSource(&TreeRootItems)`는 Tree View의 최상위 아이템 배열을 지정한다. 이 배열은 주소로 전달되므로 `Construct` 내부 지역 변수가 아니라 위젯 클래스 멤버로 유지해야 한다. 지역 변수로 만들면 `Construct` 종료 후 주소가 무효가 된다.

`TreeRootItems`의 내용이 바뀌어도 화면이 자동으로 즉시 바뀌지는 않는다. 분석 결과로 배열을 다시 구성한 뒤에는 `RequestTreeRefresh`로 Tree View 갱신을 요청한다.

```cpp
TreeView->RequestTreeRefresh();
```

이 요청 이후 Tree View는 `TreeItemsSource`를 다시 읽고, 필요한 row와 children을 콜백으로 다시 요청한다.

`OnGenerateRow`는 데이터 노드 하나를 화면 row 하나로 변환하는 콜백 슬롯이다. 이 API가 받는 delegate는 `FOnGenerateRow` 계열이며, 아이템 하나와 부모 table을 받아 `TSharedRef<ITableRow>`를 반환하는 함수가 필요하다.

```cpp
TSharedRef<ITableRow> OnGenerateTreeRow(
	TSharedPtr<FAssetReferenceTreeNode> Item,
	const TSharedRef<STableViewBase>& OwnerTable) const;
```

현재는 `GetTreeNodeDisplayText`에서 `FAssetReferenceTreeNode`의 표시 문자열을 만든 뒤, 그 값을 `STextBlock`으로 표시하는 `STableRow`를 반환한다. `ClassName`이 있으면 `DisplayName [ClassName]` 형식으로 표시하고, `SizeBytes`가 0보다 크면 `(Size)` suffix를 붙인다. `bIsCircular`가 true이면 뒤에 `[Circular]`을 붙여 현재 DFS 경로에서 다시 등장한 순환 후보 노드임을 보여준다.

표시 순서는 다음과 같다.

```text
DisplayName [ClassName] (Size) [Circular]
```

Class 정보를 찾을 수 없는 Package, placeholder, 파일 경로를 찾을 수 없는 Package는 확인 가능한 suffix만 표시한다.

```cpp
return SNew(STableRow<TSharedPtr<FAssetReferenceTreeNode>>, OwnerTable)
	[
		SNew(STextBlock)
			.Text(FText::FromString(GetTreeNodeDisplayText(Item)))
	];
```

이 코드는 다음 의미다.

```text
데이터 노드 하나
-> STableRow 한 줄 생성
-> Row 내부에 STextBlock 배치
-> TextBlock의 Text 값으로 GetTreeNodeDisplayText 결과 표시
```

`STableRow`는 `SListView`, `STreeView` 같은 테이블 계열 위젯에서 아이템 하나를 표현하는 화면 줄이다. 여기서 테이블 계열은 위계가 있다는 뜻이 아니라, 여러 데이터 아이템을 row 또는 tile 형태로 반복 표시하는 위젯 계열을 뜻한다.

`OwnerTable`은 이 row가 소속되는 부모 테이블 위젯이다. 실제로는 현재 `STreeView`가 넘어오지만, `STableRow`가 `SListView`, `STreeView`, `STileView` 계열에서 공통으로 쓰일 수 있도록 공통 부모 타입인 `STableViewBase`로 받는다.

`OnGetChildren`은 특정 노드를 펼쳤을 때 보여줄 자식 목록을 제공하는 콜백 슬롯이다.

```cpp
OutChildren.Append(Item->Children);
```

전체 흐름은 다음과 같다.

```text
Analyze 클릭
-> BuildRelationTree()
-> 선택 Asset을 루트 노드로 구성
-> Asset Registry Dependencies / Referencers를 자식 노드로 구성
-> RefreshTree()
-> TreeView가 row 필요 시 OnGenerateTreeRow 호출
-> 노드를 펼칠 때 OnGetTreeChildren 호출
```

즉 Tree View는 데이터를 직접 해석해서 그리지 않는다. 데이터 소스와 콜백 슬롯을 통해 필요한 row와 children을 요청하면서 화면을 구성한다.

---

## Slate API 사용 노트

### SNew / SAssignNew

`SNew`는 위젯을 생성하고, `SAssignNew`는 위젯을 생성하면서 멤버 포인터에도 저장한다. `AssetReferenceInspector`는 생성 이후 `TreeView->RequestTreeRefresh()`와 `TreeView->SetItemExpansion(...)`을 호출해야 하므로 `STreeView`만 `SAssignNew`로 보관한다.

자세한 항목은 `Slate_Widget_API_Catalog_KR.md`의 `SNew`, `SAssignNew`, `[] Content Slot`을 참고한다.

### Text Binding

`Text(FText::FromString(...))`는 고정 표시값이고, `Text(this, &...)`는 표시 시점에 위젯 상태를 읽는 동적 바인딩이다. 선택 Asset, 현재 모드, Path / Class Filter는 위젯 상태가 바뀔 수 있으므로 동적 바인딩을 사용한다.

자세한 항목은 `Slate_Widget_API_Catalog_KR.md`의 `STextBlock`, `SEditableTextBox`를 참고한다.

### SButton::OnClicked / FReply

`OnClicked`는 버튼 클릭 시 실행할 함수를 등록한다. `Construct`에서 함수를 즉시 실행하는 것이 아니라, 클릭 이벤트가 발생했을 때 호출할 callback을 연결한다. 버튼 callback은 Slate 입력 처리 결과인 `FReply`를 반환한다.

현재 버튼들은 선택 Asset 가져오기, 분석 실행, Dependencies / Referencers 모드 전환을 담당한다. 자세한 항목은 `Slate_Widget_API_Catalog_KR.md`의 `SButton`, `OnClicked`, `FReply`를 참고한다.

### SCheckBox / ECheckBoxState

`SCheckBox`는 `IsChecked`로 현재 상태를 읽고, `OnCheckStateChanged`로 상태 변경을 저장한다. 현재 Engine / Plugin Content 옵션은 Project Content 기본 분석 범위를 확장하는 `Include External Content` 옵션이며, `ECheckBoxState::Checked`와 `Unchecked`만 사용한다.

`SCheckBox [ STextBlock ]`에서 `STextBlock`은 체크박스 옆 label/content로 표시된다. 자세한 항목은 `Slate_Widget_API_Catalog_KR.md`의 `SCheckBox`, `IsChecked`, `OnCheckStateChanged`, `ECheckBoxState`를 참고한다.

### SEditableTextBox와 입력 Commit

`SEditableTextBox`는 `Text`로 저장된 상태를 표시하고, `OnTextCommitted`에서 입력 확정 시점에 상태를 갱신한다. 현재 Max Depth / Path / Class Filter는 입력 중 실시간 반영하지 않고, Enter 또는 포커스 이동으로 commit될 때만 저장한다.

`OnCleared`는 텍스트가 비었다는 뜻이 아니라 focus clear 계열 commit 원인이다. 이 프로젝트에서는 Enter 직후 `OnCleared`가 추가로 들어오는 사례를 확인했기 때문에, 상태 덮어쓰기를 막기 위해 `OnCleared`를 무시한다.

자세한 commit 종류와 검증 기록은 `Slate_Widget_API_Catalog_KR.md`의 `SEditableTextBox`, `OnTextCommitted`, `ETextCommit::Type`을 참고한다.

### Max Depth 입력 정책

Max Depth는 `SEditableTextBox`로 입력받지만 내부 옵션은 `int32` 값이다. 따라서 commit 시점에 텍스트를 정수로 파싱하고, 유효한 정수일 때만 `AnalysisOptions.MaxDepth`에 반영한다. 입력값은 관계 Tree 재귀 탐색 범위에 직접 영향을 주므로 `0~10` 범위로 제한한다.

```cpp
namespace
{
	constexpr int32 MaxAllowedRelationDepth = 10;
}

int32 ParsedMaxDepth = AnalysisOptions.MaxDepth;
if (FDefaultValueHelper::ParseInt(InText.ToString().TrimStartAndEnd(), ParsedMaxDepth))
{
	AnalysisOptions.MaxDepth = FMath::Clamp(ParsedMaxDepth, 0, MaxAllowedRelationDepth);
}
```

정책:

```text
"3"
= 정수 파싱 성공, MaxDepth = 3

" 2 "
= 앞뒤 공백 제거 후 정수 파싱 성공, MaxDepth = 2

"-1"
= 정수 파싱은 성공하지만 0으로 보정, MaxDepth = 0

"999"
= 정수 파싱은 성공하지만 10으로 보정, MaxDepth = 10

"abc", "1.5", ""
= 정수 파싱 실패, 기존 MaxDepth 유지
```

`FMath::Clamp(ParsedMaxDepth, 0, MaxAllowedRelationDepth)`를 사용하는 이유는 음수 depth를 분석 옵션으로 허용하지 않고, 동시에 과도하게 큰 depth 입력으로 Asset Registry 조회와 Tree 재귀 탐색이 폭증하는 상황을 막기 위해서다. 현재 Tree 생성 조건은 부모 노드의 `Depth`가 `MaxDepth` 이상이면 하위 탐색을 중단한다.

```cpp
if (!ParentNode.IsValid() || ParentNode->Depth >= AnalysisOptions.MaxDepth)
{
	return;
}
```

따라서 `MaxDepth = 0`은 선택한 root Asset만 표시하고 자식 관계는 탐색하지 않는 값으로 해석한다.

`MaxAllowedRelationDepth = 10`은 현재 UI 입력 방어선이다. 실제 프로젝트 규모와 필터 정책이 확장되면 별도 설정값 또는 고급 옵션으로 분리할 수 있다.

---

## 현재 구현 흐름

### Dependency Node 표시 정책

Asset Registry의 `GetDependencies` / `GetReferencers`는 짧은 Asset 이름이 아니라 PackageName 목록을 반환한다.

```text
Input:
  /Game/ARI_Demo/BP_Dummy

Output:
  /Game/ARI_Demo/M_Dummy
  /Engine/...
  /Script/...
```

Tree View에서는 긴 PackageName보다 짧은 AssetName이 읽기 좋다. 그래서 `CreateRelationNode`는 PackageName을 기본 표시명으로 둔 뒤, 해당 Package에 AssetData가 있으면 첫 번째 AssetData의 AssetName을 표시명으로 사용한다.

```cpp
FString DisplayName = PackageName.ToString();

TArray<FAssetData> AssetsInPackage;
AssetRegistryModule.Get().GetAssetsByPackageName(PackageName, AssetsInPackage);

if (AssetsInPackage.Num() > 0)
{
	DisplayName = AssetsInPackage[0].AssetName.ToString();
}
```

정책:

```text
PackageName
= 분석용 안정 식별자

DisplayName
= UI 표시용 짧은 이름

AssetsInPackage.Num() == 0
= AssetData를 찾지 못했으므로 PackageName 전체를 표시

AssetsInPackage.Num() > 0
= 현재 MVP에서는 첫 번째 AssetData의 AssetName을 표시
```

하나의 PackageName에서 여러 `FAssetData`가 반환될 수 있으므로, 첫 번째 AssetData를 대표 표시명으로 쓰는 것은 MVP 단계의 단순화다. 후속 개선에서는 Package short name과 AssetName이 같은 항목을 우선 선택하는 정책을 추가할 수 있다.

### Content Browser 선택 Asset 표시 흐름

`Pick Selected Asset` 버튼은 Content Browser에서 현재 선택된 Asset을 가져와 위젯 상태로 저장한다.

핵심 흐름:

```text
Pick Selected Asset 클릭
-> OnPickSelectedAssetClicked 호출
-> FContentBrowserModule 로드
-> IContentBrowserSingleton::GetSelectedAssets 호출
-> 첫 번째 FAssetData를 SelectedAssetData에 저장
-> GetSelectedAssetText가 선택 Asset 이름과 PackageName을 표시
```

이번 단계에서는 다중 선택을 분석하지 않고 첫 번째 Asset만 사용한다. 선택된 Asset이 없으면 `SelectedAssetData`를 빈 `FAssetData`로 초기화하고 `Selected Asset: None`을 표시한다.

```cpp
SelectedAssetData = SelectedAssets.Num() > 0 ? SelectedAssets[0] : FAssetData();
```

`FContentBrowserModule`, `IContentBrowserSingleton`, `FModuleManager`는 함수 구현 내부에서만 필요하므로 `.cpp`에 include한다. `FAssetData`는 위젯의 멤버 필드 타입으로 헤더에 노출되므로 헤더에서 `AssetRegistry/AssetData.h`를 include한다.

### 현재 코드에서 의도한 역할

`FAssetReferenceInspectorModule`은 더 이상 탭 내부 UI를 직접 구성하지 않는다.

```text
FAssetReferenceInspectorModule
= 탭 생성과 메뉴/Command 등록 진입점

SAssetReferenceInspectorWidget
= 탭 내부 UI 루트 위젯
```

이 구조를 유지하면 이후 기능이 늘어나도 모듈 cpp가 UI 코드로 커지지 않는다.

현재 `SAssetReferenceInspectorWidget`은 다음 역할을 가진다.

```text
- 선택 Asset 표시 상태 관리
- Analyze 버튼 이벤트 연결
- Dependencies / Referencers 모드 선택 UI
- Path / Class Filter 입력 UI
- Include External Content 표시 옵션 UI
- STreeView 결과를 Asset Registry 분석 데이터와 연결
- Tree node 더블 클릭 시 Content Browser Sync
```

---

## 작성 기준

Slate UI를 추가할 때는 다음 기준을 따른다.

- `SDockTab` 안에 직접 UI를 길게 작성하지 않는다.
- 탭 내부 화면은 전용 `SCompoundWidget`으로 분리한다.
- 배치 컨테이너와 실제 입력/표시 위젯의 역할을 구분한다.
- 상단 조작 영역은 `AutoHeight`, 결과 영역은 가능한 `FillHeight`를 사용한다.
- placeholder는 다음 기능을 붙일 자리를 보여주는 수준으로 유지한다.
- Tree View 데이터 배열은 `STreeView`보다 오래 살아야 하므로 위젯 멤버로 유지한다.
- row 표시 방식은 `OnGenerateRow`, 계층 데이터 제공은 `OnGetChildren`에서 다룬다.
