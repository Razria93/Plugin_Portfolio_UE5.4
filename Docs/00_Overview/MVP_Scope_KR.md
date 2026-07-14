# MVP Scope

## MVP 목표

MVP는 선택한 Asset의 참조 관계를 Tree View로 확인할 수 있는 Editor Plugin을 완성하는 것이다.

처음부터 모든 실무형 기능을 넣기보다, Content Browser 선택 Asset에서 시작해 Asset Registry 결과를 안정적으로 Tree View에 표시하는 흐름을 우선한다.

## MVP 포함 기능

- Project Plugin 구조 생성
- Editor 모듈 생성
- Nomad Tab 등록
- Slate 기본 UI 구성
- `STreeView` 기반 결과 표시
- Content Browser 선택 Asset 가져오기
- Dependencies 조회
- Referencers 조회
- Dependencies / Referencers 모드 전환
- Max Depth 제한
- Tree 노드 더블 클릭 시 Content Browser Sync
- CSV Export 기본 기능

## MVP 이후 확장 기능

- Asset Class 필터
- Path 필터
- Engine Content 표시 여부 옵션
- Plugin Content 표시 여부 옵션
- 순환 참조 탐지 표시 강화
- Unused Candidate 전체 스캔
- `.uasset`, `.uexp`, `.ubulk` 합산 크기 표시
- README 스크린샷과 시연 자료 정리

## Future Work

- Node Graph UI
- Dependency Type 세부 옵션
  - Hard
  - Soft
  - SearchableName
  - Manage
- Asset Manager Primary Asset 규칙 반영
- 프로젝트 전체 Reference Audit 리포트
- Editor Utility 또는 Commandlet 기반 batch export

## 범위 고정 기준

다음 조건을 만족하면 MVP 완료로 본다.

- Editor 메뉴 또는 Window 메뉴에서 플러그인 탭을 열 수 있다.
- Content Browser에서 선택한 Asset을 분석 대상으로 가져올 수 있다.
- Dependencies와 Referencers를 전환해서 조회할 수 있다.
- Max Depth가 Tree 생성에 반영된다.
- 결과가 Tree View로 표시된다.
- Tree 노드를 더블 클릭하면 Content Browser가 해당 Asset으로 이동한다.
- 현재 Tree 결과를 CSV로 저장할 수 있다.

## 범위 제외 기준

다음 항목은 MVP 완료 조건이 아니다.

- Node Graph UI
- 삭제 가능한 Asset 확정 판정
- Runtime 메모리 사용량 계산
- 코드 동적 로딩 참조의 완전한 추적
- 모든 Asset Manager 규칙 반영
- 대규모 프로젝트 전체 분석 최적화
