# AssetReferenceInspector 작업 규칙

## 기본 원칙

- 이 저장소는 `AssetReferenceInspector` Editor Plugin 개발용 Demo Host 프로젝트다.
- 실제 플러그인 구현은 `Plugins/AssetReferenceInspector` 아래에서 진행한다.
- Engine 설치 경로의 `Engine/Plugins`에는 직접 작업하지 않는다.
- 최종 배포 및 적용 단위는 `Plugins/AssetReferenceInspector` 폴더다.

## 독립성 규칙

- 본 Portfolio 프로젝트 전용 클래스를 include하지 않는다.
- `/Game/Portfolio` 같은 고정 경로에 의존하지 않는다.
- 분석 로직은 `FAssetData`, `PackageName`, Asset Registry 기반으로 작성한다.
- 다른 UE 프로젝트의 `Plugins` 폴더로 복사해도 동작하는 구조를 목표로 한다.

## Editor Plugin 규칙

- 플러그인은 Runtime 기능이 아니라 Editor 확장 도구다.
- 모듈 타입은 Editor 전용으로 구성한다.
- UI는 Slate 기반으로 구현한다.
- Editor 메뉴 또는 Window 탭을 통해 실행한다.
- Content Browser 선택 Asset과 연동한다.

## MVP 범위 규칙

- MVP는 Tree View 기반 분석 도구다.
- Node Graph UI는 Future Work로 남긴다.
- 선택 Asset 기준 Dependencies / Referencers 분석을 먼저 안정화한다.
- 필터, CSV Export, 순환 참조, 크기 표시는 Tree View 흐름이 동작한 뒤 추가한다.

## Unused Candidate 규칙

- "Unused Asset"이라고 단정하지 않는다.
- UI와 문서에서는 `Unused Candidate` 또는 `No Referencers Found` 표현을 사용한다.
- 삭제 가능 여부는 사용자가 별도로 검토해야 한다.
- Map, Redirector, Engine Content, Plugin Content는 기본 제외 또는 옵션 처리한다.
- 코드 동적 로딩, Soft Reference, Asset Manager, 문자열 경로 참조로 인해 누락이 가능하다는 점을 문서화한다.

## Asset Size 규칙

- Asset 크기는 디스크 파일 기준 추정값으로 표시한다.
- 우선 `.uasset` 파일 크기를 계산한다.
- 가능하면 `.uexp`, `.ubulk` 파일 크기도 합산한다.
- 런타임 메모리 사용량이라고 표현하지 않는다.

## Git 관리 규칙

- 저장소 루트는 Demo Host 프로젝트 루트로 둔다.
- `Plugins/AssetReferenceInspector`, `Docs`, `Config`, `.uproject`, 필요한 `Source` 파일을 추적한다.
- `Binaries`, `Intermediate`, `Saved`, `DerivedDataCache`, `.vs` 같은 생성물은 추적하지 않는다.
- 변경 전후에는 `git status --short`로 작업 범위를 확인한다.

## 검증 규칙

- 주요 구현 단위마다 UE 5.4 Editor 타깃 빌드를 확인한다.
- 빌드가 불가능한 상태로 문서를 완료했다고 판단하지 않는다.
- UI 변경은 에디터에서 탭이 열리는지 확인한다.
- Asset Registry 기능은 Demo Host Asset으로 먼저 확인한 뒤 Portfolio 프로젝트에 적용한다.

## 문서화 규칙

- 구현이 끝난 기능은 README 또는 검증 문서에 사용 방법을 남긴다.
- Known Limitations를 숨기지 않는다.
- 포트폴리오 시연 흐름은 Demo Host 검증과 실제 Portfolio 적용을 분리해서 기록한다.

