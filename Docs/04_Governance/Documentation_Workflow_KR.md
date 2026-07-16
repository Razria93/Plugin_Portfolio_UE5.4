# Documentation Workflow

이 문서는 `AssetReferenceInspector` 개발 중 `Docs`, `LocalNotes`, GitHub Issue, PR 본문의 책임을 분리하는 기준을 정의한다.

## 기본 원칙

- `Docs`는 프로젝트 지식의 원본이다.
- `LocalNotes`는 커밋하지 않는 작업 초안이다.
- GitHub Issue는 추적 카드다.
- PR 본문은 브랜치 변경 제출 기록이다.

같은 내용을 여러 곳에 원문 복사하지 않는다. 상세한 원문은 `Docs`에 두고, Issue와 PR은 요약과 링크 중심으로 작성한다.

## 문서 트리

공개 문서는 다음 구조를 따른다.

```text
Docs/
  README_KR.md

  00_Overview/
    Project_Plan_KR.md
    MVP_Scope_KR.md

  01_Planning/
    Three_Week_Roadmap_KR.md
    Feature_Work_Plan_KR.md

  02_Architecture/
    Editor_Window_Flow_KR.md
    Code_Organization_KR.md
    Slate_UI_Basics_KR.md
    Asset_Registry_Analysis_Flow_KR.md

  03_Verification/
    README_KR.md
    Build_Verification_Log_KR.md

  04_Governance/
    Working_Rules_KR.md
    Feature_Workflow_KR.md
    Documentation_Workflow_KR.md
    Codex_Init_Prompt_KR.md
```

GitHub Issue, PR, PR 댓글, Plan Issue 댓글의 작성/게시 작업장은 `Docs`가 아니라 `LocalNotes/github`에 둔다.

```text
LocalNotes/
  mini-plans/
  github/
    draft/
    public/
    snapshots/
  notes/
  screenshots/
  templates/
```

## 문서 네이밍 규칙

문서 파일명은 폴더 카테고리와 중복되는 긴 접두사를 피하고, 문서의 역할이 드러나도록 작성한다.

기본 형식:

```text
<Subject>_<Purpose>_KR.md
```

적용 기준:

- `Docs` 전체가 `AssetReferenceInspector` 문서이므로 파일명에 `AssetReferenceInspector_`를 반복하지 않는다.
- 폴더명이 이미 범주를 제공하므로 `Architecture_`, `Planning_`, `Verification_` 같은 접두사는 붙이지 않는다.
- 한국어 문서는 `_KR.md`로 끝낸다.
- 여러 단어는 PascalCase가 아니라 `_`로 구분한다.
- GitHub 게시용 초안과 제출용 본문은 공개 Docs가 아니므로 `LocalNotes/github/`에 둔다.

예:

```text
Docs/00_Overview/Project_Plan_KR.md
Docs/01_Planning/Feature_Work_Plan_KR.md
Docs/02_Architecture/Editor_Window_Flow_KR.md
Docs/03_Verification/Build_Verification_Log_KR.md
Docs/04_Governance/Feature_Workflow_KR.md
```

## Docs

`Docs`에는 장기적으로 남길 공개 문서만 둔다.

문서 보완 순서는 다음을 기본으로 한다.

1. Plan
   - 기능 계획, MVP 범위, 작업 순서
   - 위치: `Docs/01_Planning`

2. Architecture
   - 설계 원문, 모듈 구조, UI 흐름, 코드 구성 규칙
   - 위치: `Docs/02_Architecture`

3. Verification
   - 빌드 로그, 에디터 수동 확인, 기능 검증 결과, 미확인 항목
   - 위치: `Docs/03_Verification`

저장소 운영 규칙과 작업 절차는 `Docs/04_Governance`에 둔다.

## LocalNotes

`LocalNotes`는 ignored 되는 작업장이다. 커밋하지 않는다.

포함 대상:

- feature 시작 전 mini plan
- 작업 중 체크리스트
- GitHub Issue / PR / 댓글 draft
- GitHub Issue / PR / 댓글 public 제출용 본문
- 개인 학습 메모
- 공개 문서로 승격할지 아직 결정하지 않은 정리

GitHub 게시 관련 문서는 `LocalNotes/github` 아래에서 관리한다.

```text
LocalNotes/github/
  draft/
    issues/
      plan/
      architecture/
      verification/
      plan-comments/
    prs/
      feature/
      docs/
    pr-comments/

  public/
    issues/
      plan/
      architecture/
      verification/
      plan-comments/
    prs/
      feature/
      docs/
    pr-comments/

  snapshots/
```

- `draft`: 작성 중인 가안이다.
- `public`: GitHub에 실제로 게시할 본문과 동일하게 유지하는 로컬 원문이다.
- `snapshots`: GitHub API 조회 결과, 댓글 백업, 비교용 JSON이다.

`gh` CLI 업로드에는 `LocalNotes/github/public` 파일만 사용한다.

### LocalNotes 파일명과 Metadata

`LocalNotes`는 커밋하지 않지만, 반복해서 재사용하는 작업장이므로 일정한 형식을 따른다.

파일명에는 GitHub Issue 번호, PR 번호, Phase 번호를 넣지 않는다. 번호 정보는 문서 내부 `Metadata`에 기록한다.

기본 형식:

```text
<topic>_<document_type>_KR.md
```

예:

```text
dependencies_depth1_mini_plan_KR.md
dependencies_depth1_pr_draft_KR.md
dependencies_depth1_architecture_issue_KR.md
dependencies_depth1_verification_issue_KR.md
dependencies_depth1_screenshot_comment_KR.md
```

`LocalNotes/github/draft` Markdown 문서는 상단에 `Metadata` 섹션을 둔다.

```markdown
## Metadata

- Type:
- Status:
- Branch:
- Related Phase:
- Related PR:
- Related Issues:
- Source Docs:
```

권장 Status:

```text
Draft
Ready
Posted
Superseded
Archived
```

이미 GitHub에 게시한 draft는 필요하면 삭제하거나 `Archived`로 둔다. 단, GitHub에 게시한 본문 원문은 `LocalNotes/github/public`에 보존한다.

`Metadata`는 `LocalNotes/github/draft` 전용 작업 관리 정보다. `Docs`로 승격하거나 `LocalNotes/github/public` 및 GitHub Issue / PR 본문으로 제출할 때는 제거한다.

공개 문서 또는 GitHub 본문에는 다음 항목을 포함하지 않는다.

- `# Issue Draft`
- `# PR Draft`
- `## Metadata`
- `Status: Draft`
- `Type: ...`

`LocalNotes/github/public` 본문은 GitHub에 게시했거나 게시할 본문과 동일해야 한다. 새로 작성하는 public 본문은 `## Summary`, `## Scope`, `## Verification`, `## Documentation`처럼 독자가 바로 읽을 수 있는 섹션으로 시작하는 것을 권장하되, 과거 GitHub 게시 본문이 다른 형식이면 원문을 보존한다.

### Draft에서 Public으로 변환

GitHub 게시 전에는 다음 흐름을 따른다.

1. `LocalNotes/github/draft`에서 초안을 작성한다.
2. 같은 카테고리의 최신 GitHub 공개 기록을 확인한다.
3. draft를 기반으로 `LocalNotes/github/public`에 제출용 본문을 만든다.
4. `public` 본문에서 `Metadata`, `Draft`, `공개 기록 확인`, 내부 메모를 제거한다.
5. `gh` CLI는 `public` 파일만 사용해 게시한다.
6. 게시 후 GitHub 본문과 `public` 파일이 같은지 확인한다.

직접 GitHub에서 PR/Issue/댓글 본문을 수정했거나 작업 중 불일치를 발견했다면 대응되는 `LocalNotes/github/public` 파일도 같은 내용으로 갱신한다. 최신 최종본은 GitHub 공개 기록이며, `public`은 그 본문을 로컬에서 재사용하기 위한 원문이다.

PR/Issue 제목, label, base/head, draft 여부 같은 CLI 인자는 public 본문에 넣지 않는다. 필요한 경우 draft `Metadata` 또는 `snapshots`의 별도 메모에 둔다.

과거 게시물은 대응 draft 없이 `public` 본문만 남아 있을 수 있다. `draft`는 현재 작성 중인 문서만 유지한다.

## GitHub 공개 기록 확인

GitHub에 이미 게시된 같은 카테고리의 Issue, PR 본문, 댓글은 최종 공개 기록이다. `LocalNotes` 초안과 템플릿은 게시 전 초안 소스일 뿐이며, 한 번 GitHub에 게시된 뒤에는 최종 표준으로 보지 않는다.

새 GitHub Issue, PR 본문 생성/수정, Plan Issue 누적 댓글 갱신, Architecture / Verification 공개 기록, PR 스크린샷 또는 보강 댓글을 게시하기 전에는 같은 카테고리의 GitHub 공개 기록을 먼저 확인한다.

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

### Docs 승격 전 확인

`LocalNotes` 내용을 `Docs`로 승격하기 전에는 다음을 확인한다.

- 장기 공개 문서로 남길 내용인가
- 기존 `Docs`의 같은 주제 문서와 충돌하지 않는가
- 로컬 전용 `Metadata`, `Draft` 제목, 임시 상태가 섞여 있지 않은가
- Issue / PR 본문 초안을 원문 그대로 복사하는 것이 아니라 구조, 설계, 검증 사실만 문서화하는가

### GitHub 게시 전 확인

`Docs` 또는 `LocalNotes`에서 만든 내용을 GitHub에 게시하기 전에는 다음을 확인한다.

- 같은 카테고리의 최신 GitHub Issue / PR / 댓글 형식과 섹션 순서가 맞는가
- GitHub 공개 기록에서 사용하는 용어, label, `References` / `Closes` 관계와 일치하는가
- `Docs` 링크가 현재 브랜치 또는 게시 대상 기준에서 열리는가
- `## Metadata`, `Status: Draft`, `# Issue Draft`, `# PR Draft` 같은 로컬 전용 섹션이 제거되었는가
- Plan Issue 누적 댓글은 기존 첫 댓글의 진행판 형식을 유지하는가
- PR 스크린샷이나 보강 댓글은 기존 PR 공개 기록의 설명 방식과 이미지 링크 방식을 따르는가

### 불일치 수정

초안이 같은 카테고리의 GitHub 공개 형식과 다르면 다음 순서로 보정한다.

1. GitHub에 게시된 최신 같은 카테고리 기록을 기준으로 섹션 이름, 순서, 링크 표현을 맞춘다.
2. 로컬 전용 `Metadata`와 `Draft` 상태는 제출용 본문에서 제거한다.
3. `Docs` 원문 복사 대신 GitHub용 요약과 체크리스트로 줄이고, 상세 내용은 `Docs` 링크로 연결한다.
4. 수정한 제출용 본문을 다시 같은 카테고리 공개 기록과 대조한 뒤 게시한다.

## GitHub Issue

Issue는 문서 원본 저장소가 아니라 추적 단위다.

권장 유형:

- `ARI-Pxx`: Plan
- `ARI-Axx`: Architecture
- `ARI-Vxx`: Verification
- `ARI-Lxx`: Limitation / Known Limitation

Issue 본문에는 다음을 둔다.

- Tracking ID
- 대상 브랜치 또는 범위
- 목적
- 핵심 요약
- 완료 또는 검증 기준
- 관련 `Docs` 링크

상세 설명은 `Docs`로 연결한다.

GitHub Issue를 만들거나 수정할 때는 `LocalNotes/github/public/issues`의 제출용 본문을 사용한다. GitHub Issue는 생성된 순간부터 추적 카드이므로 본문에 `Draft` 상태를 남기지 않는다.

Plan Issue 본문은 `LocalNotes/github/public/issues/plan`, Architecture Issue 본문은 `LocalNotes/github/public/issues/architecture`, Verification Issue 본문은 `LocalNotes/github/public/issues/verification`, Plan Issue 댓글은 `LocalNotes/github/public/issues/plan-comments`에 둔다.

### Plan Issue 진행 댓글

상위 `[Plan]` Issue는 MVP 전체 진행 상태를 추적한다.

Plan Issue의 첫 진행 댓글은 현재까지 완료된 Phase를 누적 요약하는 댓글로 운영한다. 새로운 feature가 끝나면 이 첫 진행 댓글의 `완료` / `진행 예정` 목록을 갱신한다.

이후 각 feature 종료 시점에는 별도 작업 로그 댓글을 추가한다.

```text
첫 진행 댓글
= 현재까지 완료된 Phase 요약과 다음 예정 작업

후속 댓글
= 각 feature 완료 시점의 작업 내용, 검증 결과, 관련 PR
```

Feature 종료 정리 과정에서는 다음을 확인한다.

- 상위 Plan Issue가 있는가
- 첫 진행 댓글의 Phase 요약을 갱신해야 하는가
- 이번 feature 완료 로그 댓글을 추가해야 하는가
- 관련 PR 번호와 Architecture / Verification Issue 번호가 연결되어 있는가
- PR merge 전후에 해당 확인을 완료했는가

## GitHub Label

GitHub Issue와 PR 라벨은 문서 책임과 같은 축으로 운영한다. 현재 커스텀 라벨 이름은 그대로 사용한다.

```text
work plan
= 기능 계획, MVP 범위, 진행 순서, 상위 추적 Issue에 사용한다.

system architecture
= 설계 구조, 코드 흐름, 모듈 책임, UI 구성 설명 Issue에 사용한다.

verification log
= 빌드, 에디터 수동 확인, 기능 검증 결과 Issue에 사용한다.
```

권장 적용 기준:

- `[Plan]` Issue에는 `work plan`을 붙인다.
- `[Architecture]` Issue에는 `system architecture`를 붙인다.
- `[Verification]` Issue에는 `verification log`를 붙인다.
- PR은 필요한 경우 변경 성격에 맞는 라벨을 붙이되, Issue 라벨을 우선 관리 단위로 본다.
- 라벨 이름은 GitHub 기본 라벨과 구분되는 현재 커스텀 이름을 유지한다. 설명이 비어 있으면 위 의미에 맞춰 GitHub label description을 채운다.

## PR

PR 본문은 해당 브랜치에서 무엇이 바뀌었는지 제출하는 기록이다.

포함 대상:

- Summary
- Key Changes
- Verification
- Unverified / Limitations
- Documentation
- Related Issues
- Next Step

PR 본문은 `LocalNotes/github/draft/prs`에서 초안을 작성하고, 게시 전 `LocalNotes/github/public/prs`의 제출용 본문으로 변환한다. PR 본문 전체를 `Docs`에 복제하지 않는다.

PR 생성 또는 수정에는 `LocalNotes/github/public/prs` 파일만 사용한다. PR이 Draft 상태인지 여부는 GitHub PR 상태로 표현하고, 본문에 `Status: Draft`를 적지 않는다.

### 스크린샷 책임

`LocalNotes/screenshots`는 임시 캡처 보관소다. PR 검증 증거로 확정한 이미지만 `Docs/03_Verification/Screenshots/<branch-name>/`로 승격해 Git에 포함한다.

스크린샷 파일명은 snake_case를 사용하고 공백을 두지 않는다.

현재 또는 미래 PR은 본문 `Screenshots` 섹션에서 Docs 이미지를 링크한다.

PR 본문에 스크린샷이 3장 이상 들어가면 소제목으로 그룹화한다. Before / After 비교가 있는 경우에는 이미지 위에 짧은 설명을 붙이고, 비교 대상별로 섹션을 나눈다.

예:

```markdown
## Screenshots

### Game Package Filter

Before: Asset Registry dependency를 그대로 재귀 표시하면 Engine / Script dependency가 함께 나타난다.

![Game Filter Before](...)

After: `/Game` Package만 표시해 Demo Asset 간 참조 관계만 남긴다.

![Game Filter After](...)

### Cycle Guard

Before: 순환 방어를 끄면 순환 참조가 MaxDepth까지 반복 전개된다.

![Cycle Guard Before](...)

After: 순환 후보 노드는 표시하되 하위 확장은 중단한다.

![Cycle Guard After](...)
```

이미 merge된 과거 PR은 본문을 재작성하지 않고 댓글로 스크린샷을 보강한다. 과거 PR 보강 댓글에는 현재 프로젝트 상태에서 해당 PR의 완료 조건을 재확인한 기록임을 명시하고, Docs에 보관된 이미지 링크를 사용한다.

### PR 상태 기준

검증 자료, 스크린샷, 문서 링크, Related Issues가 정리되기 전에는 Draft PR로 둔다.

Ready for review 전환 전에는 다음을 확인한다.

- `Verification`이 실제 검증 결과와 일치한다.
- 직접 확인하지 않은 항목은 `Unverified / Limitations` 또는 검증 로그에 `미확인`으로 남아 있다.
- 필요한 스크린샷이 PR 본문 또는 검증 문서에 연결되어 있다.
- Docs 링크가 현재 브랜치에서 열리는 파일을 가리킨다.
- Related Issues 관계가 `References` / `Closes` 기준에 맞다.

PR merge 후에는 로컬 `main`을 최신 상태로 동기화한다.

```bash
git switch main
git pull
```

## References / Closes

`References`는 관련 맥락만 연결한다. Issue를 닫지 않는다.

사용 기준:

- 상위 계획 Issue
- 여러 PR에 걸쳐 진행되는 Issue
- 일부만 관련된 Issue
- 닫으면 추적 손실이 생기는 Issue

`Closes`는 PR merge 시 Issue가 완료되어도 되는 경우에만 사용한다.

사용 기준:

- 해당 PR 하나로 Issue 목적이 완료된다.
- 남은 후속 작업이 별도 Issue로 분리되어 있다.
- Architecture / Verification Issue처럼 브랜치 단위 기록이 PR merge와 함께 완료된다.

예:

```markdown
## Related Issues

- References #2
- Closes #6
- Closes #7
```

## 번호 규칙

GitHub Issue 번호는 생성 순서에 따라 바뀐다. 프로젝트 내부 추적에는 별도 번호를 사용한다.

권장 형식:

```text
[ARI-P01][Plan] AssetReferenceInspector MVP 개발 계획
[ARI-A01][Architecture] Editor Window Open Flow
[ARI-A02][Architecture] Slate UI Shell Structure
[ARI-V01][Verification] feature/ari-nomad-tab
[ARI-V02][Verification] feature/ari-ui-shell
```

번호는 Issue 제목과 PR의 Related Issues 섹션에 우선 사용한다. 신규 문서나 큰 폭으로 수정하는 Docs에는 상단 메타데이터로 추가할 수 있다. C++ 코드 식별자나 커밋 메시지 필수 prefix로는 사용하지 않는다.

## 현재 추적 매핑

현재까지의 공개 문서와 Issue 추적 ID는 다음처럼 본다.

```text
ARI-P01: AssetReferenceInspector MVP 개발 계획
  - Docs/00_Overview/Project_Plan_KR.md
  - Docs/00_Overview/MVP_Scope_KR.md
  - GitHub Issue: #2

ARI-A01: Editor Window Open Flow
  - Docs/02_Architecture/Editor_Window_Flow_KR.md
  - Branch: feature/ari-nomad-tab

ARI-V01: feature/ari-nomad-tab Verification
  - Docs/03_Verification/Build_Verification_Log_KR.md
  - Branch: feature/ari-nomad-tab

ARI-A02: Slate UI Shell Structure
  - Docs/02_Architecture/Slate_UI_Basics_KR.md
  - Docs/02_Architecture/Code_Organization_KR.md
  - Branch: feature/ari-ui-shell
  - GitHub Issue: #6

ARI-V02: feature/ari-ui-shell Verification
  - Docs/03_Verification/Build_Verification_Log_KR.md
  - Branch: feature/ari-ui-shell
  - GitHub Issue: #7
```

## 승격 기준

`LocalNotes`의 내용은 다음 조건을 만족할 때 `Docs`로 승격한다.

- 프로젝트 구조나 설계 이해에 계속 필요하다.
- 후속 feature에서 반복 참조한다.
- 포트폴리오 설명 자료로 사용할 가치가 있다.
- 검증 결과나 제한 사항으로 추적해야 한다.

단순 Issue 본문 초안, PR 작성 초안, 개인 학습 과정은 승격하지 않는다.

