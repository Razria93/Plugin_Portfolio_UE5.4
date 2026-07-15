# Repository Guidelines

## 프로젝트 구조와 모듈 구성

이 저장소는 Unreal Engine 5.4용 Editor Plugin `AssetReferenceInspector`를 개발하기 위한 Demo Host 프로젝트입니다.

- `Portfolio_PlugIn.uproject`: Demo Host 프로젝트 파일
- `Source/Portfolio_PlugIn/`: 최소 Host C++ 모듈
- `Plugins/AssetReferenceInspector/`: 실제 플러그인 구현 위치
- `Docs/`: 계획, 설계, 검증, 시연 문서
- `Config/`: Demo Host 프로젝트 설정

Engine 설치 경로의 `Engine/Plugins`에는 작업하지 않습니다. 최종 배포 단위는 `Plugins/AssetReferenceInspector` 폴더입니다.

## 빌드, 테스트, 개발 명령

UE 5.4 Unreal Build Tool 기준 빌드 명령:

```powershell
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" Portfolio_PlugInEditor Win64 Development -Project="C:\UE5_Portfolio\Portfolio_UE5.4_verGit\Portfolio_PlugIn\Portfolio_PlugIn.uproject" -WaitMutex -FromMsBuild
```

작업 전후에는 `git status --short`로 변경 범위를 확인합니다.

`Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `.vs/` 같은 생성물은 커밋하지 않습니다.

## 코딩 스타일과 이름 규칙

Unreal Engine C++ 관례를 따릅니다.

- C++ 및 `Build.cs` 파일은 탭 들여쓰기를 사용합니다.
- 타입과 함수는 PascalCase를 사용합니다.
- Unreal 접두사를 명확히 사용합니다.
  - 예: `FAssetReferenceNode`, `SAssetReferenceInspectorWidget`, `EAssetReferenceMode`
- 플러그인 코드는 Portfolio 전용 게임플레이 클래스에 의존하지 않습니다.

Slate UI는 `Private/UI`, 분석 로직은 `Private/Analysis`, export 로직은 `Private/Export` 아래에 둡니다.

Public 헤더에는 외부 모듈이 실제로 포함해야 하는 타입만 둡니다. 플러그인 내부 Command, Menu, ToolMenus, Slate 구현 세부 타입은 기본적으로 `Private`에 둡니다.

`.cpp` include는 자기 헤더, 같은 모듈 내부 헤더, Unreal Framework/Modules, ToolMenus, Slate/Widgets 순서로 그룹화합니다. 헤더에서는 public override, private helper, private field 순서로 섹션을 나눕니다.

## 검증 기준

아직 자동화 테스트는 없습니다. 변경 사항은 다음 기준으로 검증합니다.

- `Portfolio_PlugInEditor` 타깃 빌드 성공 여부
- 에디터에서 플러그인 탭 오픈 여부
- Content Browser 선택 Asset 연동 여부
- Dependencies / Referencers, Depth, Filter, CSV Export, Content Browser Sync 동작 여부

직접 확인하지 못한 항목은 성공으로 쓰지 않고 `미확인`으로 기록합니다.

## 커밋 규칙

커밋 메시지는 가벼운 Conventional Commit 형식을 사용하고, 요약은 한국어로 작성합니다.

예:

- `docs: 플러그인 계획 문서 추가`
- `chore: UE 프로젝트 gitignore 추가`
- `feat: Asset Reference Inspector 탭 추가`
- `fix: Asset Registry 조회 실패 처리`

사용자가 명시적으로 요청하기 전에는 commit, push, PR 생성, merge를 실행하지 않습니다. 작업 종료 시에는 커밋 단위, amend 필요 여부, 보류 필요 여부를 판단해 Git Bash 기준 명령을 제안합니다.

## 에이전트 작업 규칙

MVP는 Tree View 기반입니다. Asset Registry 기반 핵심 흐름이 안정화되기 전에는 Node Graph UI를 추가하지 않습니다.

`Unused Asset`은 삭제 가능 판정이 아니라 `Unused Candidate`로만 표현합니다.

작업은 `Docs/01_Planning/Feature_Work_Plan_KR.md`의 기능, 리팩토링, 검증 단위에 맞춰 나눕니다.

Feature 범위는 내부 구현 조각이 아니라 사용자가 설명 가능한 기능 단위로 잡습니다. 내부 API 등록, 의존성 추가, 함수 선언 같은 변경은 feature 안의 구현 단계로 다룹니다.

주요 feature를 시작하기 전에는 mini plan을 먼저 작성합니다. 작은 fix, 문서 링크 수정, 단순 formatting 변경은 별도 계획 문서 없이 작업 보고로 대체할 수 있습니다.

작업은 빌드 또는 동작 확인 가능한 작은 단위로 나누고, 각 단위가 끝날 때 검증 결과 또는 `미확인` 사유를 남깁니다.

작업 종료 시에는 README, BUILD, Docs, 검증 기록, Known Limitations 업데이트가 필요한지 확인합니다. 문서 업데이트가 필요하면 범위와 이유를 사용자에게 먼저 제안합니다.

작업 종료 시 C++ 코드 변경이 있으면 `Docs/02_Architecture/Code_Organization_KR.md` 기준으로 Public/Private 배치, include 순서, 헤더 섹션, 의존성 범위를 검토합니다.

feature 브랜치 작업은 PR을 통해 main에 반영하는 것을 기본 흐름으로 봅니다. 브랜치 생성, mini plan 항목, 검증 반복, PR summary, 커밋/amend/보류 판단 기준은 `Docs/04_Governance/Feature_Workflow_KR.md`를 따릅니다.

작업 종료 시 Architecture 또는 Verification 문서가 생성/수정된 경우, PR 본문과 별도로 GitHub Issue 본문 초안을 제안합니다. Issue 초안은 원문 복사가 아니라 요약, 체크리스트, 관련 Docs 링크 중심으로 작성합니다.

GitHub Issue 라벨은 `Docs/04_Governance/Documentation_Workflow_KR.md` 기준을 따릅니다. `[Plan]`은 `work plan`, `[Architecture]`는 `system architecture`, `[Verification]`은 `verification log`를 사용합니다.

GitHub Issue, PR 본문, Plan Issue 누적 댓글, Architecture/Verification 공개 기록, PR 스크린샷/댓글을 게시하기 전에는 같은 카테고리의 GitHub 공개 기록 형식을 확인합니다. 상세 기준은 `Docs/04_Governance/Documentation_Workflow_KR.md`를 따릅니다.

feature 종료 시 상위 `[Plan]` Issue가 있으면 첫 진행 댓글의 누적 Phase 요약 갱신 여부와 이번 feature 완료 로그 댓글 추가 여부를 확인합니다. Plan Issue는 `References`, 브랜치 단위 Architecture / Verification Issue는 완료 시 `Closes`로 연결합니다.

PR merge 후에는 `main`으로 전환하고 `git pull`로 로컬 기준점을 갱신했는지 확인합니다.

로컬/원격 브랜치는 사용자가 명시적으로 요청하기 전에는 삭제하지 않습니다. 삭제된 브랜치가 필요하면 남아 있는 커밋 SHA를 기준으로 복원할 수 있습니다.

Draft PR은 검증 결과, 스크린샷, 문서 링크, Related Issues가 정리된 뒤 Ready for review로 전환합니다.

검증 스크린샷이 feature 완료 증거로 필요하면 최종 이미지를 `Docs/03_Verification/Screenshots/<branch-name>/`로 승격하고 PR 또는 검증 로그에서 링크합니다. 임시 이미지는 `LocalNotes/screenshots`에 둡니다.

`LocalNotes` Markdown은 번호 없는 `<topic>_<document_type>_KR.md` 파일명과 `Metadata` 섹션을 사용합니다. GitHub Issue 번호, PR 번호, Phase 번호는 파일명이 아니라 Metadata에 기록합니다.

`LocalNotes`의 `Metadata`와 `Draft` 섹션은 로컬 초안 전용입니다. `Docs` 승격본이나 GitHub 게시 본문에는 포함하지 않습니다.
