# Asset Reference Inspector — UE5 에셋 참조 분석 도구

<a id="asset-reference-inspector"></a>

에셋 참조 관계를 조회하고 미사용 후보를 탐색하며, 분석 결과를 CSV로 저장하는 Unreal Engine 5.4 에디터 플러그인입니다. Content Browser에서 선택한 에셋을 기준으로 Asset Registry를 조회하고 Dependencies / Referencers 관계를 Slate Tree View로 표시합니다. 게임 런타임 기능이 아닌 에디터 도구입니다.

이 저장소는 독립 플러그인의 개발·검증용 Demo Host 프로젝트를 포함합니다.

[프로젝트 안내](https://razria93.github.io/site/plugin.html) · [프로젝트 PDF · 15쪽](https://razria93.github.io/files/pdf/plugin-review.pdf) · [영상·검증 자료](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html) · [로컬 실행](#local-run)

![개발 당시 미사용 후보 조회 화면과 원본 에셋 위치](https://razria93.github.io/site/assets/plugin.png)

*2026.07 개발 당시 화면 · 에셋 선택 없이 미사용 후보를 조회하고 원본 위치로 이동합니다.*

<a id="대표-검증-영상"></a>

## 대표 화면·시연

| 영상 | 길이 | 확인할 내용 |
| --- | --- | --- |
| [참조 관계 탐색](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#case1) | 약 1분 14초 | 공유 에셋의 경로별 표시와 순환 경로의 추가 탐색 중단 |

2026.10 별도 Data Asset 추가 QA입니다. 개발 당시 Blueprint·실제 프로젝트 전체 재실행과 구분합니다.

[전체 영상·검증 자료](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html)

<a id="프로젝트-정보"></a>

## 프로젝트 개요

| 항목 | 내용 |
| --- | --- |
| 엔진 | Unreal Engine 5.4 |
| 유형 | Editor Plugin / Demo Host Project |
| 플러그인 | `AssetReferenceInspector` |
| 목적 | 포트폴리오 / Unreal Editor Tool 제작 학습 |
| 기간 | 2026.07.13 ~ 2026.07.20 |
| 형태 | 1인 개발 · Codex 활용 |
| 기술 | C++, Unreal Editor Module, Asset Registry, Slate UI, Content Browser API |
| 범위 | Demo Host 기능 구현 · 실제 Portfolio 프로젝트 적용 기록 정리 |

기간은 포트폴리오 적용·제출 정리를 포함하며, Demo Host 기능 개발 기록은 07.19까지입니다. 검증 범위는 개발 당시 확인과 별도 추가 QA로 구분합니다.

## 작업 범위·기여

- **본인 작업:** 개발 목표·작업 단위·문서 운영 기준과 우선순위를 정하고 기능 요구를 검토했습니다. 입력 오류를 발견해 조작별 로그를 제공하고, 탐색 범위 변경을 제안했습니다.
- **Codex 작업:** 초기 계획·작업 지침 문서 작성, 코드 분석·구현·수정, 빌드와 자동 리뷰를 수행했습니다. 별도 환경에서 추가 QA와 자료 정리를 진행했습니다.

개발 당시 기록은 2026년 7월, 별도 추가 QA는 2026년 10월 21일 자료입니다. 추가 QA를 개발 당시 사용자 기여로 소급하지 않습니다. [개발 당시 확인·추가 QA·미검증 범위](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/development/verification_scope/index.html)를 구분해 제공합니다.

## 대표 구현

### 참조 관계·공유·순환 처리

Dependencies / Referencers를 조회합니다. 서로 다른 경로의 공유 에셋은 각 경로에 표시하고, 같은 경로에서 다시 만난 에셋은 순환 표시 후 하위 탐색을 중단합니다.

공유·순환 추가 QA는 별도 Data Asset의 hard reference 확인입니다. 당시 Blueprint·실제 프로젝트 전체 재실행을 뜻하지 않습니다.

[처리 구조 · PDF 13쪽](https://razria93.github.io/files/pdf/plugin-review.pdf#page=13) · [공유·순환 검증 자료](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#case1) · [참조·순환 탐색 코드](Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/Private/UI/SAssetReferenceInspectorWidget.cpp)

### 선택 없이 미사용 후보 탐색

`/Game` 및 하위 폴더에서 `GetReferencers()` 결과가 0개인 후보를 탐색하고 폴더·에셋 종류 필터를 적용합니다. 결과에서 원본 에셋 위치로 이동할 수 있습니다. **미사용 후보는 삭제 안전 판정이 아닙니다.** 표시 크기는 런타임 메모리가 아닌 디스크 파일 기준 추정값입니다.

[처리 흐름 · PDF 12쪽](https://razria93.github.io/files/pdf/plugin-review.pdf#page=12) · [개발 당시 화면·확인 기록](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#unused) · [미사용 후보 스캔 코드](Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/Private/UI/SAssetReferenceInspectorWidget.cpp)

### 분석 결과 CSV 저장

실제 에셋 행을 저장하고 안내·그룹 행은 제외합니다. 부모·깊이·모드·순환 여부 등을 기록합니다. CSV 행 수는 결과 범위 확인용이며 성능이나 고유 에셋 수를 뜻하지 않습니다.

[저장 결과 · PDF 14쪽](https://razria93.github.io/files/pdf/plugin-review.pdf#page=14) · [CSV 형식·저장 규칙](Docs/02_Architecture/CSV_Export_Format_KR.md) · [CSV 저장 코드](Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/Private/Export/AssetReferenceCsvExporter.cpp)

## 문제 해결

| 사례 | 핵심 결과 | 자료 | 영상 |
| --- | --- | --- | --- |
| 필터 입력<br>오류 | Enter 확정 후 OnCleared 호출이 저장 경로를 덮어쓰는 문제를 수정.<br>이전 처리 비교는 재구성한 Enter 조건에 한정 | [PDF 5–7쪽](https://razria93.github.io/files/pdf/plugin-review.pdf#page=5)<br>[조건·한계](#필터-입력-오류) | [입력 4조건<br>검증](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#case2) |
| 미사용 후보<br>탐색 범위 | 선택 에셋의 관계 목록에서 /Game 및 하위 폴더의 독립 탐색으로 변경.<br>개발 당시 확인이며 새 10월 추가 QA 사례가 아님 | [PDF 8–10쪽](https://razria93.github.io/files/pdf/plugin-review.pdf#page=8)<br>[당시 화면·기록](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#unused) | — |
| CSV 모드 불일치<br>AI 리뷰 | PR #59의 Codex 자동 리뷰에서 발견.<br>현재 버튼 값 대신 결과 생성 당시 분석 모드를 저장 | [PDF 15쪽](https://razria93.github.io/files/pdf/plugin-review.pdf#page=15)<br>[조건·한계](#csv-저장모드-불일치) | [모드 전환<br>검증](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#case3) |

**CSV 비교 범위:** 개발 당시 빌드와 별도 Data Asset QA를 구분하며, 과거 전체 빌드 비교가 아닙니다. 입력·모드 전환 영상 페이지에서 대응 CSV도 확인할 수 있습니다.

## 검증 조건·한계

### 참조 관계·공유·순환 처리

- **공유·순환 추가 QA:** 별도 Data Asset의 hard reference로 확인했습니다. 당시 Blueprint·실제 프로젝트 재실행과 크래시·성능 실측은 수행하지 않았습니다.

[공유·순환 검증 자료](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#case1)

### 미사용 후보 탐색·탐색 범위 재설계

- **미사용 후보:** 삭제 안전 판정이 아닙니다. Soft Reference·Asset Manager·동적 로딩 기반 사용 여부는 검증하지 않았습니다.
- 개발 당시 조회·필터·원본 위치 이동 확인. 새 10월 추가 QA를 수행한 사례가 아님.

### 필터 입력 오류

- 사용자 오류·로그 제공과 수정 후 Escape 확인, Codex 분석·수정 및 별도 입력 4조건 QA.
- **필터 비교 조건:** 필터의 이전 처리 비교는 이전 입력 콜백을 재구성한 Enter 조건에 한정하며 과거 전체 빌드를 실행한 비교가 아닙니다.

### CSV 저장·모드 불일치

- 개발 당시 수정 코드 빌드 확인과 10월 별도 Data Asset 양방향 모드 전환 QA를 구분. 과거 전체 빌드 비교가 아님.
- **CSV 수량 해석:** CSV 행 수는 결과 범위 확인용으로 성능이나 고유 에셋 수를 뜻하지 않습니다.

[화면·CSV 대응](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/index.html#csv-export)

### 공통 해석 범위

- **표시 크기:** 디스크 파일 기준 추정값이며 런타임 메모리 사용량이 아닙니다.
- **그 밖의 미검증 범위:** 다른 UE 버전·대규모 성능·모든 참조 유형의 완전성은 검증하지 않았습니다.

세부 조건과 사례별 사용자·Codex 작업은 연결된 검증 범위 문서와 PDF에 유지합니다.

## 관련 자료

- [통합 PDF · 플러그인 25–38쪽](https://razria93.github.io/files/pdf/combined-review.pdf#page=25)
- [전체 검증 범위](https://razria93.github.io/site/details/Plugin_Portfolio_UE5.4/portfolio-evidence/development/verification_scope/index.html)
- [플러그인 README](Plugins/AssetReferenceInspector/README.md) · [문서 인덱스](Docs/README_KR.md)

<a id="빌드사용-방법"></a>
<a id="local-run"></a>

## 로컬 실행

### 저장소·환경 준비

Unreal Engine 5.4와 Visual Studio C++ 개발 환경을 준비하고 이 저장소를 받습니다.

```powershell
git clone https://github.com/Razria93/Plugin_Portfolio_UE5.4.git
cd Plugin_Portfolio_UE5.4
```

### 에디터에서 실행

- **Demo Host에서 확인:** 아래 명령으로 `Portfolio_PlugInEditor`를 빌드하고 `Portfolio_PlugIn.uproject`를 열어 기본 사용 흐름을 확인합니다.
- **기존 프로젝트에 적용:** 이 저장소의 `Plugins/AssetReferenceInspector` 폴더를 대상 프로젝트의 `Plugins` 폴더로 복사합니다. 프로젝트를 다시 열고 필요하면 Editor Target을 빌드합니다. [플러그인 적용 안내](Plugins/AssetReferenceInspector/README.md#다른-프로젝트-적용)에서 전체 절차를 확인합니다.

[빌드 안내](BUILD.md)에서 준비 조건을 확인합니다. PowerShell에서 저장소 루트 기준으로 실행합니다. 엔진 경로는 설치 위치에 맞게 변경합니다.

```powershell
$engineRoot = 'C:\Program Files\Epic Games\UE_5.4'
$projectPath = (Resolve-Path -LiteralPath '.\Portfolio_PlugIn.uproject').Path
& "$engineRoot\Engine\Build\BatchFiles\Build.bat" Portfolio_PlugInEditor Win64 Development "-Project=$projectPath" -WaitMutex -FromMsBuild
```

<a id="기본-사용-흐름"></a>

### 실행 후 확인

1. `Window > Asset Reference Inspector`에서 창을 열고 에셋을 선택해 참조 관계를 분석합니다.
2. 결과 트리를 확인하고 원본 에셋 위치 이동과 CSV 저장을 확인합니다.
3. 미사용 후보는 `Scan Unused Candidate`로 별도 스캔합니다.

<details>
<summary>상세 조작 순서 · 필터·모드·CSV 저장 위치</summary>

1. Unreal Editor에서 `Window > Asset Reference Inspector`를 엽니다.
2. Content Browser에서 분석할 Asset을 선택합니다.
3. `Pick Selected Asset`을 클릭합니다.
4. Dependencies 또는 Referencers 모드를 선택합니다.
5. 필요하면 Max Depth, Path Filter, Class Filter, Engine / Plugin Content 표시 옵션을 조정합니다.
6. `Analyze`를 클릭합니다.
7. Tree View에서 참조 관계를 확인합니다.
8. Tree 노드를 더블 클릭해 Content Browser Sync를 수행합니다.
9. 필요하면 `Export CSV`로 현재 결과를 저장합니다. 저장 위치는 프로젝트의 `Saved/AssetReferenceInspector` 폴더입니다.

Unused Candidate 후보는 `Scan Unused Candidate`로 별도 스캔합니다.

</details>

[준비·빌드 상세](BUILD.md) · [기능별 사용·프로젝트 적용 안내](Plugins/AssetReferenceInspector/README.md)

## 저장소 구조

| 경로 | 역할 |
| --- | --- |
| [Plugins/AssetReferenceInspector](Plugins/AssetReferenceInspector/) | 독립 에디터 플러그인과 사용 안내 |
| [플러그인 Source](Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/) | Slate UI·참조 탐색·CSV 구현 |
| [Source/Portfolio_PlugIn](Source/Portfolio_PlugIn/) | 개발·검증용 Demo Host |
| [Portfolio_PlugIn.uproject](Portfolio_PlugIn.uproject) | Demo Host 실행 진입점 |
| [Docs](Docs/README_KR.md) | 계획·구조·검증·시연 문서 색인 |
| [BUILD.md](BUILD.md) | 환경 준비와 빌드 안내 |

실제 플러그인 구현은 `Plugins/AssetReferenceInspector` 아래에 둡니다. Engine 설치 경로의 `Engine/Plugins`에는 작업하지 않습니다.

## 개발 원칙

- Project Plugin 구조를 유지하고 Portfolio 전용 게임플레이 타입에 의존하지 않습니다.
- Asset Registry 기반 `FAssetData`와 PackageName 중심으로 분석합니다.
- MVP는 Slate Tree View 기반입니다.
- Node Graph UI는 Future Work로 둡니다.
- `Unused Asset`은 삭제 가능 판정이 아니라 `Unused Candidate`로만 표현합니다.
- Asset Size는 런타임 메모리 크기가 아니라 에디터 프로젝트의 디스크 파일 크기 기준 추정값입니다.

<details>
<summary>전체 기능·개발 문서·개발 당시 검증 자료</summary>

### 주요 기능

Demo Host의 구현 기능입니다. 기능별 검증 조건은 개발 당시 검증 자료와 별도 QA 기록을 확인합니다.

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

### 개발 문서

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

### 개발 당시 검증 자료

- [Dependencies Depth 1](Docs/03_Verification/Screenshots/feature_ari_dependencies_depth1/bp_dummy_dependencies.png)
- [Referencers Mode](Docs/03_Verification/Screenshots/feature_ari_referencers_mode/referencers_t_dummy_color.png)
- [Content Browser Sync](Docs/03_Verification/Screenshots/feature_ari_content_browser_sync/sync_m_dummy_from_dependencies.png)
- [CSV Export](Docs/03_Verification/Screenshots/feature_ari_csv_export/csv_export_dependencies_success.png)
- [Unused Candidate](Docs/03_Verification/Screenshots/feature_ari_unused_candidate_indicator/unused_candidates_all_project_content.png)

### Portfolio 적용 흐름

최종 시연 단계에서는 `Plugins/AssetReferenceInspector` 폴더를 기존 Portfolio 프로젝트의 `Plugins` 폴더로 복사해 적용합니다.

적용 후 Character, Weapon, Material, Texture 등 실사용 Asset의 Dependencies / Referencers 관계를 분석하고, Tree View와 CSV 결과를 시연 자료로 정리합니다.

실제 Portfolio 프로젝트 적용 결과는 [포트폴리오 시연 문서](Docs/05_Demo/Portfolio_Demo_KR.md)에 정리합니다.

</details>

<details>
<summary>이전 제출본 보존</summary>

- [2026-10-23 제출 PDF · v75 / 15쪽](https://razria93.github.io/Plugin_Portfolio_UE5.4/portfolio/files/plugin-portfolio-v75.pdf?v=20261023-nav1)
- [기존 개별 자료 안내 · PPTX 다운로드](https://razria93.github.io/Plugin_Portfolio_UE5.4/)

</details>
