# Feature Workflow

이 문서는 `AssetReferenceInspector` 개발 중 feature를 시작하고, 검증하고, 종료하는 최소 작업 흐름을 정의한다.

## 기본 원칙

- 주요 feature 작업 전에는 mini plan을 먼저 작성한다.
- 작은 fix, 문서 링크 수정, formatting 변경은 mini plan 없이 작업 보고로 대체할 수 있다.
- 작업은 빌드 또는 동작 확인 가능한 작은 단위로 나눈다.
- 직접 확인하지 못한 항목은 성공으로 쓰지 않고 `미확인`으로 남긴다.
- feature 브랜치 작업은 PR을 통해 main에 반영하는 것을 기본 흐름으로 본다.
- 사용자가 명시적으로 요청하기 전에는 commit, amend, push, PR 생성, merge를 실행하지 않는다.

## Mini Plan 기준

Mini plan이 필요한 작업:

- 여러 파일에 걸친 feature
- UI와 C++ 모듈 구조 변경
- Asset Registry, Slate, CSV Export 같은 핵심 기능 추가
- 기존 구조를 바꾸는 refactor
- 하루 이상 이어질 수 있는 작업

Mini plan이 불필요한 작업:

- 오타 수정
- 문서 링크 수정
- 작은 include 정리
- 동작 변화 없는 주석 또는 formatting 수정

Mini plan은 `LocalNotes/feature-plans/` 아래에 작성한다. 로컬 학습과 진행용 문서이므로 커밋 대상이 아니다.

## Branch 기준

브랜치는 모든 작업에 강제하지 않는다.

새 branch 권장:

- 여러 파일에 걸친 feature
- UI와 분석 로직을 함께 바꾸는 작업
- 기존 동작을 바꿀 수 있는 refactor
- 중간 상태가 빌드 불안정할 수 있는 작업

현재 branch에서 진행 가능:

- 작은 fix
- 문서 수정
- 단일 파일 정리
- 실험성이 낮은 변경

권장 이름:

```text
feature/<short-name>
```

예:

```text
feature/ari-nomad-tab
```

## PR 기준

Feature branch를 만든 작업은 PR을 통해 `main`에 반영하는 것을 기본으로 한다.

PR은 다음 목적을 가진다.

- 작업 의도 기록
- 변경 내용 요약
- 검증 결과 기록
- 미확인 항목과 제한 사항 기록

작은 문서 수정, 오타 수정, 단일 파일 정리처럼 feature branch 없이 진행한 작업은 PR을 필수로 하지 않는다.

Codex는 사용자가 명시적으로 요청하기 전에는 `git push`, `gh pr create`, `gh pr merge`를 실행하지 않는다. 작업 종료 시 필요한 명령과 PR 본문 초안을 제안한다.

## 검증 기준

C++ 코드, `.uplugin`, `Build.cs`, `.uproject` 변경은 가능한 경우 UE 5.4 Editor Target 빌드로 검증한다.

문서만 바꾼 작업은 빌드를 필수로 하지 않는다. 대신 다음을 확인한다.

- 변경 파일 범위
- 문서 간 링크와 용어 일관성
- 기존 규칙과 충돌 여부

검증 결과는 작업 보고 또는 `Docs/03_Verification`에 남긴다.

## 문서 업데이트 기준

작업 종료 시 다음 문서 업데이트가 필요한지 확인한다.

- 사용 방법이 바뀐 경우
- 빌드 방법이 바뀐 경우
- 플러그인 구조가 바뀐 경우
- UI 흐름이 바뀐 경우
- Known Limitation이 생긴 경우
- 검증 결과를 남길 필요가 있는 경우

문서 업데이트가 필요하면 범위와 이유를 먼저 제안한다.

## Commit 판단 기준

독립 커밋 권장:

- 하나의 feature가 빌드 가능한 상태로 끝난 경우
- 하나의 refactor가 동작 변화 없이 끝난 경우
- 문서 변경만 독립적으로 의미가 있는 경우

Amend 권장:

- 방금 만든 커밋의 오타 수정
- 누락된 include 또는 빌드 수정
- 같은 변경 의도의 문서 보완
- 아직 공유하지 않은 개인 작업 커밋 보정

커밋 보류 권장:

- 검증이 끝나지 않은 경우
- 변경 의도가 아직 정리되지 않은 경우
- 후속 작업과 합쳐야 의미가 명확한 경우

작업 종료 시 Git Bash 기준 명령을 제안하되, 사용자가 요청하기 전에는 직접 실행하지 않는다.

## PR Summary Template

```markdown
## Summary
- 

## Verification
- 

## Unverified / Limitations
- 

## Documentation
- 
```

## Feature Plan Template

```markdown
# Feature: <name>

## 목표
- 

## 범위
- 포함:
- 제외:

## 예상 변경 파일
- 

## 구현 단위
1. 
2. 
3. 

## 검증 방법
- 빌드:
- 에디터 확인:
- 미확인 가능성:

## 문서 업데이트 필요 여부
- README:
- BUILD:
- Docs:
- Known Limitations:

## 예상 커밋 단위
- 
```
