# Slate UI Basics

이 문서는 `AssetReferenceInspector` Editor Plugin에서 사용하는 Slate UI 요소와 기본 작성 규칙을 정리한다.

Slate는 Unreal Engine의 C++ 기반 UI 프레임워크다. Editor Plugin UI는 UMG보다 Slate를 직접 사용하는 경우가 많으며, 이 플러그인의 탭 내부 UI도 Slate 위젯으로 구성한다.

## 기본 개념

Slate 위젯 클래스는 보통 `S` 접두사를 사용한다.

```text
SButton = 버튼
STextBlock = 텍스트
SBorder = 테두리 / 배경 / 패딩 컨테이너
SBox = 크기 제약 컨테이너
SVerticalBox = 세로 배치 컨테이너
SHorizontalBox = 가로 배치 컨테이너
SUniformGridPanel = 같은 크기 셀 그리드
SScrollBox = 스크롤 가능한 컨테이너
SSeparator = 구분선
STreeView = 트리 표시 위젯
SDockTab = 에디터 도킹 탭
```

Slate 위젯은 보통 `SNew` 매크로로 생성한다.

```cpp
SNew(STextBlock)
	.Text(FText::FromString(TEXT("Asset Reference Inspector")))
```

생성한 위젯을 이후 코드에서 다시 접근해야 할 때는 `SAssignNew`를 사용한다.

```cpp
SAssignNew(TreeView, STreeView<TSharedPtr<FAssetReferenceTreeNode>>)
```

차이는 다음과 같다.

```text
SNew
= Slate 위젯을 생성해서 바로 사용한다.

SAssignNew
= Slate 위젯을 생성하면서 `TSharedPtr` 변수에도 저장한다.
```

`AssetReferenceInspector`에서는 `TreeView->SetItemExpansion(...)`처럼 생성 이후 Tree View API를 호출해야 하므로 `SAssignNew`로 `TreeView` 멤버에 저장한다.

위젯은 대괄호 `[]`로 자식 위젯을 중첩해 UI 트리를 만든다.

```cpp
SNew(SBorder)
[
	SNew(STextBlock)
		.Text(FText::FromString(TEXT("Selected Asset: None")))
]
```

## SCompoundWidget

`SAssetReferenceInspectorWidget`은 `SCompoundWidget`을 상속한다.

`SCompoundWidget`은 여러 Slate 위젯을 조합해서 하나의 커스텀 위젯을 만들 때 사용한다.

핵심 구조:

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

## Construct와 바인딩

`Construct`는 위젯 인스턴스가 생성될 때 한 번 호출되는 초기 구성 함수다. UI가 갱신될 때마다 `Construct`가 다시 위에서 아래로 실행되는 것은 아니다.

`Construct`에서 주로 하는 일:

```text
- UI 트리 생성
- Slot과 레이아웃 구성
- 이벤트 콜백 바인딩
- 동적 속성 바인딩
```

예를 들어 `Pick Selected Asset` 버튼은 `Construct`에서 생성되고, 클릭 이벤트에 함수가 연결된다.

```cpp
SNew(SButton)
	.Text(FText::FromString(TEXT("Pick Selected Asset")))
	.OnClicked(this, &SAssetReferenceInspectorWidget::OnPickSelectedAssetClicked)
```

이 코드는 `Construct` 시점에 `OnPickSelectedAssetClicked`를 실행하는 것이 아니다. 버튼이 클릭되었을 때 호출할 함수를 등록한다.

`STextBlock`의 텍스트도 고정값과 동적 바인딩을 구분한다.

```cpp
.Text(FText::FromString(TEXT("Selected Asset: None")))
```

위 코드는 고정 텍스트 값을 넣는다.

```cpp
.Text(this, &SAssetReferenceInspectorWidget::GetSelectedAssetText)
```

위 코드는 텍스트 값이 필요할 때 `GetSelectedAssetText`를 호출하도록 바인딩한다. `SelectedAssetData` 같은 위젯 상태가 바뀌면, Slate가 다음 표시 갱신 시점에 이 함수를 호출해 현재 상태 기준의 텍스트를 가져온다.

정리하면 다음과 같다.

```text
Construct
= 생성 시 한 번 UI 구조와 연결선을 만든다.

OnClicked
= 클릭 이벤트가 발생했을 때 호출할 함수 바인딩이다.

Text(this, &...)
= 텍스트 속성이 필요할 때 호출할 함수 바인딩이다.
```

`SButton::OnClicked`에 연결하는 함수는 `FReply`를 반환해야 한다.

```cpp
FReply OnPickSelectedAssetClicked();
```

`FReply`는 Slate 입력 이벤트 처리 결과를 담는 응답 객체다.

```text
FReply::Handled()
= 이 위젯이 이벤트를 처리했다.

FReply::Unhandled()
= 이 위젯이 이벤트를 처리하지 않았다.
```

현재 `Pick Selected Asset` 버튼은 클릭 이벤트 안에서 Content Browser 선택 Asset을 가져오고 `FReply::Handled()`를 반환한다.

## Slot과 배치

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

## 자주 쓰는 Slot 옵션

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

## 현재 UI Shell 구조

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
= 제목, 선택 Asset, 버튼, 모드, 결과 영역을 세로로 배치

STextBlock
= 제목, 선택 Asset placeholder, 모드 placeholder, 결과 placeholder 표시

SSeparator
= 제목과 조작 영역 구분

SUniformGridPanel
= Pick Selected Asset / Analyze 버튼을 같은 크기로 배치

SButton
= Pick Selected Asset / Analyze 실행 버튼

STreeView
= Asset 참조 관계를 계층 구조로 표시할 결과 영역
```

## STreeView 기본 구조

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

CreateDependencyNode
= Dependency PackageName 하나를 Tree 표시용 노드 하나로 변환한다.
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
	FAssetReferenceTreeNode(const FString& InDisplayName, FName InPackageName = NAME_None);

	FString DisplayName;
	FName PackageName;
	TArray<TSharedPtr<FAssetReferenceTreeNode>> Children;
};
```

`DisplayName`은 row에 표시할 이름이고, `PackageName`은 Asset Registry 조회 기준이 되는 Package 이름이다. `Children`은 노드를 펼쳤을 때 보여줄 자식 노드 목록이다.

Dependencies Depth 1 조회 후에는 다음 구조를 만든다.

```text
BP_Dummy
  M_Dummy
  /Engine/... 또는 /Script/... Package
```

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

현재 함수 형태:

```cpp
TSharedRef<ITableRow> OnGenerateTreeRow(
	TSharedPtr<FAssetReferenceTreeNode> Item,
	const TSharedRef<STableViewBase>& OwnerTable) const;
```

현재는 `FAssetReferenceTreeNode::DisplayName`을 `STextBlock`으로 표시하는 `STableRow`를 반환한다.

```cpp
return SNew(STableRow<TSharedPtr<FAssetReferenceTreeNode>>, OwnerTable)
	[
		SNew(STextBlock)
			.Text(FText::FromString(DisplayName))
	];
```

이 코드는 다음 의미다.

```text
데이터 노드 하나
-> STableRow 한 줄 생성
-> Row 내부에 STextBlock 배치
-> TextBlock의 Text 값으로 DisplayName 표시
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
-> BuildDependencyTree()
-> 선택 Asset을 루트 노드로 구성
-> Asset Registry Dependencies를 자식 노드로 구성
-> RefreshTree()
-> TreeView가 row 필요 시 OnGenerateTreeRow 호출
-> 노드를 펼칠 때 OnGetTreeChildren 호출
```

즉 Tree View는 데이터를 직접 해석해서 그리지 않는다. 데이터 소스와 콜백 슬롯을 통해 필요한 row와 children을 요청하면서 화면을 구성한다.

## Dependency Node 표시 정책

Asset Registry의 `GetDependencies`는 짧은 Asset 이름이 아니라 PackageName 목록을 반환한다.

```text
Input:
  /Game/ARI_Demo/BP_Dummy

Output:
  /Game/ARI_Demo/M_Dummy
  /Engine/...
  /Script/...
```

Tree View에서는 긴 PackageName보다 짧은 AssetName이 읽기 좋다. 그래서 `CreateDependencyNode`는 PackageName을 기본 표시명으로 둔 뒤, 해당 Package에 AssetData가 있으면 첫 번째 AssetData의 AssetName을 표시명으로 사용한다.

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

## Content Browser 선택 Asset 표시 흐름

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

## 현재 코드에서 의도한 역할

`FAssetReferenceInspectorModule`은 더 이상 탭 내부 UI를 직접 구성하지 않는다.

```text
FAssetReferenceInspectorModule
= 탭 생성과 메뉴/Command 등록 진입점

SAssetReferenceInspectorWidget
= 탭 내부 UI 루트 위젯
```

이 구조를 유지하면 이후 기능이 늘어나도 모듈 cpp가 UI 코드로 커지지 않는다.

다음 단계에서 `SAssetReferenceInspectorWidget`은 다음 역할을 갖게 된다.

```text
- 선택 Asset 표시 상태 관리
- Analyze 버튼 이벤트 연결
- Dependencies / Referencers 모드 선택 UI
- Max Depth 입력 UI
- STreeView 결과를 실제 Asset 분석 데이터와 연결
```

## 작성 기준

Slate UI를 추가할 때는 다음 기준을 따른다.

- `SDockTab` 안에 직접 UI를 길게 작성하지 않는다.
- 탭 내부 화면은 전용 `SCompoundWidget`으로 분리한다.
- 배치 컨테이너와 실제 입력/표시 위젯의 역할을 구분한다.
- 상단 조작 영역은 `AutoHeight`, 결과 영역은 가능한 `FillHeight`를 사용한다.
- placeholder는 다음 기능을 붙일 자리를 보여주는 수준으로 유지한다.
- Tree View 데이터 배열은 `STreeView`보다 오래 살아야 하므로 위젯 멤버로 유지한다.
- row 표시 방식은 `OnGenerateRow`, 계층 데이터 제공은 `OnGetChildren`에서 다룬다.
