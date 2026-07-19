# 최종 검증 요약

이 문서는 Demo Host 프로젝트 기준으로 `AssetReferenceInspector`의 구현 기능이 어디까지 검증되었는지 요약한다.

상세 빌드 출력, feature별 수동 확인 기록, 스크린샷 원문은 `Build_Verification_Log_KR.md`와 `Screenshots/`를 기준으로 한다.

## 기준

- 프로젝트: `Portfolio_PlugIn`
- Engine: Unreal Engine 5.4
- 대상 플러그인: `Plugins/AssetReferenceInspector`
- 검증 범위: Demo Host 프로젝트의 Editor Plugin 기능
- 제외 범위: 실제 Portfolio 프로젝트 적용 결과는 Phase 7-4에서 별도로 기록

## 요약

- 에디터 Window 메뉴에서 `Asset Reference Inspector` Nomad Tab 오픈을 확인했다.
- Content Browser 선택 Asset을 가져와 현재 분석 대상으로 표시하는 흐름을 확인했다.
- Dependencies / Referencers 모드 전환과 Tree View 표시를 확인했다.
- Max Depth, Path Filter, Asset Class Filter, Engine / Plugin Content 옵션의 기본 동작을 확인했다.
- Tree node 더블 클릭 시 Content Browser Sync가 동작하는 것을 확인했다.
- 순환 참조 표시, Asset Size 표시, Unused Candidate 스캔, CSV Export를 확인했다.
- Demo Host 기준으로 Portfolio 적용 전 MVP 기능 검증은 완료 상태로 본다.

## 기능별 검증 결과

| 영역 | 확인 결과 | 대표 자료 | 남은 제한 |
| --- | --- | --- | --- |
| Editor Window | Window 메뉴에서 독립형 Nomad Tab 오픈 확인 | [Nomad Tab](Screenshots/feature_ari_nomad_tab/nomad_tab.png) | 없음 |
| Slate UI Shell | 분석 옵션과 Tree View 영역 표시 확인 | [UI Shell](Screenshots/feature_ari_ui_shell/ui_shell.png) | 세부 UI polish는 후속 개선 가능 |
| 선택 Asset 연동 | Content Browser 선택 Asset 표시와 선택 없음 상태 확인 | [Selected Asset](Screenshots/feature_ari_selected_asset/selected_asset.png) | 없음 |
| Dependencies | `BP_Dummy -> M_Dummy -> T_Dummy_Color` 관계 표시 확인 | [Dependencies](Screenshots/feature_ari_dependencies_depth1/bp_dummy_dependencies.png) | Asset Registry가 제공하는 dependency 기준 |
| Referencers | `T_Dummy_Color` 기준 상위 참조 Asset 표시 확인 | [Referencers](Screenshots/feature_ari_referencers_mode/referencers_t_dummy_color.png) | Soft Reference / 동적 로딩 사용 여부는 별도 해석 필요 |
| Max Depth | Depth 0, 1, 2 표시 차이 확인 | [Max Depth 2](Screenshots/refactor_ari_analysis_options_ui/analysis_options_ui_max_depth_2.png) | UI 입력 최대값은 실무 안전값 기준으로 제한 |
| Path Filter | `/Game/`, `/Script/`, 빈 값, no match 상태 확인 | [Path Filter Game](Screenshots/feature_ari_path_filter/path_filter_game_only.png) | 문자열 prefix 기반 필터 |
| Asset Class Filter | `Material`, `Texture2D`, `Blueprint` 필터 확인 | [Class Filter](Screenshots/feature_ari_class_filter/class_filter_material_only.png) | 하위 노드가 필터를 통과하지 않으면 그 아래 관계도 표시되지 않음 |
| Engine / Plugin Content | Engine Content 포함 옵션과 Plugin Content no matching 상태 확인 | [Engine Content](Screenshots/feature_ari_engine_plugin_content_options/engine_plugin_options_include_engine_content.png) | content-mounted plugin dependency fixture는 별도 구성 필요 |
| Content Browser Sync | Tree node 더블 클릭 시 Content Browser 선택 동기화 확인 | [Content Browser Sync](Screenshots/feature_ari_content_browser_sync/sync_m_dummy_from_dependencies.png) | placeholder / non-asset node는 무시 |
| Circular Reference | 순환 후보 node에 `[Circular]` 표시 확인 | [Circular Reference](Screenshots/feature_ari_circular_reference_indicator/circular_reference_indicator_cycle_node.png) | 순환 구간은 표시하되 하위 재확장은 중단 |
| Asset Size | `.uasset`, `.uexp`, `.ubulk` 합산 기반 크기 표시 확인 | [Asset Size](Screenshots/feature_ari_asset_size_display/asset_size_dependency_tree.png) | cooked 크기 또는 런타임 메모리 크기가 아님 |
| Unused Candidate | `/Game` 전체 스캔 기반 후보 표시와 필터 적용 확인 | [Unused Candidate](Screenshots/feature_ari_unused_candidate_indicator/unused_candidates_all_project_content.png) | 삭제 가능 판정이 아니라 후보 표시 |
| CSV Export | Dependencies, Referencers, Unused Candidate CSV 저장 확인 | [CSV Export](Screenshots/feature_ari_csv_export/csv_export_dependencies_success.png) | 외부 spreadsheet 도구 열기 검증은 수행하지 않음 |

## Export 샘플

- [Dependencies CSV](Exports/feature_ari_csv_export/csv_export_dependencies.csv)
- [Referencers CSV](Exports/feature_ari_csv_export/csv_export_referencers.csv)
- [Unused Candidates CSV](Exports/feature_ari_csv_export/csv_export_unused_candidates.csv)

## 남은 제한

- Soft Reference, Asset Manager, 동적 로딩 기반 참조는 Asset Registry 결과만으로 삭제 가능 여부를 단정하지 않는다.
- `Unused Candidate`는 사용되지 않는 Asset의 후보 표시이며 삭제 가능 판정이 아니다.
- Asset Size는 에디터 프로젝트의 디스크 파일 크기 기준 추정값이다.
- Plugin Content 옵션은 content-mounted plugin dependency fixture가 없으면 positive case가 제한적이다.
- 실제 Portfolio 프로젝트 적용 결과와 CSV 샘플은 `Docs/05_Demo/Portfolio_Demo_KR.md`에서 별도 기록한다.

## 다음 단계

- Demo Host 검증 결과와 실제 Portfolio 프로젝트 적용 결과는 문서상에서 구분해 유지한다.
- 실제 Portfolio 프로젝트 적용 결과는 [Portfolio Demo](../05_Demo/Portfolio_Demo_KR.md)를 기준으로 확인한다.
