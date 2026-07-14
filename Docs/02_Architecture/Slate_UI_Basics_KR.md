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
        SScrollBox
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
= 이후 실제 기능이 연결될 버튼 placeholder

SScrollBox
= 이후 STreeView나 긴 분석 결과가 들어갈 영역
```

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
- Pick Selected Asset 버튼 이벤트 연결
- Analyze 버튼 이벤트 연결
- Dependencies / Referencers 모드 선택 UI
- Max Depth 입력 UI
- STreeView 결과 표시
```

## 작성 기준

Slate UI를 추가할 때는 다음 기준을 따른다.

- `SDockTab` 안에 직접 UI를 길게 작성하지 않는다.
- 탭 내부 화면은 전용 `SCompoundWidget`으로 분리한다.
- 배치 컨테이너와 실제 입력/표시 위젯의 역할을 구분한다.
- 상단 조작 영역은 `AutoHeight`, 결과 영역은 가능한 `FillHeight`를 사용한다.
- placeholder는 다음 기능을 붙일 자리를 보여주는 수준으로 유지한다.
- 실제 데이터 표시가 필요해지면 `STreeView` 등 목적에 맞는 Slate 위젯으로 교체한다.
