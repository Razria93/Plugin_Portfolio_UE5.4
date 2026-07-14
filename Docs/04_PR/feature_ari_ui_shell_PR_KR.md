# PR: AssetReferenceInspector 기본 Slate UI Shell 추가

## Branch

```text
feature/ari-ui-shell
```

## Summary

- `AssetReferenceInspector` 탭의 기본 UI를 `SAssetReferenceInspectorWidget`으로 분리했습니다.
- `SDockTab` 내부에서 임시 UI를 직접 구성하던 구조를 제거했습니다.
- 이후 Content Browser 선택, Analyze 버튼, Tree View를 붙일 수 있는 기본 UI Shell을 구성했습니다.
- Slate UI 기본 구성과 Slot / 배치 옵션을 문서화했습니다.
- 작업 종료 시 코드 구성 규칙 기준 검토 흐름을 문서에 반영했습니다.

## Key Changes

- `SAssetReferenceInspectorWidget` 추가
  - `Private/UI` 아래에 플러그인 전용 Slate Widget으로 배치
  - `SCompoundWidget` 기반으로 탭 내부 루트 UI 구성

- `FAssetReferenceInspectorModule` 정리
  - `OnSpawnPluginTab()`에서 `SAssetReferenceInspectorWidget`을 탭 콘텐츠로 사용
  - 기존 직접 `SBox + STextBlock`을 구성하던 임시 UI 제거

- 기본 UI Shell 구성
  - 제목 영역
  - `Selected Asset: None` placeholder
  - `Pick Selected Asset` 버튼 placeholder
  - `Analyze` 버튼 placeholder
  - `Mode: Dependencies / Referencers` placeholder
  - 분석 결과 영역 placeholder

- 문서 보완
  - Slate UI 기초 문서 추가
  - 코드 구성 검토 규칙 보완
  - 빌드 및 에디터 UI 확인 결과 기록

## Verification

- `Portfolio_PlugInEditor Win64 Development` 빌드 성공
- Window 메뉴에서 `Asset Reference Inspector` 탭 오픈 확인
- `SAssetReferenceInspectorWidget` 기반 기본 UI Shell 표시 확인
- `Selected Asset: None`, `Pick Selected Asset`, `Analyze`, `Mode`, 결과 영역 placeholder 표시 확인

## Unverified / Limitations

- `Pick Selected Asset` 버튼은 아직 Content Browser 선택 Asset을 가져오지 않습니다.
- `Analyze` 버튼은 아직 Asset Registry 분석을 실행하지 않습니다.
- Dependencies / Referencers 모드 전환 UI는 아직 실제 컨트롤이 아닙니다.
- 결과 영역은 아직 `STreeView`가 아니라 placeholder입니다.
- CSV Export, 필터, 순환 참조 탐지는 아직 구현하지 않았습니다.

## Documentation

- `Docs/02_Architecture/AssetReferenceInspector_Slate_UI_Basics_KR.md`
- `Docs/02_Architecture/AssetReferenceInspector_Code_Organization_KR.md`
- `Docs/03_Verification/Build_Verification_Log_KR.md`
- `Docs/01_Planning/AssetReferenceInspector_Feature_Work_Plan_KR.md`
- `Docs/README_KR.md`

## Related Issues

- References #1
- References #[Architecture Issue]
- References #[Verification Issue]

## Next Step

- 다음 브랜치에서 Content Browser 선택 Asset을 가져와 UI에 표시합니다.
