# Codex Init Prompt

새 Codex 세션에서 이 저장소를 다룰 때 아래 내용을 초기 지침으로 사용한다.

```text
현재 세션은 Unreal Engine 5.4 Editor Plugin 개발 세션입니다.

프로젝트 목적:
- Demo Host 프로젝트 안에서 독립 Project Plugin `AssetReferenceInspector`를 개발합니다.
- 작업 위치는 프로젝트 루트의 `Plugins/AssetReferenceInspector`입니다.
- Engine 설치 경로의 `Engine/Plugins`에는 작업하지 않습니다.
- 최종 산출물은 기존 Portfolio 프로젝트의 `Plugins` 폴더로 복사 가능한 독립 Editor Plugin입니다.

초기 확인 규칙:
- 작업 시작 전 cwd, git branch/status, `.uproject`, `Plugins/`, `Source/`, `Config/`, `Docs/`, `AGENTS.md`를 확인합니다.
- 플러그인 코드 작성 전 `.uplugin`, `Build.cs`, 모듈 타입, Editor/Runtime 분리 여부를 확인합니다.
- 사용자가 명시적으로 요청하기 전에는 commit, push, PR 생성, merge를 실행하지 않습니다.

작업 원칙:
- 본 Portfolio 프로젝트 전용 클래스에 의존하지 않습니다.
- 분석 로직은 Asset Registry, `FAssetData`, `PackageName` 기반으로 작성합니다.
- MVP는 Slate Tree View 기반입니다.
- Node Graph UI는 Future Work로 둡니다.
- "Unused Asset"은 삭제 가능 판정이 아니라 `Unused Candidate`로만 표현합니다.
- Asset Size는 런타임 메모리가 아니라 디스크 파일 기준 추정값으로 표현합니다.

Feature 작업 규칙:
- 주요 feature는 작업 전 `LocalNotes/mini-plans/`에 mini plan을 작성한 뒤 진행합니다.
- Feature 범위는 내부 구현 조각이 아니라 사용자가 설명 가능한 기능 단위로 잡고, 내부 API 등록이나 의존성 추가는 feature 안의 구현 단계로 다룹니다.
- mini plan에는 목표, 범위, 예상 변경 파일, 검증 방법, 완료 기준, 예상 커밋 단위를 포함합니다.
- 작은 fix, 문서 링크 수정, formatting 변경은 mini plan 없이 작업 보고로 대체할 수 있습니다.
- 여러 파일에 걸친 feature나 refactor는 `feature/<short-name>` 브랜치를 권장하지만 모든 작업에 브랜치를 강제하지 않습니다.
- feature 브랜치 작업은 PR을 통해 main에 반영하는 것을 기본 흐름으로 보고, 작업 종료 시 PR summary를 제안합니다.
- 작업 종료 시 문서 업데이트 필요 여부와 commit/amend/보류 여부를 판단하고, Git Bash 기준 명령만 제안합니다.
- C++ 코드 변경이 있으면 작업 종료 전에 `Docs/02_Architecture/Code_Organization_KR.md` 기준으로 Public/Private 배치, include 순서, 헤더 섹션, 의존성 범위를 검토합니다.
- feature 종료 시 상위 `[Plan]` Issue가 있으면 첫 진행 댓글 갱신과 feature 완료 로그 댓글 추가 여부를 확인합니다.
- 상위 Plan Issue 댓글은 PR 스크린샷 댓글 템플릿을 재사용하지 않고, `LocalNotes/templates/github_draft_plan_progress_summary_comment_template_KR.md` 또는 `LocalNotes/templates/github_draft_plan_feature_comment_template_KR.md`를 사용합니다.
- GitHub 게시용 Issue / PR / 댓글은 `LocalNotes/github/draft`에서 초안을 작성한 뒤 `LocalNotes/github/public` 본문으로 변환하고, `gh` CLI에는 public 파일만 사용합니다.
- GitHub public 본문은 `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`의 문서 타입별 섹션 순서, 문체, 스크린샷, 금지 요소 기준을 따릅니다.
- 사용자가 명시적으로 요청하기 전에는 commit, amend, push, PR 생성, merge를 실행하지 않습니다.

빌드/검증 규칙:
- 가능한 경우 UE 5.4 `Portfolio_PlugInEditor Win64 Development` 빌드를 수행합니다.
- 직접 빌드/실행하지 못한 항목은 반드시 `미확인`으로 기록합니다.
- 빌드 에러는 첫 번째 원인 에러를 우선 분석합니다.
- 에디터 UI 작업은 탭 오픈, 버튼 동작, Content Browser 연동 여부를 검증 기준으로 둡니다.

문서 규칙:
- 루트 `README.md`는 저장소 대표 문서로 사용합니다.
- `Docs/`는 계획, 설계, 검증, 시연 문서를 관리합니다.
- 플러그인 구현이 안정화되면 `Plugins/AssetReferenceInspector/README.md`를 별도로 작성합니다.
- Known Limitations를 숨기지 않습니다.

작업 보고 형식:
작업 시작 시 현재 repo/git 상태, 확인한 UE/plugin 구조, 작업 계획, 위험 요소를 요약합니다.
작업 종료 시 변경 파일, 핵심 변경 내용, 빌드/검증 결과, 미확인 항목, 다음 작업을 요약합니다.
```
