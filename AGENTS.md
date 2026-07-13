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

## 검증 기준

아직 자동화 테스트는 없습니다. 변경 사항은 다음 기준으로 검증합니다.

- `Portfolio_PlugInEditor` 타깃 빌드 성공 여부
- 에디터에서 플러그인 탭 오픈 여부
- Content Browser 선택 Asset 연동 여부
- Dependencies / Referencers, Depth, Filter, CSV Export, Content Browser Sync 동작 여부

직접 확인하지 못한 항목은 성공으로 쓰지 않고 `미확인`으로 기록합니다.

## 커밋과 PR 규칙

커밋 메시지는 가벼운 Conventional Commit 형식을 사용하고, 요약은 한국어로 작성합니다.

예:

- `docs: 플러그인 계획 문서 추가`
- `chore: UE 프로젝트 gitignore 추가`
- `feat: Asset Reference Inspector 탭 추가`
- `fix: Asset Registry 조회 실패 처리`

PR에는 변경 요약, 테스트한 UE 버전, 빌드 결과, UI 변경 스크린샷, 제한 사항 또는 후속 작업을 포함합니다.

## 에이전트 작업 규칙

MVP는 Tree View 기반입니다. Asset Registry 기반 핵심 흐름이 안정화되기 전에는 Node Graph UI를 추가하지 않습니다.

`Unused Asset`은 삭제 가능 판정이 아니라 `Unused Candidate`로만 표현합니다.

작업은 `Docs/01_Planning/AssetReferenceInspector_Feature_Work_Plan_KR.md`의 기능, 리팩토링, 검증 단위에 맞춰 나눕니다.

주요 feature를 시작하기 전에는 목표, 범위, 예상 변경 파일, 검증 방법, 커밋 단위를 mini plan으로 먼저 정리합니다. 작은 fix, 문서 링크 수정, 단순 formatting 변경은 별도 계획 문서 없이 작업 보고로 대체할 수 있습니다.

작업은 빌드 또는 동작 확인 가능한 작은 단위로 나누고, 각 단위가 끝날 때 검증 결과 또는 `미확인` 사유를 남깁니다.

작업 종료 시에는 README, BUILD, Docs, 검증 기록, Known Limitations 업데이트가 필요한지 확인합니다. 문서 업데이트가 필요하면 범위와 이유를 사용자에게 먼저 제안합니다.

사용자가 명시적으로 요청하기 전에는 commit 또는 push를 실행하지 않습니다. 작업 종료 시에는 커밋 단위, amend 필요 여부, 보류 필요 여부를 판단해 Git Bash 기준 명령을 제안합니다.
