# Feature Workflow

이 문서는 `AssetReferenceInspector`의 feature 작업을 시작, 구현, 검증, 종료하는 기준을 정의한다.

Feature Workflow의 책임은 작업 단위와 종료 조건을 정하는 것이다. GitHub Issue / PR 본문 작성, public 변환, 원격 게시 승인 절차는 `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md`를 따른다. GitHub public 본문의 섹션 순서와 문체 검수는 `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`를 따른다.

## 기본 원칙

- 주요 feature 작업 전에는 mini plan을 먼저 작성한다.
- 작은 fix, 문서 링크 수정, formatting 변경은 mini plan 없이 작업 보고로 대체할 수 있다.
- 작업은 빌드 또는 동작 확인 가능한 작은 단위로 나눈다.
- 직접 확인하지 못한 항목은 성공으로 쓰지 않고 `미확인`으로 남긴다.
- feature 브랜치 작업은 PR을 통해 `main`에 반영하는 흐름을 기본으로 본다.
- 사용자가 명시적으로 요청하기 전에는 commit, amend, push, PR 생성, merge를 실행하지 않는다.

## Feature 범위

Feature는 사용자가 체감하거나 설명할 수 있는 기능 단위로 잡는다.

좋은 feature 예:

- 에디터 메뉴에서 `Asset Reference Inspector` 창을 열 수 있다.
- 선택한 Asset의 Dependencies를 Tree View에 표시한다.
- Dependencies / Referencers 모드를 전환할 수 있다.
- Tree 노드를 더블 클릭하면 Content Browser에서 해당 Asset을 선택한다.
- 현재 분석 결과를 CSV로 export한다.

너무 작은 feature 예:

- `Build.cs`에 의존성만 추가한다.
- Nomad Tab Spawner만 등록한다.
- 함수 선언만 추가한다.
- include만 정리한다.

너무 작은 변경은 feature가 아니라 feature 안의 구현 단계로 다룬다.

## 구현 단계

구현 단계는 하나의 feature를 안전하게 완성하기 위한 내부 작업 순서다.

좋은 구현 단계는 다음 조건을 만족한다.

- 하나의 코드 변경 목적을 가진다.
- 다음 단계가 무엇인지 명확하다.
- 가능하면 빌드, 에디터 확인, 코드 리뷰 중 하나로 검증할 수 있다.
- 실패했을 때 되돌릴 위치를 알 수 있다.

구현 단계 예:

- Tab Spawner 등록과 해제 추가
- 기본 `SDockTab` 생성 함수 추가
- 메뉴 Command 추가
- `ToolMenus`에 Window 메뉴 진입점 추가
- 메뉴 클릭 시 `TryInvokeTab` 연결

구현 단계는 mini plan에 순서로 기록하되, 각 단계를 별도 feature나 PR로 나누지는 않는다.

## Mini Plan

Mini plan이 필요한 작업:

- 여러 파일에 걸친 feature
- UI와 C++ 모듈 구조를 함께 바꾸는 작업
- Asset Registry, Slate, CSV Export 같은 핵심 기능 추가
- 기존 구조를 바꾸는 refactor
- 하루 이상 이어질 수 있는 작업

Mini plan이 불필요한 작업:

- 오타 수정
- 문서 링크 수정
- 작은 include 정리
- 동작 변화 없는 주석 또는 formatting 수정

Mini plan에는 다음 항목을 포함한다.

- Goal
- Scope
- Target Files
- Implementation Steps
- Verification
- Documentation Check
- Commit Plan

실제 작성 템플릿은 `LocalNotes/templates/mini_plan_template_KR.md`를 사용한다. Mini plan은 `LocalNotes/mini-plans/` 아래에 작성하며 커밋하지 않는다.

## Branch

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

## 브랜치 보존과 복원

로컬/원격 feature 브랜치는 사용자가 명시적으로 요청하기 전에는 삭제하지 않는다.

브랜치 삭제가 필요한 경우 다음을 먼저 확인한다.

- 해당 브랜치가 PR로 merge되었는가
- 변경 내용이 `main`에 반영되었는가
- 복원 가능한 커밋 SHA가 남아 있는가
- 사용자가 삭제를 명시적으로 요청했는가

삭제된 브랜치는 커밋이 남아 있으면 같은 이름으로 복원할 수 있다.

```bash
git switch -c feature/<short-name> <commit-sha>
git push -u origin feature/<short-name>
```

브랜치 복원은 작업 재개뿐 아니라 검증, 비교, 기록 보강 목적으로도 사용할 수 있다. 에이전트는 로컬/원격 브랜치를 임의로 삭제하지 않는다.

## 검증

C++ 코드, `.uplugin`, `Build.cs`, `.uproject` 변경은 가능한 경우 UE 5.4 Editor Target 빌드로 검증한다.

문서만 바꾼 작업은 빌드를 필수로 하지 않는다. 대신 다음을 확인한다.

- 변경 파일 범위
- 문서 간 링크와 용어 일관성
- 기존 규칙과 충돌 여부

UI 변화나 에디터 동작이 feature 완료 기준에 포함되면 검증 스크린샷 필요 여부를 확인한다. 스크린샷 보관, 승격, 파일명, GitHub 이미지 링크 기준은 `Docs/04_Governance/Screenshot_Workflow_KR.md`를 따른다.

검증 결과는 작업 보고 또는 `Docs/03_Verification`에 남긴다. 직접 확인하지 못한 항목은 `미확인`으로 기록한다.

## 문서 업데이트

작업 종료 시 다음 문서 업데이트가 필요한지 확인한다.

- 사용 방법이 바뀐 경우
- 빌드 방법이 바뀐 경우
- 플러그인 구조가 바뀐 경우
- UI 흐름이 바뀐 경우
- Known Limitation이 생긴 경우
- 검증 결과를 남길 필요가 있는 경우

문서 업데이트가 필요하면 범위와 이유를 먼저 제안한다.

문서 책임 경계는 `Docs/04_Governance/Documentation_Responsibility_KR.md`를 따른다. 작성, 승격, GitHub 게시 준비 절차는 `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md`를 따른다.

### 구현 중 Architecture 보강

기능 구현 후 코드 설명, 사용자 질문, 설계 판단 검토 과정에서 다음 내용이 확정되면 관련 `Docs/02_Architecture` 문서에 즉시 반영하거나 반영 범위를 제안한다.

- API 선택 이유
- 필터 / 표시 / 분석 정책
- Unreal Engine 경로, 모듈, Slate, Asset Registry 동작 해석
- 나중에 다시 혼동될 수 있는 제거/유지 판단
- 현재 구현과 직접 연결되는 제한 사항

이 단계에서 Architecture 문서를 보강했다면 PR 준비 전에는 같은 내용을 다시 작성하지 않고, stale 문구와 링크만 최종 확인한다.

## 코드 구성 검토

C++ 코드, `Build.cs`, `.uplugin` 변경이 있는 작업은 종료 전에 `Docs/02_Architecture/Code_Organization_KR.md` 기준으로 구조를 검토한다.

검토 항목:

- Public 헤더에 외부 모듈이 실제로 include해야 하는 타입만 노출했는가
- 플러그인 내부 Command, Menu, ToolMenus, Slate 구현 세부 타입이 `Private`에 있는가
- Public 헤더에 노출된 타입 때문에 필요한 의존성만 `PublicDependencyModuleNames`에 있는가
- 구현에서만 쓰는 의존성은 `PrivateDependencyModuleNames`에 있는가
- `.cpp` include 순서가 자기 헤더, 같은 모듈 내부 헤더, Unreal Framework/Modules, ToolMenus, Slate/Widgets 흐름을 따르는가
- 헤더 섹션이 public override, private helper, private field 순서로 읽히는가
- 새 UI / Analysis / Export 파일이 권장 폴더인 `Private/UI`, `Private/Analysis`, `Private/Export`에 배치되었는가

구조 보완이 필요하면 기능 구현 커밋에 포함할지, 별도 `refactor` / `docs` 커밋으로 분리할지 작업 종료 보고에서 제안한다.

## Commit 판단

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

## PR 준비 기준

Feature branch를 만든 작업은 PR을 통해 `main`에 반영하는 것을 기본으로 한다.

PR은 다음 목적을 가진다.

- 작업 의도 기록
- 변경 내용 요약
- 검증 결과 기록
- 미확인 항목과 제한 사항 기록

작은 문서 수정, 오타 수정, 단일 파일 정리처럼 feature branch 없이 진행한 작업은 PR을 필수로 하지 않는다.

다음 항목이 남아 있으면 PR은 Draft로 둔다.

- 검증 결과가 아직 정리되지 않았다.
- 검증 스크린샷 또는 문서 링크가 아직 반영되지 않았다.
- Related Issues 관계가 확정되지 않았다.
- 미확인 항목과 제한 사항이 PR 본문에 정리되지 않았다.

다음 항목을 확인한 뒤 Ready for review로 전환한다.

- `Verification` 섹션이 실제 검증 결과와 일치한다.
- `Unverified / Limitations` 섹션이 남은 제한을 숨기지 않는다.
- `Documentation` 섹션이 관련 Docs를 가리킨다.
- `Related Issues`에서 상위 Plan Issue는 `References`, 해당 PR로 완료되는 Architecture / Verification Issue는 `Closes`로 연결된다.
- PR 본문 작성, public 변환, 원격 게시 준비가 `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md` 기준을 만족한다.

## PR 종료 체크

PR merge 전에는 다음을 확인한다.

- 상위 Plan Issue를 `References`로 연결했는가
- 해당 PR에서 완료되는 Architecture / Verification Issue를 `Closes`로 연결했는가
- 필요한 label이 적용되었는가
- 검증 결과와 스크린샷 필요 여부를 확인했는가
- 문서 링크와 `Unverified / Limitations`가 현재 상태와 일치하는가
- PR / Issue / Plan 댓글 작성과 원격 반영 준비가 `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md` 기준을 만족하는가

PR merge 후에는 다음을 확인한다.

- 상위 Plan Issue의 첫 누적 진행 댓글을 갱신했는가
- 상위 Plan Issue에 feature 완료 로그 댓글을 추가했는가
- 로컬을 `main`으로 전환했는가
- 원격 `main`을 pull 했는가

```bash
git switch main
git pull
```

상위 Plan Issue 갱신을 누락한 경우, 발견 즉시 첫 누적 진행 댓글과 feature 완료 로그 댓글을 보정한다.

## GitHub Issue 제안 기준

PR 전후로 Architecture 또는 Verification 문서가 생성/수정된 경우, PR 본문과 별도로 GitHub Issue 본문 초안을 제안한다.

Issue는 Docs 원문을 복사하는 공간이 아니라 GitHub 상의 추적과 리뷰를 위한 요약 공간으로 사용한다. 상세 본문 구조와 문체는 `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`를 따른다.

권장 Issue 유형:

- `[Architecture]`: 구조 요약, 주요 설계 판단, 관련 Docs 링크
- `[Verification]`: 대상 브랜치, 검증 항목, 결과, 특이사항, 관련 Docs 링크

권장 label과 `References` / `Closes` 기준은 `Docs/04_Governance/Documentation_Responsibility_KR.md`를 따른다.

## Plan Issue 운영 기준

상위 `[Plan]` Issue는 MVP 전체 진행 상태를 추적하는 장기 기록이다. 브랜치마다 새 Plan Issue를 만들지 않고, feature 종료 시 기존 상위 Plan Issue의 댓글을 갱신한다.

Plan Issue 댓글은 두 종류로 나눈다.

- 첫 누적 진행 댓글: 현재까지 완료된 Phase 요약과 다음 예정 작업을 보여주는 진행판
- feature 완료 로그 댓글: 각 feature 완료 시점의 작업 내용, 검증 결과, 관련 PR / Issue 기록

Plan Issue 댓글의 본문 구조와 문체는 `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`의 Plan Progress Comment / Plan Feature Comment 기준을 따른다.

## GitHub Issue / PR 작성 책임

Feature Workflow는 Issue / PR을 언제 확인할지만 정의한다. 본문 작성 방식은 전담 문서를 따른다.

- Issue / PR 책임 경계, label, `References` / `Closes`: `Docs/04_Governance/Documentation_Responsibility_KR.md`
- draft 작성, public 변환, 원격 게시 승인 절차: `Docs/04_Governance/Documentation_Authoring_Workflow_KR.md`
- PR body, Architecture Issue, Verification Issue, Plan Issue 댓글, PR 댓글 섹션 구조와 검수 기준: `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`
- 스크린샷 보관, 승격, 파일명, 이미지 링크: `Docs/04_Governance/Screenshot_Workflow_KR.md`

브랜치마다 새 Plan Issue 본문은 만들지 않는다. 계획은 mini plan과 `Docs/01_Planning`에서 관리하고, 기존 상위 Plan Issue가 있을 때만 완료 로그와 누적 진행 댓글을 갱신한다.
