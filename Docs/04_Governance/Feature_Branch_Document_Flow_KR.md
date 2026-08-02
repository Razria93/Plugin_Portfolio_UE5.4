# Feature Branch Document Flow

이 문서는 `AssetReferenceInspector` 개발 과정에서 사용한 feature branch 단위 작업 흐름과 문서 산출물의 역할을 정리한다.

목적은 브랜치가 시작되고 마감될 때까지 어떤 문서를 참고하고, 어떤 파일을 만들고, 그 파일이 어느 시점에 GitHub Issue / PR / Docs로 이어지는지 설명하는 것이다.

## 1. 작업 시작: 범위 확정

브랜치를 시작하기 전에 전체 계획과 현재 Phase 위치를 먼저 확인한다.

참고 문서:

- `Docs/00_Overview/Project_Plan_KR.md`
- `Docs/00_Overview/MVP_Scope_KR.md`
- `Docs/01_Planning/Three_Week_Roadmap_KR.md`
- `Docs/01_Planning/Feature_Work_Plan_KR.md`
- `Docs/04_Governance/Feature_Workflow_KR.md`

이 단계에서 확정하는 내용:

- 이번 브랜치가 어떤 Phase에 속하는지
- 이번 작업의 완료 기준
- 이번 브랜치에서 제외할 일
- 빌드, 에디터 수동 검증, 스크린샷 필요 여부

산출 문서:

- `LocalNotes/mini-plans/<feature_name>_mini_plan_KR.md`

예시:

- `LocalNotes/mini-plans/path_filter_mini_plan_KR.md`
- `LocalNotes/mini-plans/csv_export_mini_plan_KR.md`
- `LocalNotes/mini-plans/portfolio_demo_verification_mini_plan_KR.md`

## 2. 코드 수정 전: 구조 파악

코드를 바로 수정하지 않고, 먼저 수정될 영역과 책임을 나눈다.

참고 문서:

- `Docs/02_Architecture/Code_Organization_KR.md`
- `Docs/02_Architecture/Editor_Window_Flow_KR.md`
- `Docs/02_Architecture/Asset_Registry_Analysis_Flow_KR.md`
- `Docs/02_Architecture/Slate_UI_Basics_KR.md`
- `Docs/02_Architecture/Slate_Widget_API_Catalog_KR.md`

검토 기준:

- UI 코드는 `Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/Private/UI`
- 분석 로직은 `Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/Private/Analysis`
- CSV export 로직은 `Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/Private/Export`
- Public 헤더에 노출할 타입인지, Private 구현 세부인지
- Slate callback, attribute binding, helper API 섹션 배치가 적절한지
- 함수명이 기존 네이밍과 일관되는지

## 3. 구현: 검증 가능한 단위로 수정

기능은 에디터에서 확인 가능한 단위로 나누어 구현한다.

실제 구현 순서 예시:

- Nomad Tab 열기
- UI Shell 구성
- 선택 Asset 표시
- Dependencies 조회
- Referencers 전환
- Path / Class / Engine / Plugin 필터
- Max Depth
- 순환 참조 표시
- Asset Size
- Unused Candidate
- CSV Export
- Portfolio 프로젝트 적용 검증

구현 중 구조 판단이 필요한 내용은 Architecture 문서로 정리한다.

대표 문서:

- `Docs/02_Architecture/Asset_Registry_Analysis_Flow_KR.md`
- `Docs/02_Architecture/CSV_Export_Format_KR.md`
- `Docs/02_Architecture/Slate_UI_Basics_KR.md`

## 4. 검증: 빌드, 에디터 확인, 스크린샷 기록

C++ 변경은 UE 빌드로 확인한다.

```powershell
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" Portfolio_PlugInEditor Win64 Development -Project="C:\UE5_Portfolio\Portfolio_UE5.4_verGit\Portfolio_PlugIn\Portfolio_PlugIn.uproject" -WaitMutex -FromMsBuild
```

검증 결과는 다음 문서에 누적한다.

- `Docs/03_Verification/Build_Verification_Log_KR.md`
- `Docs/03_Verification/Final_Verification_Summary_KR.md`

스크린샷은 먼저 로컬에 저장한다.

- `LocalNotes/screenshots/`

검증에 사용할 스크린샷은 Docs로 승격한다.

- `Docs/03_Verification/Screenshots/<feature_name>/`

CSV 결과물은 export 검증 자료로 보관한다.

- `Docs/03_Verification/Exports/feature_ari_csv_export/`
- `Docs/03_Verification/Exports/portfolio_demo_verification/`

스크린샷 운영 기준은 `Docs/04_Governance/Screenshot_Workflow_KR.md`를 따른다.

## 5. 문서화: 공개 문서와 로컬 게시 본문 분리

기능 구현이 끝나면 공개 Docs에 반영할 내용과 GitHub 게시용 본문을 나눈다.

공개 문서:

- `Docs/01_Planning/`
- `Docs/02_Architecture/`
- `Docs/03_Verification/`
- `Docs/04_Governance/`
- `Docs/05_Demo/`

GitHub 게시용 로컬 본문:

- `LocalNotes/github/public/prs/feature/`
- `LocalNotes/github/public/prs/refactor/`
- `LocalNotes/github/public/issues/architecture/`
- `LocalNotes/github/public/issues/verification/`
- `LocalNotes/github/public/issues/plan-comments/`

문서 책임 기준:

- `Docs/04_Governance/Documentation_Responsibility_KR.md`
- `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md`
- `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`

## 6. PR 준비: GitHub 본문 생성

PR 생성 전에는 GitHub에 바로 쓰지 않고, 먼저 `LocalNotes/github/public` 아래에 본문을 준비한다.

PR 본문 예시:

- `LocalNotes/github/public/prs/feature/path_filter_pr_body_KR.md`
- `LocalNotes/github/public/prs/feature/csv_export_pr_body_KR.md`
- `LocalNotes/github/public/prs/feature/portfolio_demo_verification_pr_body_KR.md`

Architecture Issue 본문 예시:

- `LocalNotes/github/public/issues/architecture/path_filter_architecture_issue_KR.md`

Verification Issue 본문 예시:

- `LocalNotes/github/public/issues/verification/path_filter_verification_issue_KR.md`

Plan Issue 댓글 예시:

- `LocalNotes/github/public/issues/plan-comments/path_filter_plan_comment_KR.md`
- `LocalNotes/github/public/issues/plan-comments/plan_progress_summary_comment_KR.md`

검증 도구:

- `LocalNotes/tools/validate_github_public_docs.ps1`

이 단계에서 확인하는 내용:

- 섹션 순서
- 스크린샷 제목과 설명
- 존댓말 / draft 문구 제거
- `References` / `Closes` 관계
- Plan Issue 댓글 갱신 여부

## 7. PR 마감: 리뷰 대응, 머지, Plan Issue 갱신

PR을 만들고 나면 다음 순서로 마감한다.

- Draft PR 생성
- Architecture / Verification Issue 연결
- 스크린샷 링크 확인
- Ready for Review 전환
- 리뷰 코멘트 대응
- 일반 merge
- `main`으로 전환 후 pull
- 상위 Plan Issue 첫 댓글 갱신
- feature 완료 댓글 추가

참고 문서:

- `Docs/04_Governance/GitHub_CLI_Execution_Workflow_KR.md`
- `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`
- `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md`

## 8. 최종 마감: Demo / 제출용 문서 정리

플러그인 기능 구현 후에는 실제 Portfolio 프로젝트 적용 결과를 별도 Demo 문서로 정리한다.

최종 문서:

- `Docs/05_Demo/Portfolio_Demo_KR.md`
- `Docs/05_Demo/Portfolio_Submission_Summary_KR.md`
- `Docs/03_Verification/Final_Verification_Summary_KR.md`

최종 검증 자료:

- `Docs/03_Verification/Screenshots/portfolio_demo_verification/`
- `Docs/03_Verification/Exports/portfolio_demo_verification/`

이 단계의 목적은 단순히 기능 구현을 완료했다는 사실이 아니라, 다음 내용을 보여주는 것이다.

- Demo Host에서 독립 플러그인으로 개발
- 실제 Portfolio 프로젝트에 적용
- 실제 Asset 관계 분석 가능
- 빌드, UI, CSV, Content Browser Sync 검증
- 작업 과정과 판단 근거를 문서로 보존

## 문서 흐름 요약

```text
Docs/
= 공개 가능한 계획, 설계, 검증, Demo 문서

LocalNotes/mini-plans/
= 브랜치 시작 전 작업 계획

LocalNotes/screenshots/
= 검증 스크린샷 원본 보관

Docs/03_Verification/Screenshots/
= 공개 검증 자료로 승격된 스크린샷

LocalNotes/github/public/
= GitHub Issue / PR / 댓글에 게시할 본문 후보 또는 게시 후 로컬 사본

Docs/04_Governance/
= 위 흐름을 일관되게 유지하기 위한 작업 규칙
```
