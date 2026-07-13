# Codex Init Prompt

새 Codex 세션에서 이 저장소를 다룰 때 아래 내용을 초기 지침으로 사용한다.

```text
현재 세션은 Unreal Engine 5.4 Editor Plugin 개발 세션입니다.

프로젝트 목적:
- Demo Host 프로젝트 안에서 독립 Project Plugin `AssetReferenceInspector`를 개발합니다.
- 작업 위치는 프로젝트 루트의 `Plugins/AssetReferenceInspector`입니다.
- Engine 설치 경로의 `Engine/Plugins`에는 작업하지 않습니다.
- 최종 산출물은 기존 Portfolio 프로젝트의 `Plugins` 폴더로 복사 가능한 독립 Editor Plugin입니다.

초기 확인 규칙:
- 작업 시작 전 cwd, git branch/status, `.uproject`, `Plugins/`, `Source/`, `Config/`, `Docs/`, `AGENTS.md`를 확인합니다.
- 플러그인 코드 작성 전 `.uplugin`, `Build.cs`, 모듈 타입, Editor/Runtime 분리 여부를 확인합니다.
- 사용자가 명시적으로 요청하기 전에는 commit/push 하지 않습니다.

작업 원칙:
- 본 Portfolio 프로젝트 전용 클래스에 의존하지 않습니다.
- 분석 로직은 Asset Registry, `FAssetData`, `PackageName` 기반으로 작성합니다.
- MVP는 Slate Tree View 기반입니다.
- Node Graph UI는 Future Work로 둡니다.
- "Unused Asset"은 삭제 가능 판정이 아니라 `Unused Candidate`로만 표현합니다.
- Asset Size는 런타임 메모리가 아니라 디스크 파일 기준 추정값으로 표현합니다.

빌드/검증 규칙:
- 가능한 경우 UE 5.4 `Portfolio_PlugInEditor Win64 Development` 빌드를 수행합니다.
- 직접 빌드/실행하지 못한 항목은 반드시 `미확인`으로 기록합니다.
- 빌드 에러는 첫 번째 원인 에러를 우선 분석합니다.
- 에디터 UI 작업은 탭 오픈, 버튼 동작, Content Browser 연동 여부를 검증 기준으로 둡니다.

문서 규칙:
- 루트 `README.md`는 저장소 대표 문서로 사용합니다.
- `Docs/`는 계획, 설계, 검증, 시연 문서를 관리합니다.
- 플러그인 구현이 안정화되면 `Plugins/AssetReferenceInspector/README.md`를 별도로 작성합니다.
- Known Limitations를 숨기지 않습니다.

작업 보고 형식:
작업 시작 시 현재 repo/git 상태, 확인한 UE/plugin 구조, 작업 계획, 위험 요소를 요약합니다.
작업 종료 시 변경 파일, 핵심 변경 내용, 빌드/검증 결과, 미확인 항목, 다음 작업을 요약합니다.
```

