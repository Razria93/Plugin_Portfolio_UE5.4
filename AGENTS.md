# 저장소 작업 지침

## 프로젝트 범위

이 저장소는 Unreal Engine 5.4용 Editor Plugin `AssetReferenceInspector`를 개발하기 위한 Demo Host 프로젝트다.

- Demo Host 프로젝트: `Portfolio_PlugIn.uproject`
- Host 모듈: `Source/Portfolio_PlugIn/`
- 플러그인 구현: `Plugins/AssetReferenceInspector/`
- 프로젝트 문서: `Docs/`
- 로컬 초안과 GitHub 게시용 본문은 Git 추적 대상이 아닌 `LocalNotes/`에서 관리한다.

Engine 설치 경로의 `Engine/Plugins`에는 작업하지 않는다. 최종 배포 단위는 `Plugins/AssetReferenceInspector` 폴더다.

## 필수 명령

C++ / `.uplugin` / `Build.cs` / `.uproject` 변경은 다음 UE 5.4 빌드 명령으로 검증한다.

```powershell
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" Portfolio_PlugInEditor Win64 Development -Project="C:\UE5_Portfolio\Portfolio_UE5.4_verGit\Portfolio_PlugIn\Portfolio_PlugIn.uproject" -WaitMutex -FromMsBuild
```

작업 전후에는 worktree를 확인한다.

```bash
git status --short
```

`Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `.vs/` 같은 생성물은 커밋하지 않는다.

## 절대 규칙

- 사용자가 명시적으로 요청하기 전에는 commit, amend, push, PR 생성, merge, branch 삭제, GitHub Issue / PR / 댓글 원격 생성·수정을 실행하지 않는다.
- 로컬/원격 feature branch는 사용자가 명시적으로 요청하기 전에는 삭제하지 않는다. 삭제된 브랜치가 필요하면 남아 있는 커밋 SHA로 복원한다.
- `LocalNotes/`는 사용자가 명시적으로 요청하기 전에는 커밋하거나 강제 추가하지 않는다.
- MVP는 Tree View 기반이다. Asset Registry 흐름이 안정화되기 전에는 Node Graph UI를 추가하지 않는다.
- 플러그인은 Portfolio 전용 게임플레이 클래스에 의존하지 않는다. Player, Weapon, Action, Component 계열 프로젝트 전용 타입을 include하지 않는다.
- `Unused Asset`은 삭제 가능 판정이 아니라 `Unused Candidate`로만 표현한다.
- 직접 확인하지 못한 항목은 성공으로 쓰지 않고 `미확인`으로 기록한다.

## 작업 흐름

Feature 범위, mini plan 기준, branch 사용, 검증, PR 준비, Plan Issue 갱신, commit/amend/보류 판단은 `Docs/04_Governance/Feature_Workflow_KR.md`를 따른다.

주요 feature는 작업 전 mini plan을 작성한다. 위치는 `LocalNotes/mini-plans/`이며, 템플릿은 `LocalNotes/templates/mini_plan_template_KR.md`를 사용한다. 작은 fix, 링크 수정, formatting-only 변경은 짧은 작업 보고로 대체할 수 있다.

작업 종료 시에는 다음을 보고한다.

- 변경 내용
- 검증 결과 또는 `미확인` 항목
- 문서 업데이트 필요 여부
- commit, amend, 보류 중 권장 판단
- commit/amend/push가 필요할 경우 Git Bash 기준 명령

## 코드 규칙

Unreal Engine C++ 관례를 따른다.

- C++ 및 `Build.cs`는 탭 들여쓰기를 사용한다.
- 타입과 함수는 PascalCase를 사용한다.
- Unreal 접두사 `F`, `S`, `E`를 명확히 사용한다.
- Slate UI는 `Private/UI` 아래에 둔다.
- 분석 로직은 `Private/Analysis` 아래에 둔다.
- export 로직은 `Private/Export` 아래에 둔다.

C++ 변경이 있으면 작업 종료 전 `Docs/02_Architecture/Code_Organization_KR.md` 기준으로 검토한다. Public 헤더에는 외부 모듈이 실제로 필요한 타입만 노출하고, Command, Menu, ToolMenus, Slate 구현 세부 타입은 기본적으로 `Private`에 둔다.

## 문서 규칙

상세 기준은 다음 문서를 따른다.

- 문서 책임과 폴더 경계: `Docs/04_Governance/Documentation_Responsibility_KR.md`
- 초안 작성, public 변환, GitHub 게시 흐름: `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md`
- GitHub 본문 구조와 검수 기준: `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`
- 스크린샷 보관과 승격: `Docs/04_Governance/Screenshot_Workflow_KR.md`
- Feature 작업 흐름: `Docs/04_Governance/Feature_Workflow_KR.md`
- 코드 구성 규칙: `Docs/02_Architecture/Code_Organization_KR.md`

GitHub public 본문은 draft가 아니라 `LocalNotes/github/public` 파일을 기준으로 준비한다. GitHub 원격 게시 또는 수정 전에는 `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md` 기준을 수동으로 확인한다.

로컬 환경에 검증 스크립트가 있으면 다음 검증도 함께 실행한다. `LocalNotes/`는 Git 추적 대상이 아니므로 clean checkout에서는 이 명령이 없을 수 있다.

```powershell
if (Test-Path LocalNotes/tools/validate_github_public_docs.ps1) {
    powershell -ExecutionPolicy Bypass -File LocalNotes/tools/validate_github_public_docs.ps1
}
```

자동 검증 스크립트 통과는 충분조건이 아니다. PR 본문, Issue 본문, Plan 댓글, PR 댓글은 섹션 순서와 금지 요소를 수동으로도 확인한다.

원격 반영 전에는 대상 Issue / PR / 댓글, 사용할 public 파일, 실행할 `gh` 명령, 예상 변경 요약을 사용자에게 제시한다. 사용자가 승인하기 전에는 `gh` 명령을 실행하지 않는다.

Windows Codex 환경에서 `gh auth status`가 `token in keyring is invalid` 등으로 실패하면 인증 만료로 단정하지 않는다. 같은 `gh` 명령을 PowerShell + `require_escalated`로 재시도한다. 이 경로에서 성공하면 Codex 샌드박스의 Windows Credential Manager/keyring 접근 제한으로 판단한다. 세부 기준은 `Docs/04_Governance/GitHub_CLI_Execution_Workflow_KR.md`를 따른다.

## 커밋 스타일

커밋 메시지는 가벼운 Conventional Commit 형식을 사용하고 요약은 한국어로 작성한다.

- `docs: 플러그인 계획 문서 추가`
- `chore: UE 프로젝트 gitignore 추가`
- `feat: Asset Reference Inspector 탭 추가`
- `fix: Asset Registry 조회 실패 처리`
