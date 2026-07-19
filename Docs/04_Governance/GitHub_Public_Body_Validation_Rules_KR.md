# GitHub Public Body Validation Rules

이 문서는 `LocalNotes/github/public`에 보관하는 GitHub 게시 전 본문 후보 또는 게시 후 로컬 사본을 검사하는 기준을 정의한다.

목표는 PR body, Issue body, Issue 댓글, PR 댓글이 매번 같은 구조와 문체로 작성되도록 하는 것이다. 자동 validator는 기계적으로 확인 가능한 섹션 순서, 금지 문구, 스크린샷 구조를 검사하고, 의미 판단과 예외 판단은 수동 검토로 보완한다.

이 문서는 GitHub public 본문 검수 기준의 단일 원본이다. `AGENTS.md`, `Documentation_Authoring_Workflow_KR.md`, `Feature_Workflow_KR.md`, `Codex_Init_Prompt_KR.md`, `LocalNotes` 템플릿은 이 문서를 참조하고, 동일한 상세 규칙을 반복 정의하지 않는다.

## 적용 범위

검사 대상:

- `LocalNotes/github/public/prs/**/*.md`
- `LocalNotes/github/public/issues/architecture/*.md`
- `LocalNotes/github/public/issues/verification/*.md`
- `LocalNotes/github/public/issues/plan-comments/*.md`
- `LocalNotes/github/public/pr-comments/*.md`

비대상:

- `LocalNotes/github/draft/**`
- `LocalNotes/mini-plans/**`
- `LocalNotes/notes/**`
- `Docs/**/*.md`

`draft`는 작성 중인 초안이므로 `Metadata`, 작업 메모, 공개 기록 확인 항목을 포함할 수 있다. `public`은 GitHub 게시 전 본문 후보 또는 게시 후 로컬 사본이므로 로컬 전용 항목을 포함하지 않는다.

`public` 파일을 수정해도 GitHub 원격 본문은 자동으로 바뀌지 않는다. 원격 생성/수정은 `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md`의 승인 절차를 따른다.

## 공통 규칙

모든 public 본문은 다음 규칙을 따른다.

- 평서형 또는 명사형 bullet로 작성한다.
- `니다`, `니까` 등 사용자 대화형 존댓말 어미를 사용하지 않는다.
- `Metadata`, `Draft`, `Status: Draft`, `공개 기록 확인`, 내부 작업 메모를 포함하지 않는다.
- `PR 생성 후 연결한다`, `PR 본문에 첨부한다` 같은 초안용 문구를 포함하지 않는다.
- `LocalNotes/pr-drafts`, `LocalNotes/github-issues` 같은 이전 경로를 포함하지 않는다.
- Issue 번호, PR 번호, branch, label, draft 여부는 전용 섹션이나 GitHub UI 메타데이터로 분리한다.
- `Summary`에는 번호, 링크 목록, 스크린샷 경로, 상세 검증 로그를 넣지 않는다.

## PR Body

대상:

```text
LocalNotes/github/public/prs/**/*.md
```

필수 섹션 순서:

```markdown
## Summary

## Key Changes

## Verification

## Screenshots

## Unverified / Limitations

## Documentation

## Related Issues

## Next Step
```

검사 기준:

- `Summary`는 3~5개 bullet을 기본으로 한다.
- `Key Changes`는 변경된 파일 또는 책임 단위별로 정리한다.
- `Verification`에는 빌드, 수동 확인, 문서 검수 결과를 먼저 정리한다.
- `Screenshots`는 이미지 유무와 관계없이 항상 두며, 검증 결과를 시각적으로 보강하는 위치로 `Verification` 뒤에 둔다.
- `Related Issues`는 상위 Plan Issue는 `References`, 해당 PR로 완료되는 Architecture / Verification Issue는 `Closes`로 연결한다.
- `Next Step`에는 merge 이후 이어질 작업을 1~3개 bullet로 정리한다.

## Architecture Issue

대상:

```text
LocalNotes/github/public/issues/architecture/*.md
```

필수 섹션 순서:

```markdown
# <Architecture Title>

## Summary

## Scope

## Design Notes

## Documentation

## Related PR
```

검사 기준:

- `Summary`는 1문단 1~2문장으로 작성한다.
- `Scope`에는 대상 branch, 기능 범위, 관련 구조를 둔다.
- `Design Notes`에는 구현 의도와 설계 판단을 요약한다.
- 상세 설명은 GitHub 본문에 길게 쓰지 않고 `Documentation`에서 Docs 링크로 연결한다.

## Verification Issue

대상:

```text
LocalNotes/github/public/issues/verification/*.md
```

필수 섹션 순서:

```markdown
# feature/<branch-name> Verification

## Summary

## Build Verification

## Editor UI Verification

## Screenshots

## Unverified

## Documentation

## Related PR
```

검사 기준:

- `Summary`는 검증 목적을 1문단 1~2문장으로 작성한다.
- `Build Verification`에는 빌드 결과와 빌드 관련 제한을 둔다.
- `Editor UI Verification`에는 에디터에서 직접 확인한 동작만 적는다.
- 직접 확인하지 못한 항목은 `Unverified`로 분리한다.
- `Related PR`에는 해당 검증을 완료하는 PR 번호를 둔다.

## Plan Progress Comment

대상:

```text
LocalNotes/github/public/issues/plan-comments/plan_progress_summary_comment_KR.md
```

필수 섹션 순서:

```markdown
## MVP 진행 요약

## 완료

## 진행 예정

## Related PRs
```

검사 기준:

- 상위 Plan Issue의 첫 진행 댓글에 대응한다.
- 현재까지 완료된 Phase를 누적 요약한다.
- 개별 feature의 상세 검증은 넣지 않고, 완료 Phase와 관련 PR을 연결한다.
- 새 feature가 끝나면 전체 진행판을 갱신한다.

## Plan Feature Comment

대상:

```text
LocalNotes/github/public/issues/plan-comments/*_plan_comment_KR.md
```

단, `plan_progress_summary_comment_KR.md`는 제외한다.

필수 섹션 순서:

```markdown
## Phase <n-n> 진행 기록

## 완료 내용

## 검증

## 남은 제한

## Related PR
```

검사 기준:

- 각 feature 완료 시 상위 Plan Issue에 추가하는 작업 로그다.
- `검증` 섹션은 빌드만 확인했더라도 생략하지 않는다.
- 직접 확인하지 못한 내용은 `남은 제한`에 둔다.
- `Related PR`에는 해당 feature PR을 연결한다.

## PR Screenshot Comment

대상:

```text
LocalNotes/github/public/pr-comments/*.md
```

필수 섹션 순서:

```markdown
## Verification Screenshots

### <screenshot title>

<이 이미지가 무엇을 증명하는지 1문장>

![Description](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/03_Verification/Screenshots/<folder>/<image>.png?raw=true)
```

검사 기준:

- 과거 PR을 본문 수정 없이 보강할 때 사용한다.
- merge 이후 장기 보존용 댓글이므로 `main` 기준 이미지 URL을 사용한다.
- 이미지가 여러 장이면 각 이미지마다 `###` 하위 섹션을 둔다.
- `## Verification Screenshots` 바로 아래에는 공통 요약 문단을 두지 않고, 각 이미지 단위의 `###` 섹션과 설명문으로 검증 내용을 나눈다.

## Screenshots 규칙

PR body와 Verification Issue는 `Screenshots` 섹션을 항상 둔다.

이미지가 없으면 다음처럼 작성한다.

```markdown
## Screenshots

- 없음
```

이미지가 있으면 다음 순서를 따른다.

```markdown
## Screenshots

### <screenshot title>

<이 스크린샷이 무엇을 증명하는지 1문장으로 작성한다.>

![Description](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/<branch-or-main>/Docs/03_Verification/Screenshots/<folder>/<image>.png?raw=true)
```

금지:

- 이미지 앞 `###` 하위 섹션 누락
- 설명 문장 없이 바로 이미지 배치
- 단순 파일 경로 bullet
- 이미지와 `- 없음` 혼용
- 스크린샷 아래 별도 확인 블록 작성

세부 검증 내용은 PR body의 `Verification`, Verification Issue의 `Build Verification` 또는 `Editor UI Verification`에 둔다.

## 검사 범위

이 문서는 public 본문의 섹션 순서, 금지 요소, 문체, `Screenshots` 구조만 검수한다.

draft 작성, public 변환, 원격 게시 승인, `gh` 실행 순서는 `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md`를 따른다.

로컬 환경에 `LocalNotes/tools/validate_github_public_docs.ps1`가 있으면 다음 항목을 자동 검사한다. `LocalNotes/`는 Git 추적 대상이 아니므로 clean checkout 기준 필수 도구로 간주하지 않는다.

- PR body 섹션 순서와 `Screenshots` 구조
- Architecture Issue H1 제목, 섹션 순서, 공통 금지 문구
- Verification Issue 섹션 순서와 `Screenshots` 구조
- Plan Progress Comment 섹션 순서, Phase heading, 상세 검증 문구 혼입 여부
- Plan Feature Comment 섹션 순서, 필수 섹션 내용, `Related PR` 존재 여부
- PR Screenshot Comment 섹션 구조, 이미지별 `###` 제목, 설명문, 이미지 URL
- `LocalNotes/templates`의 고정 Issue 번호 사용 여부

validator에서 warning으로 출력되는 항목은 예외 가능성이 있는 스타일 판단이므로, 원격 반영 전 수동으로 최종 판단한다. validator가 있더라도 의미 품질을 보장하지 않으므로 Architecture Issue의 설계 타당성, Summary 품질, 링크 적합성 같은 내용 판단은 별도 수동 검토로 확인한다.

## 수동 검토 결과 기록

수동 검토 결과는 작업 보고 또는 PR `Verification` 섹션에 다음처럼 기록한다.

```markdown
- PR public 본문 섹션 순서 확인
- Architecture Issue public 본문 섹션 순서 확인
- Verification Issue public 본문 및 Screenshots 구조 확인
- Plan Comment public 본문 섹션 순서 확인
- PR Comment public 본문 이미지 URL 확인
- public 금지 문구 검색 결과 없음
```
