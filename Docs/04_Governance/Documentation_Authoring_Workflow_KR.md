# Documentation Authoring Workflow

이 문서는 문서 초안 작성, Docs 승격, GitHub public 본문 작성과 원격 게시 승인 절차를 정의한다.

## 기본 흐름

GitHub 게시 본문은 다음 순서를 따른다.

1. `LocalNotes/github/draft`에서 템플릿으로 초안을 작성한다.
2. 같은 카테고리의 최신 GitHub 공개 기록을 확인한다.
3. draft를 기반으로 `LocalNotes/github/public`에 제출용 본문을 만든다.
4. `public` 본문에서 `Metadata`, `Draft`, `공개 기록 확인`, 내부 메모를 제거한다.
5. 스크린샷이 있으면 `Docs/04_Governance/Screenshot_Workflow_KR.md` 기준으로 이미지 경로와 URL을 확인한다.
6. `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md` 기준으로 public 본문을 수동 검수한다.
7. `LocalNotes/tools/validate_github_public_docs.ps1`로 자동 검증 가능한 범위를 검사한다.
8. 반영 대상 파일, 대상 Issue / PR / 댓글, 실행할 `gh` 명령을 목록으로 정리한다.
9. 사용자가 해당 목록 그대로 원격 반영을 승인한 뒤에만 `gh` CLI를 실행한다.
10. 게시 후 GitHub 본문과 `public` 파일이 같은지 확인한다.

## 템플릿 사용

작성 시작점은 `LocalNotes/templates`다.

- mini plan: `LocalNotes/templates/mini_plan_template_KR.md`
- PR body draft: `LocalNotes/templates/github_draft_pr_template_KR.md`
- Architecture Issue draft: `LocalNotes/templates/github_draft_architecture_issue_template_KR.md`
- Verification Issue draft: `LocalNotes/templates/github_draft_verification_issue_template_KR.md`
- Plan progress comment draft: `LocalNotes/templates/github_draft_plan_progress_summary_comment_template_KR.md`
- Plan feature comment draft: `LocalNotes/templates/github_draft_plan_feature_comment_template_KR.md`
- PR screenshot/comment draft: `LocalNotes/templates/github_draft_pr_comment_template_KR.md`

템플릿은 draft 전용이다. GitHub에 게시할 때는 `LocalNotes/github/public` 본문을 사용한다.

## Docs 승격 전 확인

`LocalNotes` 내용을 `Docs`로 승격하기 전에는 다음을 확인한다.

- 장기 공개 문서로 남길 내용인가
- 기존 `Docs`의 같은 주제 문서와 충돌하지 않는가
- 로컬 전용 `Metadata`, `Draft` 제목, 임시 상태가 섞여 있지 않은가
- Issue / PR 본문 초안을 원문 그대로 복사하는 것이 아니라 구조, 설계, 검증 사실만 문서화하는가

## GitHub 공개 기록 확인

GitHub에 이미 게시된 같은 카테고리의 Issue, PR 본문, 댓글은 공개 기준이다. `LocalNotes` 초안과 템플릿은 게시 전 초안 소스이며, `LocalNotes/github/public`은 게시 후보 또는 게시 후 로컬 mirror다.

단, 과거 공개 기록의 섹션 순서, 문서 경로, 링크 표현이 현재 검수 규칙과 충돌하면 새 public 본문은 `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`와 현재 `Docs` 경로를 우선한다.

과거 공개 기록은 원칙적으로 반복 수정하지 않는다. 그러나 이미 편집 이력이 누적되어 있고 현재 문서 체계와 충돌하거나, 스크린샷 섹션/설명/링크 부족으로 공개 본문 이해를 방해하는 경우에는 최신 public body 기준으로 1회 정규화할 수 있다.

새 GitHub Architecture / Verification Issue, PR 본문 생성/수정, Plan Issue 누적 댓글 갱신, PR 스크린샷 또는 보강 댓글을 게시하기 전에는 같은 카테고리의 GitHub 공개 기록을 먼저 확인한다.

필수 확인 대상:

- 신규 Issue 생성
- PR 본문 생성 또는 업데이트
- Plan Issue 첫 누적 진행 댓글 업데이트
- Architecture / Verification Issue 본문 작성
- PR 스크린샷 섹션 또는 PR 보강 댓글 작성

생략 가능 대상:

- 단순 오타 수정
- 링크 경로만 고치는 변경
- `LocalNotes` 안에서만 남기는 메모
- GitHub에 게시하지 않는 작업 보고

## Public 본문 작성

`LocalNotes/github/public` 본문은 GitHub에 게시할 본문 후보이거나, 게시 후 원격 본문을 로컬에서 재현하는 mirror다.

GitHub 원격 본문이 이미 존재하면 원격이 공개 기준이다. `public` 파일을 수정해도 원격은 자동으로 바뀌지 않으며, 원격 반영 여부는 별도 승인 절차에서 결정한다.

`public` 본문에는 다음을 넣지 않는다.

- draft용 `Metadata`
- `Status: Draft`
- 공개 기록 확인용 체크리스트
- 내부 작업 메모
- PR title, label, base/head, draft 여부 같은 CLI 인자

PR/Issue 제목, label, base/head, draft 여부 같은 CLI 인자는 public 본문에 넣지 않는다. 필요한 경우 draft `Metadata` 또는 `snapshots`의 별도 메모에 둔다.

## GitHub 게시 전 확인

`Docs` 또는 `LocalNotes`에서 만든 내용을 GitHub에 게시하기 전에는 다음을 확인한다.

- 같은 카테고리의 최신 GitHub Issue / PR / 댓글 형식과 섹션 순서가 맞는가
- GitHub 공개 기록에서 사용하는 용어, label, `References` / `Closes` 관계와 일치하는가
- `Docs` 링크가 현재 브랜치 또는 게시 대상 기준에서 열리는가
- public 본문이 `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md` 검수 기준을 만족하는가
- 스크린샷이 있으면 `Docs/04_Governance/Screenshot_Workflow_KR.md` 기준을 만족하는가

## 원격 반영 승인 절차

원격 GitHub 수정은 별도 승인 단계로 분리한다. 에이전트는 로컬 수정과 검증을 마친 뒤 다음 정보를 먼저 보고한다.

- 수정할 GitHub 대상: Issue 번호, PR 번호, 댓글 URL 또는 댓글 ID
- 사용할 로컬 public 파일
- 실행할 `gh` 명령
- 예상 변경 요약

사용자가 이 목록을 승인하기 전에는 원격 본문을 생성하거나 수정하지 않는다.

validator의 error는 원격 반영 전 수정한다. warning은 예외 가능성이 있는 스타일 판단이므로, 사용자가 판단할 수 있도록 작업 보고에 남긴다.

승인 없이는 다음 명령을 실행하지 않는다.

- `gh issue create`
- `gh issue edit`
- `gh issue comment`
- `gh pr create`
- `gh pr edit`
- `gh pr merge`
- `gh api graphql updateIssueComment`

## 과거 공개 기록 정규화

과거 Issue / PR / 댓글 본문을 최신 기준으로 정규화할 때는 다음 순서를 따른다.

1. 대상 원격 본문을 확인한다.
2. 대응되는 `LocalNotes/github/public` 파일을 최신 public body 기준으로 수정한다.
3. `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md` 기준으로 수동 검수한다.
4. `LocalNotes/tools/validate_github_public_docs.ps1`로 자동 검증 가능한 범위를 확인한다.
5. 수정할 원격 대상, 사용할 public 파일, 실행할 `gh` 명령, 변경 요약을 사용자에게 제시한다.
6. 사용자가 승인한 뒤 원격 본문을 1회 반영한다.
7. 게시 후 GitHub 원격 본문과 `public` mirror가 일치하는지 확인한다.

정규화 목적은 현재 보이는 공개 본문의 이해 가능성과 양식 일관성을 회복하는 것이다. 정규화 이후에는 같은 과거 본문을 양식 이유만으로 반복 수정하지 않는다.

## 게시 후 일치 확인

직접 GitHub에서 PR/Issue/댓글 본문을 수정했거나 작업 중 불일치를 발견했다면 대응되는 `LocalNotes/github/public` 파일도 같은 내용으로 갱신한다.

최신 최종본은 GitHub 공개 기록이며, `public`은 그 본문을 로컬에서 재사용하기 위한 mirror다.

## 불일치 수정

초안이 같은 카테고리의 GitHub 공개 형식과 다르면 다음 순서로 보정한다.

1. GitHub에 게시된 최신 같은 카테고리 기록을 기준으로 섹션 이름, 순서, 링크 표현을 맞춘다.
2. 로컬 전용 `Metadata`와 `Draft` 상태는 제출용 본문에서 제거한다.
3. `Docs` 원문 복사 대신 GitHub용 요약과 체크리스트로 줄이고, 상세 내용은 `Docs` 링크로 연결한다.
4. 수정한 제출용 본문을 다시 같은 카테고리 공개 기록과 대조한 뒤 게시한다.

## PR 게시 전 상태 확인

PR을 생성하거나 Ready for review로 전환하기 전에는 `Docs/04_Governance/Feature_Workflow_KR.md`의 PR 준비 기준을 확인한다.

이 문서에서는 PR 상태 자체를 판단하지 않고, PR 본문을 draft에서 public으로 변환하고 GitHub에 게시하기 전의 작성/검수/승인 절차만 다룬다.
