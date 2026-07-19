# AssetReferenceInspector Demo Host

Unreal Engine 5.4용 Editor Plugin `AssetReferenceInspector`를 개발하고 검증하기 위한 Demo Host 프로젝트다.

`AssetReferenceInspector`는 Content Browser에서 선택한 Asset을 기준으로 Asset Registry를 조회하고, Dependencies / Referencers 관계를 Slate Tree View에 표시하는 에디터 확장 도구다. 게임 런타임 기능이 아니라 에디터에서 Asset 참조 구조를 분석하고 포트폴리오 프로젝트에 적용하는 것을 목표로 한다.

## 프로젝트 정보

- 엔진: Unreal Engine 5.4
- 유형: Editor Plugin / Demo Host Project
- 플러그인: `AssetReferenceInspector`
- 개발 목적: 포트폴리오 / Unreal Editor Tool 제작 학습
- 개발 기간: 2026.07.13 ~ 2026.07.19
- 개발 상태: 개인 개발
- 주요 기술: C++, Unreal Editor Module, Asset Registry, Slate UI, Content Browser API
- 현재 단계: Demo Host 기능 구현 및 검증 완료, Portfolio 프로젝트 적용 예정

## 현재 상태

현재 Demo Host 기준으로 다음 기능을 구현하고 검증했다.

- Editor Window 메뉴 기반 Nomad Tab
- Slate 기반 분석 UI와 `STreeView` 결과 표시
- Content Browser 선택 Asset 가져오기
- Dependencies / Referencers 모드 전환
- Max Depth 기반 재귀 Tree 생성
- Path / Asset Class 필터
- Engine / Plugin Content 표시 옵션
- Tree 노드 더블 클릭 시 Content Browser Sync
- 순환 참조 표시
- Asset 디스크 크기 표시
- Unused Candidate 전체 스캔
- CSV Export

## 빠른 링크

- [작업 지침](AGENTS.md)
- [빌드 가이드](BUILD.md)
- [문서 인덱스](Docs/README_KR.md)
- [프로젝트 계획](Docs/00_Overview/Project_Plan_KR.md)
- [MVP 범위](Docs/00_Overview/MVP_Scope_KR.md)
- [Feature 작업 계획](Docs/01_Planning/Feature_Work_Plan_KR.md)
- [검증 로그](Docs/03_Verification/Build_Verification_Log_KR.md)
- [플러그인 README](Plugins/AssetReferenceInspector/README.md)
- [포트폴리오 시연 계획](Docs/05_Demo/Portfolio_Demo_KR.md)

## 저장소 구조

```text
Portfolio_PlugIn/
  Portfolio_PlugIn.uproject
  Source/Portfolio_PlugIn/
  Plugins/
    AssetReferenceInspector/
      AssetReferenceInspector.uplugin
      Source/AssetReferenceInspector/
  Docs/
```

실제 플러그인 구현은 `Plugins/AssetReferenceInspector` 아래에 둔다. Engine 설치 경로의 `Engine/Plugins`에는 작업하지 않는다.

## 빌드

PowerShell에서 저장소 루트 기준으로 실행한다.

```powershell
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" Portfolio_PlugInEditor Win64 Development -Project="C:\UE5_Portfolio\Portfolio_UE5.4_verGit\Portfolio_PlugIn\Portfolio_PlugIn.uproject" -WaitMutex -FromMsBuild
```

## 기본 사용 흐름

1. Unreal Editor에서 `Window > Asset Reference Inspector`를 연다.
2. Content Browser에서 분석할 Asset을 선택한다.
3. `Pick Selected Asset`을 클릭한다.
4. Dependencies 또는 Referencers 모드를 선택한다.
5. 필요하면 Max Depth, Path Filter, Class Filter, Engine / Plugin Content 표시 옵션을 조정한다.
6. `Analyze`를 클릭한다.
7. Tree View에서 참조 관계를 확인한다.
8. Tree 노드를 더블 클릭해 Content Browser Sync를 수행한다.
9. 필요하면 `Export CSV`로 현재 결과를 저장한다.

Unused Candidate 후보는 `Scan Unused Candidate`로 별도 스캔한다.

## 개발 원칙

- Project Plugin 구조를 유지하고 Portfolio 전용 게임플레이 타입에 의존하지 않는다.
- Asset Registry 기반 `FAssetData`와 PackageName 중심으로 분석한다.
- MVP는 Slate Tree View 기반이다.
- Node Graph UI는 Future Work로 둔다.
- `Unused Asset`은 삭제 가능 판정이 아니라 `Unused Candidate`로만 표현한다.
- Asset Size는 런타임 메모리 크기가 아니라 에디터 프로젝트의 디스크 파일 크기 기준 추정값이다.

## 검증 자료

- [Build Verification Log](Docs/03_Verification/Build_Verification_Log_KR.md)
- [Dependencies Depth 1](Docs/03_Verification/Screenshots/feature_ari_dependencies_depth1/bp_dummy_dependencies.png)
- [Referencers Mode](Docs/03_Verification/Screenshots/feature_ari_referencers_mode/referencers_t_dummy_color.png)
- [Content Browser Sync](Docs/03_Verification/Screenshots/feature_ari_content_browser_sync/sync_m_dummy_from_dependencies.png)
- [CSV Export](Docs/03_Verification/Screenshots/feature_ari_csv_export/csv_export_dependencies_success.png)
- [Unused Candidate](Docs/03_Verification/Screenshots/feature_ari_unused_candidate_indicator/unused_candidates_all_project_content.png)

## Portfolio 적용 흐름

최종 시연 단계에서는 `Plugins/AssetReferenceInspector` 폴더를 기존 Portfolio 프로젝트의 `Plugins` 폴더로 복사해 적용한다.

적용 후 Character, Weapon, Material, Texture 등 실사용 Asset의 Dependencies / Referencers 관계를 분석하고, Tree View와 CSV 결과를 시연 자료로 정리한다.
