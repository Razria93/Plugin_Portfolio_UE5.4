# PR: AssetReferenceInspector 에디터 창 오픈 추가

## Branch

```text
feature/ari-nomad-tab
```

## Summary

- `AssetReferenceInspector` Editor Plugin에 에디터 Window 메뉴 진입점을 추가했다.
- `TCommands`, `FUICommandList`, `ToolMenus` 기반으로 메뉴 Command를 구성했다.
- `FGlobalTabmanager`와 `RegisterNomadTabSpawner`를 사용해 독립형 Nomad Tab을 등록했다.
- 메뉴 클릭 시 `TryInvokeTab`으로 `Asset Reference Inspector` 탭을 열도록 연결했다.
- 에디터 창 오픈 구조를 아키텍처 문서로 정리했다.

## Key Changes

- `FAssetReferenceInspectorCommands` 추가
  - `OpenPluginWindow` Command 정보를 `UI_COMMAND`로 등록
  - 기본 표시 이름, 설명, 버튼 타입, 단축키 없음 설정

- `FAssetReferenceInspectorModule` 확장
  - Command 등록과 해제
  - `FUICommandList` 생성과 `MapAction` 연결
  - `ToolMenus` 기반 Window 메뉴 항목 등록
  - Nomad Tab Spawner 등록과 해제
  - `SDockTab` 기반 최소 Slate 콘텐츠 생성

- 문서 보완
  - 에디터 창 오픈 구조 문서 추가
  - 문서 인덱스 업데이트
  - 빌드 및 에디터 UI 검증 로그 업데이트

## Verification

- `Portfolio_PlugInEditor Win64 Development` 빌드 성공
- Window 메뉴에서 `Asset Reference Inspector` 항목 클릭 확인
- 독립형 `Asset Reference Inspector` 탭 오픈 확인
- 탭 내부 중앙의 `Asset Reference Inspector` 텍스트 표시 확인

## Unverified / Limitations

- Toolbar Button은 이번 범위에서 제외했다.
- 커스텀 아이콘과 Slate Style은 이번 범위에서 제외했다.
- 실제 Asset 분석 UI, Asset Registry 조회, Tree View는 아직 구현하지 않았다.
- 현재 탭 내부 UI는 임시 `STextBlock`만 표시한다.

## Documentation

- `Docs/02_Architecture/AssetReferenceInspector_Editor_Window_Flow_KR.md`
- `Docs/03_Verification/Build_Verification_Log_KR.md`
- `Docs/README_KR.md`
- `Docs/01_Planning/AssetReferenceInspector_Feature_Work_Plan_KR.md`

## Next Step

- 새 브랜치에서 `SAssetReferenceInspectorWidget` 기반 기본 Slate UI Shell을 분리한다.
