# Documentation Responsibility

이 문서는 `Docs`, `LocalNotes`, GitHub Issue, PR 본문의 책임 경계를 정의한다.

## 기본 원칙

- `Docs`는 커밋에 포함해 장기 유지할 계획, 설계, 검증 문서를 보관한다.
- `LocalNotes`는 커밋하지 않는 작업 초안과 GitHub 게시 준비본, 게시 후 로컬 mirror를 관리한다.
- GitHub Issue는 작업 범위, 설계, 검증 항목을 추적하는 단위다.
- PR 본문은 브랜치에서 변경한 내용과 검증 결과를 요약한다.

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
    Screenshots/

  04_Governance/
    Working_Rules_KR.md
    Feature_Workflow_KR.md
    Documentation_Responsibility_KR.md
    Documentation_Authoring_Workflow_KR.md
    GitHub_Public_Body_Validation_Rules_KR.md
    Screenshot_Workflow_KR.md
    Codex_Init_Prompt_KR.md
```

## Docs 책임

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

## LocalNotes 책임

`LocalNotes`는 ignored 작업장이다. 커밋하지 않는다.

포함 대상:

- feature 시작 전 mini plan
- 작업 중 체크리스트
- GitHub Issue / PR / 댓글 draft
- GitHub Issue / PR / 댓글 public 제출용 본문
- 개인 학습 메모
- 공개 문서로 승격할지 아직 결정하지 않은 정리

GitHub 게시 관련 문서는 `LocalNotes/github` 아래에서 관리한다. GitHub Issue / PR / 댓글에 실제로 게시된 본문이 공개 기준이며, `LocalNotes/github/public`은 그 본문을 준비하거나 재현하기 위한 로컬 mirror다.

```text
LocalNotes/github/
  draft/
    issues/
      architecture/
      verification/
      plan-comments/
    prs/
      feature/
      docs/
    pr-comments/

  public/
    issues/
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
- `public`: GitHub에 게시할 본문 후보이거나, 게시 후 원격 본문을 로컬에서 재현하는 mirror다.
- `snapshots`: GitHub API 조회 결과, 댓글 백업, 비교용 JSON이다.

`public`을 수정했다고 원격 GitHub 본문을 자동으로 수정하지 않는다. 원격 수정은 `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md`의 승인 절차를 거친 뒤에만 수행한다.

과거 GitHub 공개 기록은 원칙적으로 반복 수정하지 않는다. 다만 이미 편집 이력이 누적되어 있고 현재 문서 체계와 충돌하거나, 스크린샷 섹션/설명/링크 부족으로 공개 본문 이해를 방해하는 경우에는 최신 public body 기준으로 1회 정규화할 수 있다. 이때도 수정 대상 목록, public 본문, validator 결과, 실행할 `gh` 명령을 먼저 제시하고 사용자 승인을 받은 뒤 원격에 반영한다.

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

## LocalNotes Metadata

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

`Metadata`는 `LocalNotes/github/draft` 전용 작업 관리 정보다. `Docs`로 승격하거나 `LocalNotes/github/public` 및 GitHub Issue / PR 본문으로 제출할 때는 제거한다.

공개 문서 또는 GitHub 본문에는 다음 항목을 포함하지 않는다.

- `# Issue Draft`
- `# PR Draft`
- `## Metadata`
- `Status: Draft`
- `Type: ...`

## GitHub Issue 책임

Issue는 문서 원본 저장소가 아니라 추적 단위다. 상세 설명은 `Docs`로 연결하고, GitHub public 본문의 구체 섹션 순서와 문체는 `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`를 따른다.

새 Plan Issue 본문은 브랜치별로 생성하지 않는다. 계획 문서는 `Docs/01_Planning`과 `LocalNotes/mini-plans`에서 관리하고, 기존 상위 Plan Issue가 있을 때만 `LocalNotes/github/public/issues/plan-comments`에 진행 댓글 본문을 둔다.

Architecture Issue 본문은 `LocalNotes/github/public/issues/architecture`, Verification Issue 본문은 `LocalNotes/github/public/issues/verification`에 둔다.

## GitHub Label

GitHub Issue와 PR 라벨은 문서 책임과 같은 축으로 운영한다. 현재 커스텀 라벨 이름은 그대로 사용한다.

```text
work plan
= 기존 상위 추적 Issue처럼 MVP 범위와 진행 순서를 장기 추적하는 Issue에만 사용한다.

system architecture
= 설계 구조, 코드 흐름, 모듈 책임, UI 구성 설명 Issue에 사용한다.

verification log
= 빌드, 에디터 수동 확인, 기능 검증 결과 Issue에 사용한다.
```

## PR 책임

PR 본문은 해당 브랜치에서 무엇이 바뀌었는지 제출하는 기록이다.

PR 본문은 `LocalNotes/github/draft/prs`에서 초안을 작성하고, 게시 전 `LocalNotes/github/public/prs`의 제출용 본문으로 변환한다. PR 본문 전체를 `Docs`에 복제하지 않는다.

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

GitHub Issue와 PR 연결은 GitHub 번호와 `References` / `Closes` 관계로 관리한다. 별도 내부 추적 번호는 사용하지 않는다.

## 승격 기준

`LocalNotes`의 내용은 다음 조건을 만족할 때 `Docs`로 승격한다.

- 프로젝트 구조나 설계 이해에 계속 필요하다.
- 후속 feature에서 반복 참조한다.
- 포트폴리오 설명 자료로 사용할 가치가 있다.
- 검증 결과나 제한 사항으로 추적해야 한다.

단순 Issue 본문 초안, PR 작성 초안, 개인 학습 과정은 승격하지 않는다.
