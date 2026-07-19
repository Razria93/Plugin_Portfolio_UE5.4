# GitHub CLI Execution Workflow

이 문서는 Codex가 GitHub CLI `gh`를 실행할 때의 인증 오류 진단, sandbox 제한 대응, 원격 명령 실행 기준을 정의한다.

## 책임 범위

이 문서는 GitHub CLI 실행 환경과 오류 대응만 다룬다.

- `gh` 인증 상태 확인
- Codex sandbox에서 발생하는 keyring 오류 판단
- `require_escalated` 재시도 기준
- Git Bash 우회, 재로그인, 토큰 주입 같은 대응의 사용 여부
- GitHub 원격 변경 명령 실행 전 승인 기준과의 연결

GitHub 게시 본문 작성, draft/public 변환, 검증 스크립트 검수, 원격 반영 대상 보고 절차는 `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md`를 따른다.

## 기본 원칙

Windows Codex 환경에서 `gh auth status`가 실패하더라도 사용자 인증 만료로 단정하지 않는다.

Codex 기본 PowerShell은 sandbox 안에서 실행되며, Windows Credential Manager/keyring 또는 Git Bash/MSYS 런타임 리소스에 제한적으로 접근할 수 있다. 따라서 기본 PowerShell에서 실패한 `gh` 명령이 샌드박스 밖 실행에서는 정상 동작할 수 있다.

## 반복 발생한 문제

이 프로젝트에서는 같은 유형의 GitHub CLI 문제가 반복 발생했다.

- 사용자 Git Bash에서는 `gh auth status`가 정상으로 표시되었다.
- Codex 기본 PowerShell에서는 `The token in keyring is invalid`가 표시되었다.
- 같은 `gh auth status`를 `require_escalated`로 실행하면 정상으로 표시되었다.
- Codex 기본 PowerShell에서 Git Bash를 직접 호출하면 `CreateFileMapping ..., Win32 error 5`가 발생했다.
- `%APPDATA%\GitHub CLI\hosts.yml`에는 token 필드가 없었고, GitHub CLI가 Windows keyring을 사용하는 형태였다.

이 조합은 GitHub 토큰 만료보다 Codex sandbox의 Windows keyring 접근 제한으로 보는 것이 맞다.

## 진단 기준

다음 증상이 나오면 사용자 인증 문제로 단정하지 않는다.

- `gh auth status`가 `The token in keyring is invalid` 또는 유사한 keyring 오류로 실패한다.
- 사용자 Git Bash 또는 샌드박스 밖 실행에서는 `gh auth status`가 정상이다.
- Codex 기본 PowerShell에서 Git Bash를 직접 호출할 때 `CreateFileMapping ..., Win32 error 5`가 발생한다.
- `%APPDATA%\GitHub CLI\hosts.yml`에 token 필드가 없다.

`hosts.yml`에 token 필드가 없는 것은 Windows keyring 저장 방식에서는 정상일 수 있다.

## 실행 순서

GitHub CLI 명령이 keyring 오류로 실패하면 다음 순서를 따른다.

1. 기본 PowerShell에서 실패한 `gh` 명령과 오류 메시지를 확인한다.
2. 같은 `gh` 명령을 PowerShell + `require_escalated`로 재실행한다.
3. `require_escalated`에서 성공하면 Codex sandbox의 keyring 접근 제한으로 판단한다.
4. 이후 동일 작업의 GitHub CLI 명령은 PowerShell + `require_escalated` 경로로 실행한다.
5. 그래도 실패하면 원격 작업을 중단하고 오류 메시지, 실행한 명령, 필요한 사용자 조치를 보고한다.

## 금지 대응

다음 대응은 기본 해결책으로 사용하지 않는다.

- 사용자에게 `gh auth login` 또는 `gh auth logout`을 먼저 요구한다.
- Codex 내부에서 Git Bash 호출로 우회한다.
- `GH_TOKEN` 또는 `GITHUB_TOKEN` 환경변수로 토큰을 주입한다.
- `gh auth token`처럼 토큰 값을 출력할 수 있는 명령을 진단용으로 실행한다.

`GH_TOKEN` 방식은 사용자가 명시적으로 승인한 임시 최소 권한 토큰을 사용할 때만 예외적으로 고려한다.

## 원격 변경 명령 승인 관계

GitHub CLI 실행 경로가 결정되어도 원격 변경 명령은 별도 승인 없이 실행하지 않는다.

승인 없이는 다음 명령을 실행하지 않는다.

- `gh issue create`
- `gh issue edit`
- `gh issue comment`
- `gh pr create`
- `gh pr edit`
- `gh pr merge`
- `gh api graphql updateIssueComment`

원격 게시 또는 수정 전에는 대상 Issue / PR / 댓글, 사용할 public 파일, 실행할 `gh` 명령, 예상 변경 요약을 사용자에게 제시한다.

## 관련 문서

- `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md`
- `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`
- `Docs/04_Governance/Documentation_Responsibility_KR.md`
