# Feature Work Plan

## 목적

이 문서는 `AssetReferenceInspector` 개발을 주차 단위가 아니라 기능, 리팩토링, 검증 단위로 나누어 진행하기 위한 기준이다.

각 작업 단위는 가능한 한 하나의 명확한 목적을 가지며, 빌드 가능한 상태와 커밋 가능한 경계를 목표로 한다.

## Week / Phase / MVP 용어

Week는 일정과 목표를 설명하는 단위이고, Phase는 구현 범위를 설명하는 단위다.

```text
1주차 = 플러그인이 “동작한다”는 증명
2주차 = 플러그인이 “도구 구조를 갖췄다”는 증명
3주차 = 플러그인이 “포트폴리오 완성 기능을 갖췄다”는 증명
```

Phase 매핑:

```text
1주차: Phase 1-3
2주차: Phase 4-5
3주차: Phase 6-7
```

완료 단계 표현:

```text
1주차 완료
= Editor Plugin이 열리고 선택 Asset의 Dependencies Depth 1을 Tree View로 표시하는 동작 증명

2주차 완료
= Dependencies / Referencers, Max Depth, 필터, Content Browser Sync를 갖춘 Core Tool Structure

3주차 완료
= 순환 참조, Unused Candidate, Asset Size, CSV Export, README / 시연 정리가 포함된 Portfolio-ready 상태
```

이 문서에서 MVP는 문맥에 따라 다르게 보일 수 있으므로, 기능 완료 기준을 말할 때는 가능하면 `1주차 완료`, `Core Tool Structure`, `Portfolio-ready`처럼 단계 이름을 함께 쓴다.

## 작업 단위 기준

하나의 작업 단위는 다음 조건을 만족해야 한다.

- 하나의 명확한 목적이 있다.
- 변경 범위가 설명 가능하다.
- 가능하면 빌드 가능한 상태로 끝난다.
- 검증 결과 또는 미확인 항목을 남길 수 있다.
- 커밋 메시지 하나로 설명 가능하다.

## Feature 시작 규칙

주요 feature를 시작하기 전에는 작업에 바로 들어가지 않고 mini plan을 먼저 남긴다.

mini plan에는 다음 항목을 포함한다.

- 목표
- 포함 범위
- 제외 범위
- 예상 변경 파일
- 검증 방법
- 예상 커밋 단위

다만 모든 변경에 새 계획 문서를 만들지는 않는다. 작은 fix, 문서 링크 수정, 단순 formatting 변경, include 정리처럼 범위가 명확하고 위험이 낮은 작업은 작업 보고로 대체할 수 있다.

별도 mini plan이 필요한 예:

- Editor Plugin 골격 생성
- Nomad Tab / Slate UI 추가
- Asset Registry Analyzer 구현
- CSV Export 구현
- Portfolio 프로젝트 적용 시연

별도 mini plan이 불필요한 예:

- 버튼 라벨 수정
- 문서 링크 수정
- 빌드 경고 수준의 include 정리
- 오타 수정

## 검증 반복 규칙

작업은 빌드 또는 동작 확인이 가능한 작은 단위로 나눈다.

각 단위가 끝날 때 다음 중 하나를 기록한다.

- 검증 완료
- 일부 검증
- 미확인

직접 확인하지 못한 항목은 성공으로 기록하지 않고 `미확인`으로 남긴다.

검증 예:

- UE 5.4 Editor Target 빌드
- 에디터에서 플러그인 탭 오픈
- 버튼 클릭 동작
- Content Browser 선택 Asset 연동
- Tree View 렌더링
- CSV 파일 생성

## 작업 종료 문서화 체크

작업이 끝나면 문서 업데이트가 필요한지 확인한다.

문서화가 필요한 경우:

- 사용 방법이 바뀐 경우
- 빌드 방법이 바뀐 경우
- 플러그인 구조가 바뀐 경우
- UI 흐름이 바뀐 경우
- Known Limitation이 발견된 경우
- 검증 결과가 새로 생긴 경우
- 포트폴리오 설명에 쓸 만한 기능이 추가된 경우

문서화가 불필요한 경우:

- 내부 변수명만 바뀐 경우
- formatting만 수정한 경우
- 작은 include 정리
- 동작 변화 없는 주석 수정

문서 업데이트가 필요하다고 판단되면 바로 임의로 확장하지 않고, 사용자에게 업데이트 범위와 이유를 먼저 제안한다.

## Phase 0: 저장소 기준선

### 0-1. 초기 문서와 운영 규칙

목적:

- 프로젝트 목표, 작업 규칙, 빌드 기준, 문서 체계를 고정한다.

주요 변경:

- `AGENTS.md`
- `README.md`
- `BUILD.md`
- `Docs/`
- `.gitignore`

커밋 예시:

```text
docs: 플러그인 개발 초기 문서와 작업 규칙 추가
```

### 0-2. Demo Host UE 프로젝트 초기 파일

목적:

- 플러그인 개발의 기준이 되는 UE Demo Host 프로젝트 파일을 저장소에 고정한다.

주요 변경:

- `Portfolio_PlugIn.uproject`
- `Config/`
- `Source/Portfolio_PlugIn/`
- `.vsconfig`

커밋 예시:

```text
chore: Demo Host UE 프로젝트 초기 파일 추가
```

## Phase 1: 플러그인 골격

### 1-1. Editor Plugin 골격 생성

목적:

- UE 5.4에서 인식 가능한 `AssetReferenceInspector` Project Plugin을 만든다.
- Blank 기반 Editor Plugin 상태를 시작점으로 확정한다.
- 단, Runtime/Content용 Blank 기본값을 그대로 따르지 않고 `Type: Editor`, `CanContainContent: false`인 독립 Editor Tool 기준으로 유지한다.

주요 변경:

- `Plugins/AssetReferenceInspector/AssetReferenceInspector.uplugin`
- `Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/AssetReferenceInspector.Build.cs`
- `AssetReferenceInspectorModule` Startup / Shutdown

완료 기준:

- 플러그인이 프로젝트 플러그인으로 인식된다.
- Editor Target 빌드가 가능하다.

커밋 예시:

```text
feat: AssetReferenceInspector Editor Plugin 골격 추가
```

### 1-2. Demo Host 플러그인 활성화

목적:

- Demo Host `.uproject`에서 플러그인을 활성화한다.

주요 변경:

- `Portfolio_PlugIn.uproject`

완료 기준:

- 에디터 실행 시 플러그인 모듈이 로드된다.

## Phase 2: Editor Tab / Slate UI

Phase 2는 Unreal Editor의 `에디터 독립형 창` 템플릿을 참고하되, 전체 템플릿 코드를 한 번에 가져오지 않는다.

도입 순서는 다음 기준을 따른다.

1. Nomad Tab 등록과 해제
2. 기본 `SDockTab` 생성
3. 창을 열 수 있는 최소 메뉴/Command 진입점 추가
4. Slate Widget 분리

`Commands`와 `ToolMenus`는 사용자가 에디터에서 창을 열 수 있게 하는 최소 진입점이므로 Phase 2-1의 같은 feature 안에서 다룬다. `Style`과 Toolbar Button은 아이콘/툴바 편의 기능이므로 후속 작업에서 필요성을 판단한다.

### 2-1. Editor 독립형 창 최소 오픈

목적:

- 에디터 메뉴에서 `Asset Reference Inspector` 독립형 창을 열 수 있게 한다.
- 독립형 창 템플릿의 `FGlobalTabmanager`, `RegisterNomadTabSpawner`, `SDockTab`, `Commands`, `ToolMenus` 흐름을 참고한다.
- 현재 단계에서는 기본 Slate 콘텐츠만 표시하고 실제 Asset 분석 UI는 만들지 않는다.

완료 기준:

- 등록된 Tab Spawner로 기본 `SDockTab`을 생성할 수 있다.
- 메뉴 진입점을 통해 창을 열 수 있다.
- 모듈 종료 시 등록이 정상 해제된다.
- Editor Target 빌드가 성공한다.
- Style과 Toolbar Button은 아직 포함하지 않는다.

상태:

- 완료.
- `feature/ari-nomad-tab` 브랜치에서 구현했다.
- Window 메뉴에서 `Asset Reference Inspector` 탭 오픈을 수동 확인했다.
- Command, ToolMenus, TabManager, Slate 흐름은 `Docs/02_Architecture/Editor_Window_Flow_KR.md`에 정리했다.
- 다음 작업은 `SAssetReferenceInspectorWidget` 기반 Slate UI Shell 분리다.

### 2-2. Slate 기본 위젯

목적:

- 분석 기능을 붙일 기본 UI를 만든다.

주요 UI:

- Analyze 버튼
- Pick Selected Asset 버튼
- Dependencies / Referencers 모드 자리
- Max Depth 입력 자리
- Tree View 자리

상태:

- 부분 완료.
- `feature/ari-ui-shell` 브랜치에서 `SAssetReferenceInspectorWidget` 기반 기본 UI Shell을 분리했다.
- `SDockTab` 내부 UI를 모듈 cpp에서 직접 구성하지 않고 전용 Slate Widget으로 이동했다.
- 현재 UI Shell에는 제목, 선택 Asset placeholder, `Pick Selected Asset`, `Analyze`, 모드 placeholder, 결과 영역 placeholder가 표시된다.
- 실제 Max Depth 입력과 Tree View는 후속 작업에서 구현한다.
- Slate UI 기본 구조는 `Docs/02_Architecture/Slate_UI_Basics_KR.md`에 정리했다.

### 2-3. STreeView 더미 데이터

목적:

- 실제 Asset 분석 전 Tree View 렌더링과 row 구성을 검증한다.

완료 기준:

- 더미 노드가 계층 구조로 표시된다.
- expand / collapse가 동작한다.

상태:

- 완료.
- `feature/ari-tree-view-dummy` 브랜치에서 결과 영역 placeholder를 `STreeView` 기반 더미 계층 구조로 교체했다.
- 더미 데이터는 `BP_Player` 아래에 `SK_Player`, `ABP_Player`, `M_Player`, `PlayerConfig`를 배치하고, `M_Player` 아래에 `Player_D`, `Player_L`을 배치한다.
- `Portfolio_PlugInEditor` 빌드는 성공했다.
- 에디터에서 실제 Tree View 표시, 계층 노드 표시, expand / collapse UI, 선택 row 하이라이트를 수동 확인했다.

## Phase 3: Asset 선택과 Registry 조회

### 3-1. Content Browser 선택 Asset 가져오기

목적:

- 현재 선택된 Asset을 분석 대상으로 가져온다.

완료 기준:

- 선택 Asset의 이름과 PackageName을 UI에 표시한다.
- 선택 없음 상태를 안전하게 처리한다.

상태:

- 완료.
- `feature/ari-selected-asset` 브랜치에서 `Pick Selected Asset` 버튼에 Content Browser 선택 Asset 조회를 연결했다.
- 선택 Asset의 이름과 PackageName을 `Selected Asset` 텍스트에 표시하도록 구성했다.
- 선택 Asset이 없으면 `Selected Asset: None`을 표시하도록 구성했다.
- `Portfolio_PlugInEditor` 빌드는 성공했다.
- 에디터에서 `BP_Dummy` 선택 표시와 선택 없음 상태 표시를 수동 확인했다.

### 3-2. Dependencies Depth 1 조회

목적:

- Asset Registry로 선택 Asset의 1-depth Dependencies를 조회한다.

완료 기준:

- `IAssetRegistry::GetDependencies` 결과가 Tree에 표시된다.

상태:

- 완료.
- `feature/ari-dependencies-depth1` 브랜치에서 `Analyze` 버튼에 Asset Registry Dependencies 조회를 연결했다.
- `SelectedAssetData.PackageName` 기준으로 1-depth Dependencies를 조회하도록 구성했다.
- Tree 루트는 선택 Asset, 자식 노드는 조회된 Dependency Package로 표시하도록 구성했다.
- 조회 결과가 없거나 선택 Asset이 없는 상태를 안전하게 표시하도록 구성했다.
- `Portfolio_PlugInEditor` 빌드는 성공했다.
- 에디터에서 `BP_Dummy` 기준 `M_Dummy` Dependency 표시를 수동 확인했다.
- 에디터에서 `M_Dummy` 기준 `T_Dummy_Color` Dependency 표시를 수동 확인했다.
- 선택 Asset이 없는 상태에서 `Analyze` 클릭 시 안전하게 처리되는지는 검증 로그에 `미확인`으로 남겼다.

### 3-3. 분석 데이터 구조 분리

목적:

- UI와 분석 로직 사이의 데이터 구조를 분리한다.

주요 타입:

- `EAssetReferenceMode`
- `FAssetReferenceTreeNode`
- 분석 옵션 구조체

완료 기준:

- 분석 Tree 노드 타입이 UI 위젯 헤더에서 `Private/Analysis` 영역으로 분리된다.
- 분석 방향과 실행 옵션 타입이 후속 Phase 4 확장을 받을 수 있는 형태로 정의된다.
- 기존 Dependencies Depth 1 조회 결과와 Tree View 표시 동작이 유지된다.

상태:

- 완료.
- `FAssetReferenceTreeNode`를 `Private/Analysis/AssetReferenceTypes.h`로 분리했다.
- `EAssetReferenceMode`와 `FAssetReferenceAnalysisOptions`를 추가했다.
- `SAssetReferenceInspectorWidget`은 새 Analysis 타입을 include해 기존 Tree View 동작을 유지한다.
- `Portfolio_PlugInEditor` 빌드는 성공했다.
- 에디터에서 `BP_Dummy`, `M_Dummy`, `T_Dummy_Color` 기준 기존 Dependencies Depth 1 결과가 유지되는 것을 수동 확인했다.

커밋 예시:

```text
refactor: Asset Reference 분석 데이터 구조 분리
```

## Phase 4: Tree 분석 MVP

Phase 4부터는 플러그인이 단순 조회 데모를 넘어 분석 도구 구조를 갖추는 단계다. 2주차 목표의 전반부에 해당한다.

### 4-1. Max Depth 기반 재귀 Tree 생성

목적:

- Depth 제한을 가진 재귀 참조 Tree를 만든다.

완료 기준:

- Max Depth 설정이 Tree 생성에 반영된다.

상태:

- 완료.
- `FAssetReferenceAnalysisOptions::MaxDepth` 기본값을 기준으로 Dependencies Tree를 재귀 생성하도록 변경했다.
- Root는 Depth 0, 직접 Dependencies는 Depth 1, 하위 Dependencies는 Depth 2 이상으로 기록한다.
- DFS 현재 경로 기준 중복 PackageName 재확장을 막아 무한 순환을 방지한다.
- 순환 후보 노드는 실제 dependency로 Tree에 표시하되, 하위 확장은 중단하는 정책을 적용했다.
- Phase 4-1 검증을 위해 `/Game` Package만 Tree에 표시하는 최소 필터를 추가했다.
- 순환 방어 검증용 `BP_CycleA`, `BP_CycleB` Demo Asset을 추가했다.
- `Portfolio_PlugInEditor` 빌드는 성공했다.
- 에디터에서 `BP_Dummy -> M_Dummy -> T_Dummy_Color` 재귀 표시를 수동 확인했다.
- 에디터에서 `BP_CycleA -> BP_CycleB -> BP_CycleA` 표시 후 하위 확장이 중단되는 것을 수동 확인했다.
- 정식 Max Depth 입력 UI와 필터 UI는 후속 Phase에서 처리한다.

### 4-2. Referencers 모드 추가

목적:

- Dependencies와 Referencers를 전환해 분석할 수 있게 한다.

완료 기준:

- `GetDependencies`와 `GetReferencers` 경로가 같은 Tree 모델을 사용한다.

상태:

- 완료.
- `EAssetReferenceMode`를 실제 분석 분기 조건으로 사용하도록 반영했다.
- `GetRelatedPackageNames`에서 `AnalysisOptions.Mode`에 따라 `GetDependencies` / `GetReferencers`를 선택한다.
- Dependencies / Referencers 양방향 조회가 같은 Tree Node 모델과 재귀 생성 흐름을 사용한다.
- 내부 처리 함수는 Dependencies / Referencers 양방향을 포괄하기 위해 `Relation` 계열 이름으로 정리했다.
- UI에는 최소 Mode 전환 버튼을 추가했다.
- `Portfolio_PlugInEditor` 빌드는 성공했다.
- 에디터에서 `BP_Dummy` Dependencies 결과가 기존과 동일하게 유지되는 것을 수동 확인했다.
- 에디터에서 `M_Dummy` Referencers 결과에 `BP_Dummy`가 표시되는 것을 수동 확인했다.
- 에디터에서 `T_Dummy_Color` Referencers 결과에 `M_Dummy -> BP_Dummy`가 표시되는 것을 수동 확인했다.
- Max Depth 입력 UI, 필터 UI, Content Browser Sync는 후속 Phase에서 처리한다.

### 4-3. Content Browser Sync

목적:

- Tree 노드를 더블 클릭하면 Content Browser에서 해당 Asset을 선택한다.

완료 기준:

- `IContentBrowserSingleton::SyncBrowserToAssets`가 정상 동작한다.

상태:

- 완료.
- Tree Row 더블 클릭 이벤트를 `STreeView`에 연결했다.
- 더블 클릭한 노드의 PackageName으로 Asset Registry에서 `FAssetData`를 재조회한다.
- 유효한 Asset이면 `IContentBrowserSingleton::SyncBrowserToAssets`로 Content Browser 선택을 동기화한다.
- `NAME_None`, `/Script`처럼 Content Browser Asset으로 해석되지 않는 노드는 실패 없이 무시한다.
- `Portfolio_PlugInEditor` 빌드는 성공했다.
- 에디터에서 Dependencies 모드와 Referencers 모드의 `/Game` Asset 노드 더블 클릭 Sync를 수동 확인했다.
- 에디터에서 `No dependencies found` placeholder 노드 더블 클릭 시 별도 오류 없이 무시되는 것을 수동 확인했다.
- 검증을 위해 `/Game` 최소 필터를 일시 해제한 상태에서 `/Script` Package 노드 더블 클릭 시 별도 오류 없이 무시되는 것을 수동 확인했다.
- 필터링 이후 표시 결과가 비어 있는 경우의 메시지 정책은 Phase 5 필터 작업에서 함께 정리한다.

## Phase 5: 필터

Phase 5는 2주차 목표의 후반부에 해당하며, 분석 결과를 실제 프로젝트에서 다룰 수 있게 제한하고 좁히는 사용성 구조를 만든다.

### 5-1. Path 필터

목적:

- 특정 경로만 포함하거나 제외한다.

완료 기준:

- 기존 `/Game/` 고정 필터를 분석 옵션의 Path Filter 기본값으로 옮긴다.
- UI에서 Path Filter 값을 입력하고 commit할 수 있다.
- 빈 Path Filter는 전체 Package 표시로 처리한다.
- Path Filter 적용 후 표시 가능한 자식이 없으면 기존 empty relation 메시지를 표시한다.
- Editor Target 빌드가 성공한다.

상태:

- 완료.
- `feature/ari-path-filter` 브랜치에서 `FAssetReferenceAnalysisOptions::PathFilter`와 Slate 입력 UI를 추가했다.
- `ShouldIncludeRelatedPackage`가 고정 `/Game/` 대신 현재 Path Filter 값을 기준으로 PackageName을 판정하도록 변경했다.
- 필터 적용 후 표시 가능한 자식이 없는 경우 `No dependencies found` 또는 `No referencers found` placeholder를 추가하도록 정리했다.
- `OnTextCommitted`에서 Enter 후 `OnCleared`가 추가로 들어오는 사례를 확인하고, `OnCleared`가 Path Filter 값을 빈 문자열로 덮어쓰지 않도록 조치했다.
- `Portfolio_PlugInEditor` 빌드는 성공했다.
- `/Game/`, `/Script/`, 빈 Path Filter, `/NoMatch/` Path Filter 적용을 에디터에서 수동 확인했다.
- 검증 스크린샷은 `Docs/03_Verification/Screenshots/feature_ari_path_filter/`에 정리했다.

### 5-2. Asset Class 필터

목적:

- Asset Class 기준으로 결과를 제한한다.

완료 기준:

- UI에서 Asset Class Filter 값을 입력하고 commit할 수 있다.
- 빈 Asset Class Filter는 모든 Asset Class 통과로 처리한다.
- 관계 PackageName을 `FAssetData`로 해석해 `AssetClassPath` 기준으로 필터링한다.
- Path Filter와 Asset Class Filter가 함께 적용된다.
- Tree row에 Asset Class 이름을 함께 표시한다.
- Dependencies / Referencers 양쪽에서 Asset Class Filter가 적용된다.
- Editor Target 빌드가 성공한다.

상태:

- 완료.
- `feature/ari-class-filter` 브랜치에서 `FAssetReferenceAnalysisOptions::ClassFilter`와 Slate 입력 UI를 추가했다.
- 관계 노드에 `ClassName`을 저장하고 Tree row에 `DisplayName [ClassName]` 형식으로 표시하도록 정리했다.
- PackageName으로 `FAssetData`를 다시 조회하는 `TryGetPrimaryAssetDataForPackage` helper를 추가했다.
- `ShouldPassRelationFilters`에서 Path Filter와 Class Filter를 함께 적용하도록 정리했다.
- Class Filter는 `AssetClassPath`와 Class 이름을 대소문자 구분 없이 비교한다.
- 현재 필터 정책은 필터를 통과한 노드만 표시하고, 통과한 노드만 계속 재귀 탐색한다.
- 중간 노드가 필터에 걸리면 하위에 매칭되는 노드가 있어도 현재 구현에서는 표시하지 않는다.
- 향후 필터 UX 확장 시 하위 매칭을 보존하기 위해 부모 경로를 유지하고 직접 매칭되지 않은 부모를 흐리게 표시하는 방식을 검토한다.
- `Portfolio_PlugInEditor` 빌드는 성공했다.
- 빈 Class Filter, `Material`, `Texture2D`, Referencers + `Blueprint` 적용을 에디터에서 수동 확인했다.
- 검증 스크린샷은 `Docs/03_Verification/Screenshots/feature_ari_class_filter/`에 정리했다.

### 5-3. Engine / Plugin Content 옵션

목적:

- Engine Content와 Plugin Content 표시 여부를 제어한다.

### 5-4. Filter Predicate 분리

목적:

- 필터 조건을 Analyzer 내부에서 독립 함수 또는 구조로 분리한다.

완료 기준:

- Path Filter, Asset Class Filter, Engine Content, Plugin Content 판정 로직을 UI 위젯에서 분리한다.
- `SAssetReferenceInspectorWidget`은 filter predicate 세부 구현을 직접 갖지 않고 Analysis 계층의 filter API를 호출한다.
- 기존 Path / Class / Engine / Plugin Content 동작이 유지된다.

커밋 예시:

```text
refactor: Asset Reference 필터 조건 분리
```

## 2주차 마감 보완: Analysis Options UI 정리

목적:

- Phase 4-5에서 추가된 모드, Max Depth, 필터, Content Scope 옵션을 한 화면에서 읽기 쉬운 도구 UI로 정리한다.

완료 기준:

- 분석 옵션 영역을 `Mode`, `Filters`, `Content Scope`로 구분한다.
- 선택된 Dependencies / Referencers 모드가 텍스트 스타일로 구분된다.
- Max Depth 입력 UI를 제공하고 0~10 범위의 정수로 저장한다.
- Path Filter, Class Filter, Max Depth 입력이 같은 label/input 구조로 정렬된다.
- 기존 Dependencies / Referencers, Path Filter, Class Filter, Engine / Plugin Content 동작이 유지된다.
- Editor Target 빌드가 성공한다.

상태:

- 완료.
- `refactor/ari-analysis-options-ui` 브랜치에서 Analysis Options UI를 섹션 단위로 재배치했다.
- `SGridPanel` 기반 label/input 배치로 Max Depth, Path Filter, Class Filter를 정렬했다.
- Dependencies / Referencers 버튼은 선택된 모드의 `STextBlock` font를 bold로 표시하도록 변경했다.
- Max Depth 입력은 `SEditableTextBox::OnTextCommitted`에서 정수로 파싱하고, 0~10 범위로 보정한다.
- `Portfolio_PlugInEditor` 빌드는 성공했다.
- Max Depth 0, 1, 2와 Referencers 선택 상태를 에디터에서 수동 확인했다.
- 검증 스크린샷은 `Docs/03_Verification/Screenshots/refactor_ari_analysis_options_ui/`에 정리했다.

## Phase 6: 실무형 분석 기능

Phase 6부터는 3주차 목표에 해당한다. 참조 관계 조회 도구를 포트폴리오에서 설명 가능한 실무형 분석 기능으로 확장한다.

### 6-1. 순환 참조 탐지

목적:

- 전체 방문 여부가 아니라 현재 DFS 경로 기준으로 순환 참조를 탐지한다.

완료 기준:

- 같은 PackageName이 현재 경로에 다시 등장하면 `Circular`로 표시한다.
- 순환 후보 노드는 실제 관계로 Tree에 남기되 하위 확장은 중단한다.
- Tree row에는 `[Circular]` suffix를 표시한다.

진행 기록:

- `feature/ari-circular-reference-indicator` 브랜치에서 기존 `bIsCircular` 상태를 Tree row 표시 문자열에 연결했다.
- 순환 후보 노드가 `DisplayName [ClassName] [Circular]` 형식으로 표시되도록 `GetTreeNodeDisplayText`를 추가했다.

### 6-2. Asset 디스크 크기 표시

목적:

- Asset의 디스크 파일 기준 추정 크기를 표시한다.

기준:

- 1차: `.uasset`
- 가능하면 합산: `.uexp`, `.ubulk`

완료 기준:

- Tree row에 Asset 디스크 크기 suffix를 표시한다.
- 파일 경로를 찾을 수 없는 Package, `/Script` Package, placeholder 노드는 크기 표시를 생략한다.
- 순환 후보 노드에서도 크기 suffix와 `[Circular]` suffix가 함께 표시된다.

진행 기록:

- `feature/ari-asset-size-display` 브랜치에서 `FAssetReferenceTreeNode::SizeBytes`를 추가했다.
- PackageName을 local package filename으로 변환해 `.uasset`, `.uexp`, `.ubulk` 크기를 합산하는 경로를 추가했다.
- Tree row 표시 문자열을 `DisplayName [ClassName] (Size) [Circular]` 순서로 정리했다.

### 6-3. Unused Candidate 표시

목적:

- 선택 Asset 관계 Tree가 아니라 `/Game` 전체 Asset을 대상으로 참조자가 없는 검토 후보를 스캔해 표시한다.

주의:

- 삭제 가능 판정이 아니다.
- UI와 문서에서 `Unused Candidate` 표현을 사용한다.
- Soft Reference, Asset Manager, 동적 로딩은 이번 판정에서 확인하지 않는다.

완료 기준:

- `Scan Unused Candidates` 버튼을 통해 `/Game` 전체 Asset 후보 스캔을 실행한다.
- Referencer 수가 0인 Project Content Asset을 `Unused Candidates` Tree 아래에 표시한다.
- 후보 row는 `DisplayName [ClassName] (Size) [Unused Candidate]` 형식으로 표시한다.
- Path Filter와 Class Filter가 후보 스캔에도 적용된다.
- 후보가 없으면 `No unused candidates found` placeholder를 표시한다.

진행 기록:

- `feature/ari-unused-candidate-indicator` 브랜치에서 전체 `/Game` Asset 스캔 기반으로 구현했다.
- 선택 Asset의 Dependencies / Referencers child node에는 Unused Candidate 판정을 붙이지 않도록 범위를 분리했다.
- 검증용 `Content/ARI_Demo/Unused/T_Unused_Color.uasset` fixture를 추가했다.
- 전체 후보, Path Filter, Class Filter, 후보 없음 placeholder 검증 스크린샷을 추가했다.

### 6-4. Analyzer 메타데이터 정리

목적:

- 순환 참조, 크기, Unused Candidate 같은 결과 메타데이터를 일관된 구조로 정리한다.

완료 기준:

- `FAssetReferenceTreeNode`가 CSV Export에 필요한 주요 metadata를 보유한다.
- relation child node는 부모 Asset의 PackageName을 `ParentPackageName`으로 저장한다.
- root node, placeholder node, Unused Candidate 후보 node의 parent metadata 정책을 구분한다.
- Tree row 표시 문자열의 suffix 순서를 유지한다.
- CSV Export 후보 컬럼과 node metadata의 대응 관계를 문서화한다.

진행 기록:

- `refactor/ari-analysis-metadata` 브랜치에서 `FAssetReferenceTreeNode::ParentPackageName`을 추가했다.
- relation child node 생성 시 부모 node의 PackageName을 저장하도록 정리했다.
- placeholder / grouping node는 `PackageName == NAME_None` 기준으로 후속 export에서 제외 가능한 정책으로 정리했다.
- Unused Candidate 후보 node는 선택 Asset relation child가 아니므로 `ParentPackageName`을 비워 두는 정책으로 정리했다.

## Phase 7: Export / 문서 / 시연

Phase 7은 Portfolio-ready 상태를 만들기 위한 마감 단계다. 기능 결과를 저장하고, 사용법과 시연 흐름을 정리한다.

### 7-1. CSV Export

목적:

- 현재 Tree 결과를 CSV로 저장한다.

컬럼:

```text
AssetName,PackageName,Class,Path,Depth,SizeBytes,Mode,ParentPackage,IsCircular,IsUnusedCandidate
```

완료 기준:

- `Export CSV` 버튼으로 현재 Tree 결과를 CSV 파일로 저장한다.
- CSV 파일은 `Saved/AssetReferenceInspector/` 아래 timestamp 기반 파일명으로 저장한다.
- `PackageName == NAME_None`인 placeholder / grouping node는 CSV row에서 제외한다.
- Dependencies, Referencers, Unused Candidate 결과를 같은 exporter 경로로 저장한다.
- 저장 성공 또는 실패를 사용자에게 알린다.

진행 기록:

- `feature/ari-csv-export` 브랜치에서 `FAssetReferenceCsvExporter`를 추가했다.
- `TreeRootItems`를 재귀 순회해 export 가능한 Asset node만 CSV row로 변환한다.
- CSV 문자열 escape와 timestamp 기반 저장 경로를 구현했다.

### 7-2. README / 시연 문서 정리

목적:

- `Plugins/AssetReferenceInspector`만 복사해도 사용법을 이해할 수 있게 한다.
- Demo Host 루트 README에서 현재 구현 상태와 사용 흐름을 파악할 수 있게 한다.
- 실제 Portfolio 적용 전 시연 계획과 촬영 체크리스트를 정리한다.

완료 기준:

- 루트 `README.md`가 현재 구현된 기능과 사용 방법을 설명한다.
- `Plugins/AssetReferenceInspector/README.md`가 플러그인 단위 설치 / 사용 / 제한 사항을 설명한다.
- `Docs/05_Demo/Portfolio_Demo_KR.md`가 Demo Host 시연 순서와 Portfolio 적용 시연 후보를 정리한다.
- `Docs/README_KR.md` 문서 인덱스가 `05_Demo` 문서를 가리킨다.
- 실제 Portfolio 프로젝트 적용 검증은 후속 Phase로 남긴다.

상태:

- 완료.
- `feature/ari-readme-demo-docs` 브랜치에서 루트 README를 현재 구현 상태 기준으로 갱신했다.
- 플러그인 단위 README를 추가해 사용 방법, 복사 적용 흐름, 제한 사항을 정리했다.
- `Docs/05_Demo/Portfolio_Demo_KR.md`를 추가해 Demo Host 시연 순서, Portfolio 프로젝트 시연 후보, 촬영 체크리스트를 정리했다.
- 실제 Portfolio 프로젝트 복사 적용과 실사용 Asset 분석 결과 검증은 Phase 7-4로 남긴다.

### 7-3. 검증 기록

목적:

- 빌드 결과, 에디터 실행 결과, 기능별 확인 결과를 `Docs/03_Verification`에 기록한다.

### 7-4. Portfolio 적용 시연 문서

목적:

- 기존 Portfolio 프로젝트에 플러그인을 복사 적용하고 실제 Asset 분석 시나리오를 정리한다.

## 커밋 운영 기준

- `feat`: 사용자 기능 추가
- `refactor`: 동작 유지 구조 개선
- `fix`: 빌드 또는 동작 오류 수정
- `docs`: 문서 변경
- `chore`: 설정, 프로젝트 파일, 유지보수

작업 종료 시에는 다음 중 하나를 판단한다.

- 독립 커밋
- 직전 커밋 amend
- 검증 전 커밋 보류

필요한 경우 Git Bash 기준 명령을 제안한다.
