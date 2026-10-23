# Mini Plan: Phase 7-1 CSV Export

## Metadata

- Type: Mini Plan
- Status: Draft
- Branch: `feature/ari-csv-export`
- Related Phase: Phase 7-1. CSV Export
- Related PR: TBD
- Related Issues: TBD
- Source Docs:
  - `Docs/01_Planning/Feature_Work_Plan_KR.md`
  - `Docs/02_Architecture/Asset_Registry_Analysis_Flow_KR.md`
  - `Docs/02_Architecture/Code_Organization_KR.md`
  - `Docs/03_Verification/Build_Verification_Log_KR.md`

## Goal

현재 Tree View에 표시된 분석 결과를 CSV 파일로 저장한다.

이번 작업은 Asset 관계 분석 결과를 에디터 화면 안에서만 확인하는 상태에서, 외부에서 검토 가능한 파일 산출물로 내보내는 첫 Export 기능이다.

## Scope

포함:

- `Export CSV` 버튼 추가
- 현재 `TreeRootItems`를 순회해 CSV row 생성
- `PackageName == NAME_None`인 placeholder / grouping node는 export 대상에서 제외
- CSV 컬럼 순서 구현
- CSV 특수 문자 escape 처리
- `Saved/AssetReferenceInspector/` 아래 timestamp 기반 CSV 파일 저장
- 저장 성공 / 실패를 editor notification 또는 log로 표시
- CSV Export 전용 helper 또는 exporter 파일 추가

제외:

- 파일 저장 경로 선택 dialog
- CSV 컬럼 선택 UI
- Tree View 컬럼 UI
- Excel 전용 formatting
- JSON / Markdown / Graph export
- Export 이후 파일 자동 열기
- Analyzer 클래스 전체 분리

## Target Files

- `Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/Private/Export/AssetReferenceCsvExporter.h`
- `Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/Private/Export/AssetReferenceCsvExporter.cpp`
- `Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/Private/UI/SAssetReferenceInspectorWidget.h`
- `Plugins/AssetReferenceInspector/Source/AssetReferenceInspector/Private/UI/SAssetReferenceInspectorWidget.cpp`
- `Docs/01_Planning/Feature_Work_Plan_KR.md`
- `Docs/02_Architecture/Asset_Registry_Analysis_Flow_KR.md`
- `Docs/02_Architecture/Code_Organization_KR.md`
- `Docs/03_Verification/Build_Verification_Log_KR.md`

## Implementation Steps

1. CSV Exporter 파일을 `Private/Export` 아래에 추가한다.
2. CSV row 컬럼 순서와 node metadata 매핑을 구현한다.
3. Tree node를 재귀 순회하되 `PackageName == NAME_None` row는 제외한다.
4. 문자열 필드는 CSV 규칙에 맞게 escape한다.
5. `Saved/AssetReferenceInspector/AssetReferenceReport_YYYYMMDD_HHMMSS.csv` 경로에 저장한다.
6. `SAssetReferenceInspectorWidget`에 `Export CSV` 버튼과 callback을 추가한다.
7. 저장 성공 / 실패 결과를 사용자에게 알려준다.
8. 빌드와 에디터 수동 확인 후 문서와 검증 로그를 갱신한다.

## Verification

- Build:
  - `Portfolio_PlugInEditor Win64 Development` 빌드 성공
- Editor:
  - Dependencies 결과를 CSV로 저장
  - Referencers 결과를 CSV로 저장
  - Unused Candidate 결과를 CSV로 저장
  - placeholder / grouping node가 CSV에서 제외되는지 확인
  - `ParentPackage` 값이 relation child row에 기록되는지 확인
  - 저장 성공 또는 실패 알림 확인
- Unverified:
  - 파일 저장 경로 선택 dialog는 이번 범위에서 제외
  - Excel 표시 최적화는 이번 범위에서 검증하지 않음

## Documentation Check

- `Feature_Work_Plan_KR.md`에 Phase 7-1 완료 기준과 진행 기록을 반영한다.
- `Asset_Registry_Analysis_Flow_KR.md`에 CSV Export 흐름과 컬럼 매핑을 정리한다.
- `Code_Organization_KR.md`에 `Private/Export` 책임과 exporter 파일 배치를 반영한다.
- `Build_Verification_Log_KR.md`에 빌드와 에디터 수동 확인 결과를 기록한다.
- 검증 스크린샷은 필요 시 `Docs/03_Verification/Screenshots/feature_ari_csv_export/`에 승격한다.

## Commit Plan

- CSV Export 구현, 문서, 검증 로그를 하나의 feature commit으로 묶는 것을 우선한다.
- 추천 메시지:

```text
feat: 분석 결과 CSV Export 추가
```
