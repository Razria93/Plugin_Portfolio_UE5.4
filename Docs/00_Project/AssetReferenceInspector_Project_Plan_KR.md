# AssetReferenceInspector 프로젝트 계획

## 목적

`AssetReferenceInspector`는 Unreal Engine 5.4 기준의 독립 Editor Plugin이다.

Content Browser에서 선택한 Asset을 기준으로 Asset Registry를 조회하여 Dependencies와 Referencers 관계를 Tree View로 표시하고, 포트폴리오 프로젝트의 실제 Asset 참조 구조를 분석하는 것을 목표로 한다.

## 개발 위치

이 저장소는 플러그인 개발용 Demo Host 프로젝트다.

```text
Portfolio_PlugIn/
  Portfolio_PlugIn.uproject
  Plugins/
    AssetReferenceInspector/
      AssetReferenceInspector.uplugin
      Source/
        AssetReferenceInspector/
```

플러그인은 Engine 설치 경로의 `Engine/Plugins`가 아니라 Demo Host 프로젝트 루트 아래의 `Plugins/AssetReferenceInspector`에서 개발한다.

최종 시연 단계에서는 `Plugins/AssetReferenceInspector` 폴더를 기존 Portfolio 프로젝트의 `Plugins` 폴더로 복사하여 적용한다.

## 목표

- UE 5.4에서 빌드 가능한 Editor Plugin을 만든다.
- 본 Portfolio 프로젝트 코드에 의존하지 않는 독립 플러그인으로 유지한다.
- Asset Registry 기반으로 Asset Dependencies와 Referencers를 조회한다.
- Slate 기반 Tree View UI로 분석 결과를 표시한다.
- 선택 Asset 기준 분석 흐름을 안정화한 뒤 필터, CSV Export, 순환 참조 표시를 추가한다.
- 최종적으로 실제 Portfolio 프로젝트에 적용 가능한 산출물로 정리한다.

## 비목표

- 게임 런타임 기능을 만들지 않는다.
- 본 Portfolio 프로젝트의 `CPlayer`, `CWeaponComponent`, `CActionComponent` 같은 전용 타입을 include하지 않는다.
- 처음부터 Node Graph UI를 구현하지 않는다.
- Asset 삭제 가능 여부를 단정하지 않는다.
- 코드 동적 로딩, 문자열 기반 로딩, Asset Manager 규칙까지 완전하게 추적한다고 주장하지 않는다.

## 핵심 기능

- Content Browser 선택 Asset 가져오기
- Dependencies / Referencers 모드 전환
- Max Depth 제한
- Asset Class 필터
- Path 필터
- Engine Content / Plugin Content 표시 여부 옵션
- 순환 참조 탐지
- Unused Candidate 표시
- Asset 크기 표시
- CSV Export
- Tree 노드 더블 클릭 시 Content Browser에서 해당 Asset 선택

## 예시 분석 대상

```text
BP_Player
  SK_Player
  ABP_Player
  M_Player
    Player_D
    Player_L
  PlayerConfig
```

## 주요 Unreal API

- `FAssetRegistryModule`
- `IAssetRegistry`
- `FAssetData`
- `FName PackageName`
- `IAssetRegistry::GetDependencies`
- `IAssetRegistry::GetReferencers`
- `FContentBrowserModule`
- `IContentBrowserSingleton::SyncBrowserToAssets`
- Slate `STreeView`

## 권장 코드 구조

```text
Plugins/
  AssetReferenceInspector/
    AssetReferenceInspector.uplugin
    Source/
      AssetReferenceInspector/
        AssetReferenceInspector.Build.cs
        Public/
        Private/
          AssetReferenceInspectorModule.cpp
          UI/
            SAssetReferenceInspectorWidget.h
            SAssetReferenceInspectorWidget.cpp
          Analysis/
            AssetReferenceAnalyzer.h
            AssetReferenceAnalyzer.cpp
            AssetReferenceNode.h
            AssetReferenceTypes.h
          Export/
            AssetReferenceCsvExporter.h
            AssetReferenceCsvExporter.cpp
```

## 주요 책임

### AssetReferenceInspectorModule

- 플러그인 시작/종료 처리
- Editor 메뉴 또는 Window 탭 등록
- Nomad Tab Spawner 등록
- Slate 위젯 생성

### SAssetReferenceInspectorWidget

- Slate UI 구성
- Analyze 버튼
- Pick Selected Asset 버튼
- Dependencies / Referencers 전환 UI
- Max Depth 입력
- 필터 UI
- CSV Export 버튼
- `STreeView` 결과 표시
- Tree 노드 더블 클릭 처리

### AssetReferenceAnalyzer

- Asset Registry 접근
- Dependencies / Referencers 조회
- Depth 제한
- 필터 적용
- Tree Node 생성
- 현재 DFS 경로 기준 순환 참조 탐지
- Unused Candidate 판정
- Asset 디스크 크기 계산

### AssetReferenceCsvExporter

- 분석 결과를 CSV로 저장
- CSV 컬럼 순서와 escaping 규칙 관리

## 데이터 구조 초안

```cpp
enum class EAssetReferenceMode
{
    Dependencies,
    Referencers
};

struct FAssetReferenceNode
{
    FAssetData AssetData;
    FName PackageName;

    int32 Depth = 0;
    int64 SizeBytes = 0;

    bool bIsCircular = false;
    bool bIsUnusedCandidate = false;

    TArray<TSharedPtr<FAssetReferenceNode>> Children;
};
```

## 분석 흐름

1. Content Browser에서 선택된 Asset을 가져온다.
2. `FAssetData.PackageName`을 확보한다.
3. 현재 분석 모드를 확인한다.
   - Dependencies
   - Referencers
4. Max Depth를 확인한다.
5. Asset Registry를 조회한다.
6. Asset Class, Path, Engine/Plugin Content 필터를 적용한다.
7. Tree Node를 생성한다.
8. 현재 DFS 경로 기준으로 순환 참조를 표시한다.
9. Asset 디스크 크기 추정값을 계산한다.
10. Tree View를 갱신한다.

## 제한 사항

- Asset Registry 기반 분석은 코드에서 동적으로 로딩하는 Asset을 모두 보장하지 않는다.
- Soft reference, 문자열 기반 경로, Asset Manager 규칙은 별도 정책 없이는 누락될 수 있다.
- Unused Candidate는 삭제 가능 판정이 아니라 검토 후보 표시다.
- Asset Size는 런타임 메모리 사용량이 아니라 디스크 파일 기준 추정값이다.
- Node Graph UI는 MVP 이후 Future Work로 둔다.

## 포트폴리오 메시지

> Unreal Engine의 Asset Registry와 Slate UI를 활용해, 선택한 Asset의 Dependencies / Referencers를 분석하고 Tree View로 시각화하는 독립 Editor Plugin을 제작했습니다. Demo Host 프로젝트에서 개발한 뒤 실제 액션 RPG Portfolio 프로젝트에 적용하여 BP_Player, Character, Weapon, Material 등 실사용 Asset 참조 관계를 분석했습니다.

