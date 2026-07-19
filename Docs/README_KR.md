# AssetReferenceInspector 문서 인덱스

이 폴더는 `AssetReferenceInspector` Editor Plugin 개발을 위한 공개 문서를 관리한다.

현재 프로젝트는 플러그인 개발용 Demo Host 프로젝트이며, 실제 플러그인 구현은 프로젝트 루트의 `Plugins/AssetReferenceInspector` 아래에서 진행한다.

## 00_Overview

프로젝트의 목적, 범위, MVP 기준처럼 "무엇을 왜 만드는가"를 설명하는 문서다.

- `00_Overview/Project_Plan_KR.md`
  - 프로젝트 목표, 개발 원칙, 주요 기능, 산출물 기준을 정리한다.
- `00_Overview/MVP_Scope_KR.md`
  - MVP 범위와 확장 범위를 분리한다.

## 01_Planning

일정과 feature 단위처럼 "어떤 순서와 단위로 만들 것인가"를 정리하는 문서다.

- `01_Planning/Three_Week_Roadmap_KR.md`
  - 3주 개발 일정을 단계별 산출물 기준으로 정리한다.
- `01_Planning/Feature_Work_Plan_KR.md`
  - 기능, 리팩토링, 검증 단위를 실제 작업 기준으로 분해한다.

## 02_Architecture

플러그인 구조, Unreal Editor 확장 흐름, Slate UI, 코드 배치 규칙처럼 설계 이해에 필요한 문서다.

- `02_Architecture/Editor_Window_Flow_KR.md`
  - 에디터 Window 메뉴에서 독립형 Nomad Tab을 여는 Command, Menu, TabManager, Slate 흐름을 정리한다.
- `02_Architecture/Code_Organization_KR.md`
  - 플러그인 C++ 코드의 Public/Private 폴더, include 순서, 헤더 섹션 구성 규칙을 정리한다.
- `02_Architecture/Slate_UI_Basics_KR.md`
  - 플러그인 UI에 사용하는 Slate 위젯, Slot, 배치 옵션, 현재 UI Shell 구조를 정리한다.
- `02_Architecture/Slate_Widget_API_Catalog_KR.md`
  - Slate 위젯과 주요 API를 생성 문법, 표시, 입력, 컨테이너, 테이블, 에디터 도킹, 이벤트 기준으로 정리한다.
- `02_Architecture/Asset_Registry_Analysis_Flow_KR.md`
  - Asset Registry 기반 Dependencies 조회와 Tree View 표시 흐름을 정리한다.
- `02_Architecture/CSV_Export_Format_KR.md`
  - CSV Export row / field 구분자, escaping, 컬럼 매핑, 저장 위치를 정리한다.

## 03_Verification

빌드, 에디터 실행, 기능별 확인 결과와 검증 스크린샷을 기록하는 문서다. 직접 확인하지 못한 항목은 성공으로 쓰지 않고 `미확인`으로 남긴다.

- `03_Verification/README_KR.md`
  - 검증 기록 폴더의 역할과 문서 목록을 정리한다.
- `03_Verification/Final_Verification_Summary_KR.md`
  - Demo Host 기준 최종 기능 검증 결과, 대표 스크린샷, 남은 제한을 요약한다.
- `03_Verification/Build_Verification_Log_KR.md`
  - UE 5.4 빌드와 에디터 UI 확인 결과를 기록한다.
- `03_Verification/Screenshots/`
  - PR 또는 검증 로그에 사용할 최종 검증 스크린샷을 보관한다.
- `03_Verification/Exports/`
  - CSV Export처럼 파일 생성 기능을 검증하기 위한 최종 export 샘플을 보관한다.

## 04_Governance

저장소 운영 규칙, 작업 절차, 문서 책임 분리, Codex 세션 지침처럼 "어떻게 일할 것인가"를 정리하는 문서다.

- `04_Governance/Working_Rules_KR.md`
  - 저장소에서 플러그인을 개발할 때 지켜야 할 기본 작업 규칙을 정리한다.
- `04_Governance/Feature_Workflow_KR.md`
  - feature 시작, mini plan 작성, 브랜치 판단, 검증, 문서화, 커밋 판단 기준을 정리한다.
- `04_Governance/Documentation_Responsibility_KR.md`
  - `Docs`, `LocalNotes`, GitHub Issue, PR 본문의 책임 경계와 문서 네이밍 규칙을 정리한다.
- `04_Governance/Documentation_Authoring_Workflow_KR.md`
  - draft 작성, public 변환, GitHub 게시 전 검수, 원격 반영 승인 절차를 정리한다.
- `04_Governance/GitHub_CLI_Execution_Workflow_KR.md`
  - GitHub CLI 실행 경로, keyring 오류 진단, `require_escalated` 재시도 기준을 정리한다.
- `04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`
  - GitHub PR 본문, Issue 본문, 댓글 public 본문의 섹션 순서, 스크린샷 구조, 금지 요소 검사 기준을 정리한다.
- `04_Governance/Screenshot_Workflow_KR.md`
  - 검증 스크린샷 보관, 승격, 파일명, GitHub 이미지 링크 기준을 정리한다.
- `04_Governance/Codex_Init_Prompt_KR.md`
  - 새 Codex 세션에서 사용할 초기 작업 지침을 정리한다.

## 05_Demo

Demo Host에서 구현한 플러그인을 실제 Portfolio 프로젝트에 적용하고 시연하기 위한 흐름을 정리하는 문서다.

- `05_Demo/Portfolio_Demo_KR.md`
  - Portfolio 적용 준비, Demo Host 시연 순서, 실제 Portfolio 프로젝트 시연 후보, 촬영 체크리스트를 정리한다.

## LocalNotes

`LocalNotes/`는 버전 관리 대상이 아닌 로컬 자료 영역이다. mini plan, GitHub 게시용 초안/게시 본문, 학습 메모처럼 공개 문서로 커밋하지 않을 자료를 둔다.

- `LocalNotes/mini-plans/`
  - feature 시작 전 mini plan을 둔다.
- `LocalNotes/github/`
  - GitHub Architecture / Verification Issue, PR, PR 댓글, 상위 Plan Issue 댓글의 draft와 public 본문을 둔다.
  - `gh` CLI 업로드에는 `LocalNotes/github/public` 파일만 사용한다.
- `LocalNotes/screenshots/`
  - Docs로 승격하기 전 임시 스크린샷을 둔다.
- `LocalNotes/templates/`
  - LocalNotes 문서 작성에 사용하는 템플릿을 둔다.
