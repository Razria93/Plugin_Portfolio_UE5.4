# AssetReferenceInspector Demo Host

Unreal Engine 5.4용 Editor Plugin `AssetReferenceInspector`를 개발하고 검증하기 위한 Demo Host 프로젝트다.

`AssetReferenceInspector`는 Content Browser에서 선택한 Asset을 기준으로 Asset Registry를 조회하고, Dependencies / Referencers 관계를 Slate Tree View에 표시하는 에디터 확장 도구다. 게임 런타임 기능이 아니라 에디터에서 Asset 참조 구조를 분석하고 포트폴리오 프로젝트에 적용하는 것을 목표로 한다.

## 제출 포트폴리오 · 먼저 보기

UE 5.4에서 에셋 참조 관계를 조회하고 미사용 후보를 찾으며, 결과를 CSV로 저장하는 1인 개발 프로젝트입니다. 초기 계획·작업 지침부터 구현·리뷰까지 Codex를 활용했으며, 사용자의 판단·오류 발견·검증 기여와 Codex 작업을 PDF에 구분했습니다.

- [포트폴리오 PDF · v75 / 15쪽](https://razria93.github.io/Plugin_Portfolio_UE5.4/portfolio/files/plugin-portfolio-v75.pdf?v=20261023-nav1)
- [공개 자료 안내 · PPTX 다운로드](https://razria93.github.io/Plugin_Portfolio_UE5.4/)
- [영상·검증 자료 · PDF 순서별 요약](https://razria93.github.io/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html)
- [개발 당시 검증 / 추가 QA / 미검증 범위](https://razria93.github.io/Plugin_Portfolio_UE5.4/portfolio-evidence/development/verification_scope/index.html)

PDF에서 프로젝트 소개 → 문제 해결·검증 → 프로젝트 구현·검증 순으로 읽고, 필요한 근거만 아래에서 확인할 수 있습니다. 공개 근거 페이지마다 해당 PDF 페이지로 돌아가는 링크가 있습니다.

| PDF | 자료 | 확인할 내용 |
| --- | --- | --- |
| 5–7쪽 | [필터 입력 오류](https://razria93.github.io/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#case2) | Enter 후 필터가 풀리던 원인·수정과 입력 4조건 추가 QA |
| 8–10쪽 | [미사용 후보 탐색 재설계](https://razria93.github.io/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#unused) | 선택 에셋의 관계 검사에서 프로젝트 콘텐츠 탐색으로 전환한 판단·결과 |
| 13쪽 | [공유·순환 관계 처리](https://razria93.github.io/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#case1) | 개발 당시 Blueprint 전후 화면과 별도 Data Asset 추가 QA |
| 14쪽 | [CSV 저장 기능](https://razria93.github.io/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#csv-export) | 실제 분석 화면과 저장한 CSV의 대응 |
| 15쪽 | [AI 리뷰 활용](https://razria93.github.io/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#case3) | PR 리뷰 → 수정 커밋 → 이후 모드 전환 QA |

개발 당시 기록은 2026년 7월, 별도 추가 QA는 2026년 10월 21일 자료입니다. 추가 QA를 개발 당시 사용자 기여로 소급하지 않습니다. 미사용 후보는 삭제 안전 판정이 아니며, 다른 UE 버전·대규모 성능·모든 참조 유형은 검증하지 않았습니다.

## 프로젝트 정보

- 엔진: Unreal Engine 5.4
- 유형: Editor Plugin / Demo Host Project
- 플러그인: `AssetReferenceInspector`
- 개발 목적: 포트폴리오 / Unreal Editor Tool 제작 학습
- 개발 기간: 2026.07.13 ~ 2026.07.20 (포트폴리오 적용·제출 정리 포함; Demo Host 기능 개발 기록은 07.19까지)
- 개발 형태: 1인 개발 · Codex 활용
- 주요 기술: C++, Unreal Editor Module, Asset Registry, Slate UI, Content Browser API
- 현재 단계: Demo Host 기능 구현 및 검증 완료, Portfolio 프로젝트 적용 시연 기록 완료

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
- [최종 검증 요약](Docs/03_Verification/Final_Verification_Summary_KR.md)
- [검증 로그](Docs/03_Verification/Build_Verification_Log_KR.md)
- [플러그인 README](Plugins/AssetReferenceInspector/README.md)
- [포트폴리오 시연 계획](Docs/05_Demo/Portfolio_Demo_KR.md)
- [포트폴리오 제출 요약](Docs/05_Demo/Portfolio_Submission_Summary_KR.md)

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

- [Final Verification Summary](Docs/03_Verification/Final_Verification_Summary_KR.md)
- [Build Verification Log](Docs/03_Verification/Build_Verification_Log_KR.md)
- [Dependencies Depth 1](Docs/03_Verification/Screenshots/feature_ari_dependencies_depth1/bp_dummy_dependencies.png)
- [Referencers Mode](Docs/03_Verification/Screenshots/feature_ari_referencers_mode/referencers_t_dummy_color.png)
- [Content Browser Sync](Docs/03_Verification/Screenshots/feature_ari_content_browser_sync/sync_m_dummy_from_dependencies.png)
- [CSV Export](Docs/03_Verification/Screenshots/feature_ari_csv_export/csv_export_dependencies_success.png)
- [Unused Candidate](Docs/03_Verification/Screenshots/feature_ari_unused_candidate_indicator/unused_candidates_all_project_content.png)

## Portfolio 적용 흐름

최종 시연 단계에서는 `Plugins/AssetReferenceInspector` 폴더를 기존 Portfolio 프로젝트의 `Plugins` 폴더로 복사해 적용한다.

적용 후 Character, Weapon, Material, Texture 등 실사용 Asset의 Dependencies / Referencers 관계를 분석하고, Tree View와 CSV 결과를 시연 자료로 정리한다.

실제 Portfolio 프로젝트 적용 결과는 [포트폴리오 시연 문서](Docs/05_Demo/Portfolio_Demo_KR.md)에 정리한다.
