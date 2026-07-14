# Editor Window Flow

이 문서는 `AssetReferenceInspector` Editor Plugin이 Unreal Editor에서 독립형 창을 여는 구조를 정리한다.

현재 단계의 목표는 실제 Asset 분석 기능이 아니라, 에디터 `Window` 메뉴에서 `Asset Reference Inspector` 항목을 클릭했을 때 기본 Nomad Tab이 열리는 흐름을 이해하고 유지하는 것이다.

## 플러그인과 모듈 위치

Unreal Plugin은 프로젝트의 `Plugins/<PluginName>` 아래에 배치한다.

```text
Plugins/AssetReferenceInspector/
```

`.uplugin`은 플러그인의 메타데이터 파일이다. 여기에는 플러그인 표시 정보와 구성 모듈의 이름, 타입, 로드 페이즈가 들어간다.

```json
"Modules": [
  {
    "Name": "AssetReferenceInspector",
    "Type": "Editor",
    "LoadingPhase": "Default"
  }
]
```

모듈 소스는 `Plugins/<PluginName>/Source/<ModuleName>` 아래에 둔다.

```text
Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/
```

`AssetReferenceInspector.Build.cs`는 이 모듈의 빌드 규칙을 담는다. `PublicDependencyModuleNames`는 Public 헤더를 사용하는 외부 모듈에도 전파되어야 하는 의존성이고, `PrivateDependencyModuleNames`는 모듈 내부 구현에서만 필요한 의존성이다.

현재 모듈의 C++ 구성요소는 다음처럼 나뉜다.

```text
Public/
  AssetReferenceInspectorModule.h

Private/
  AssetReferenceInspectorModule.cpp
  AssetReferenceInspectorCommands.h
  AssetReferenceInspectorCommands.cpp
```

## 모듈 프레임 객체

`FAssetReferenceInspectorModule`은 `IModuleInterface`를 상속한 모듈의 기본 프레임 객체다.

이 객체는 모듈이 로드될 때 실행되는 `StartupModule()`과 모듈이 언로드될 때 실행되는 `ShutdownModule()`을 override한다.

```text
StartupModule = 모듈 초기화와 등록 지점
ShutdownModule = StartupModule에서 등록한 항목 정리 지점
```

현재 `StartupModule()`은 다음 작업을 수행한다.

```text
1. FAssetReferenceInspectorCommands::Register() 호출
2. PluginCommands FUICommandList 생성
3. Command와 실제 실행 함수 매핑
4. Nomad Tab Spawner 등록
5. ToolMenus Startup Callback 등록
```

현재 `ShutdownModule()`은 다음 작업을 수행한다.

```text
1. ToolMenus Startup Callback 해제
2. 현재 모듈이 등록한 메뉴 항목 해제
3. Nomad Tab Spawner 해제
4. FAssetReferenceInspectorCommands::Unregister() 호출
```

즉, `FAssetReferenceInspectorModule`은 플러그인의 모든 기능을 직접 수행하는 객체라기보다, 이 모듈이 에디터에 제공하는 확장 지점을 등록하고 정리하는 진입점이다.

## Command 정보 객체

`FAssetReferenceInspectorCommands`는 이 플러그인에서 사용할 UI Command 정보를 정의하는 클래스다.

이 클래스는 `TCommands<FAssetReferenceInspectorCommands>`를 상속한다. `TCommands`는 Command Context 등록, `Register` / `Unregister`, `Get` 접근 흐름을 제공하는 템플릿 기반 베이스 클래스다.

생성자에서 넘기는 값은 Command 묶음 자체의 메타정보다.

```text
Context 이름 = 엔진 내부에서 이 Command 묶음을 식별하는 이름
Context 표시 이름 = 사용자에게 보일 수 있는 묶음 이름
부모 Context = 다른 Command Context 아래에 둘 경우 사용
Style Set 이름 = Command 아이콘과 스타일을 찾을 때 사용할 스타일 세트
```

`FAssetReferenceInspectorCommands::Register()`를 호출하면 `TCommands`가 관리하는 Command Context가 준비되고, override한 `RegisterCommands()`가 호출된다.

`RegisterCommands()` 내부의 `UI_COMMAND`는 `OpenPluginWindow`라는 `FUICommandInfo`에 Command 정보를 구성하고 Command 시스템에 등록한다.

```text
UI_COMMAND가 등록하는 정보
- C++에서 참조할 FUICommandInfo 변수
- UI에 표시될 Command 이름
- Tooltip 또는 설명
- Command UI 타입
- 기본 단축키
```

현재 Command 정보는 다음과 같다.

```text
FUICommandInfo 변수 = OpenPluginWindow
표시 이름 = Asset Reference Inspector
설명 = Open the Asset Reference Inspector window.
UI 타입 = Button
기본 단축키 = 없음
```

중요한 점은 `UI_COMMAND`가 실제 실행 함수를 연결하지 않는다는 것이다. 이 단계에서는 엔진이 인식할 수 있는 Command 메타데이터만 등록된다.

## CommandList와 실행 함수 매핑

`FUICommandInfo`는 Command의 이름, 설명, 타입, 단축키 같은 메타데이터다.

`FUICommandList`는 해당 Command가 실행될 때 어떤 C++ 함수를 호출할지 저장하는 실행 매핑 테이블이다.

현재 모듈은 `PluginCommands`를 만들고, `FAssetReferenceInspectorCommands`에서 정의한 `OpenPluginWindow` Command를 실제 실행 함수에 매핑한다.

```text
OpenPluginWindow FUICommandInfo
-> FExecuteAction Delegate
-> FAssetReferenceInspectorModule::OpenPluginWindow()
```

`FExecuteAction::CreateRaw(this, &FAssetReferenceInspectorModule::OpenPluginWindow)`는 일반 C++ 객체인 현재 모듈 객체와 멤버 함수를 `FExecuteAction` Delegate 객체로 포장한다.

`CreateRaw`는 객체 생명주기를 자동 추적하지 않는다. 따라서 `StartupModule()`에서 등록한 Command, 메뉴, 탭 Spawner는 `ShutdownModule()`에서 명시적으로 해제해야 한다.

## ToolMenus와 메뉴 등록

`UToolMenus::RegisterStartupCallback`은 ToolMenus 시스템이 메뉴를 수정할 수 있는 시점에 `RegisterMenus()`를 호출하도록 예약한다.

이 콜백을 사용하는 이유는 모듈 로드 시점에 에디터 메뉴 시스템이 아직 준비되지 않았을 수 있기 때문이다.

`RegisterMenus()`는 에디터 메뉴 UI에 Command를 배치하는 함수다.

```text
1. FToolMenuOwnerScoped OwnerScoped(this)
2. LevelEditor.MainMenu.Window 메뉴 가져오기
3. WindowLayout 섹션 가져오기 또는 생성
4. AddMenuEntryWithCommandList로 Command를 메뉴에 추가
```

`FToolMenuOwnerScoped OwnerScoped(this)`는 이 스코프 안에서 추가하는 메뉴 항목의 소유자를 현재 모듈 객체로 지정한다.

이 설정 덕분에 `ShutdownModule()`에서 `UToolMenus::UnregisterOwner(this)`를 호출하면, 이 모듈이 등록한 메뉴 항목을 정리할 수 있다.

`AddMenuEntryWithCommandList`의 두 인자는 역할이 다르다.

```text
첫 번째 인자 = 메뉴에 표시할 FUICommandInfo
두 번째 인자 = 클릭 시 실행 매핑을 찾을 FUICommandList
```

따라서 현재 메뉴 등록은 다음 의미를 가진다.

```text
Window 메뉴의 WindowLayout 섹션에
OpenPluginWindow Command를 표시한다.

표시 이름과 설명은 FUICommandInfo에서 가져오고,
실제 실행 함수는 PluginCommands에서 찾는다.
```

즉, `UI_COMMAND`는 Command 정보를 정의하고, `MapAction`은 기능을 연결하고, `AddMenuEntryWithCommandList`는 해당 Command를 실제 UI에 표시한다.

## Nomad Tab 등록과 생성

`FGlobalTabmanager`는 에디터 탭 정보를 관리하는 전역 TabManager다.

`RegisterNomadTabSpawner`는 탭을 즉시 여는 함수가 아니다. 이 함수는 특정 TabId와 탭 생성 Delegate를 TabManager에 등록한다.

현재 등록 구조는 다음과 같다.

```text
TabId = AssetReferenceInspectorTabName
Delegate = FOnSpawnTab::CreateRaw(this, &FAssetReferenceInspectorModule::OnSpawnPluginTab)
```

`FOnSpawnTab`은 탭 생성용 Delegate 타입이다. `CreateRaw`는 일반 C++ 멤버 함수인 `OnSpawnPluginTab`을 `FOnSpawnTab` Delegate 객체로 포장한다.

`FOnSpawnTab`은 대략 다음 시그니처의 함수를 요구한다.

```cpp
TSharedRef<SDockTab> FunctionName(const FSpawnTabArgs& Args);
```

그래서 현재 탭 생성 함수는 다음 형태를 가진다.

```cpp
TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);
```

`OpenPluginWindow()`가 호출되면 내부에서 `TryInvokeTab(AssetReferenceInspectorTabName)`을 호출한다.

TabManager는 이 TabId를 기준으로 이미 열린 탭이 있는지 확인한다. 이미 열려 있으면 기존 탭을 활성화하고, 없으면 등록된 `FOnSpawnTab` Delegate를 실행해 새 탭을 생성한다.

```text
TryInvokeTab(AssetReferenceInspectorTabName)
-> TabManager가 TabId 검색
-> 등록된 FOnSpawnTab Delegate 실행
-> OnSpawnPluginTab 호출
-> SDockTab 반환
-> 에디터에 탭 표시
```

## 탭 생성 함수와 Slate UI

`OnSpawnPluginTab()`은 실제 탭의 형태를 구성하는 함수다.

현재는 최소 검증을 위해 다음 구조만 만든다.

```text
SDockTab
  SBox
    STextBlock
```

`SDockTab`은 에디터에서 도킹 가능한 탭 객체다. `ETabRole::NomadTab`은 특정 Asset Editor에 종속되지 않고 에디터 전체에서 독립적으로 열 수 있는 도구 탭이라는 의미다.

`SBox`는 임시 콘텐츠를 가운데 배치하기 위한 컨테이너이고, `STextBlock`은 탭이 정상적으로 열렸음을 확인하기 위한 텍스트를 표시한다.

이후 실제 분석 UI는 `SDockTab` 안에 직접 계속 추가하기보다, 별도 Slate 위젯인 `SAssetReferenceInspectorWidget`을 만들어 탭 내부 루트 위젯으로 넣는 방향이 적합하다.

## 실행 흐름 요약

현재 에디터 창 오픈 흐름은 다음처럼 정리할 수 있다.

```text
StartupModule()
-> FAssetReferenceInspectorCommands::Register()
-> UI_COMMAND로 OpenPluginWindow FUICommandInfo 등록
-> PluginCommands 생성
-> MapAction으로 OpenPluginWindow Command와 OpenPluginWindow() 함수 매핑
-> RegisterNomadTabSpawner로 TabId와 OnSpawnPluginTab Delegate 등록
-> ToolMenus Startup Callback에 RegisterMenus 등록

ToolMenus 준비
-> RegisterMenus()
-> Window 메뉴에 OpenPluginWindow Command 표시

사용자 메뉴 클릭
-> FUICommandList에서 OpenPluginWindow 실행 매핑 검색
-> FAssetReferenceInspectorModule::OpenPluginWindow()
-> TryInvokeTab(AssetReferenceInspectorTabName)
-> TabManager가 OnSpawnPluginTab Delegate 실행
-> SDockTab 생성 및 표시

ShutdownModule()
-> 메뉴 콜백과 메뉴 소유자 해제
-> Tab Spawner 해제
-> Command 등록 해제
```

## 핵심 구분

현재 구조에서 같은 "등록"이라는 표현이 여러 번 나오지만, 실제 의미는 서로 다르다.

```text
FAssetReferenceInspectorCommands::Register()
= Command Context와 Command 정보를 Command 시스템에 등록

UI_COMMAND
= 개별 FUICommandInfo 메타데이터 등록

FUICommandList::MapAction
= Command 정보와 실제 실행 함수 매핑

AddMenuEntryWithCommandList
= Command를 에디터 메뉴 UI에 배치

RegisterNomadTabSpawner
= TabId와 탭 생성 Delegate를 TabManager에 등록
```

즉, 현재 플러그인 창 오픈 구조는 Unreal Editor가 제공하는 Command, ToolMenus, TabManager, Slate 시스템에 필요한 정보를 단계별로 등록하고 연결하는 방식으로 구성된다.
