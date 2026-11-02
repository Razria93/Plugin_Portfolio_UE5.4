# 기능·원칙·개발 자료

[프로젝트 README](../../README.md)

<a id="development-principles"></a>

## 개발 원칙

- Project Plugin 구조를 유지하고 Portfolio 전용 게임플레이 타입에 의존하지 않습니다.
- Asset Registry 기반 `FAssetData`와 PackageName 중심으로 분석합니다.
- MVP는 Slate Tree View 기반입니다.
- Node Graph UI는 Future Work로 둡니다.
- `Unused Asset`은 삭제 가능 판정이 아니라 `Unused Candidate`로만 표현합니다.
- Asset Size는 런타임 메모리 크기가 아니라 에디터 프로젝트의 디스크 파일 크기 기준 추정값입니다.



### 주요 기능

Demo Host의 구현 기능입니다. 기능별 검증 조건은 개발 당시 검증 자료와 별도 QA 기록을 확인합니다.

- Editor Window 메뉴 기반 Nomad Tab
- Slate 기반 분석 UI와 `STreeView` 결과 표시
- Content Browser 선택 Asset 가져오기
- Dependencies / Referencers 모드 전환
- Max Depth 기반 재귀 Tree 생성
- Path / Asset Class 필터
- Engine / Plugin Content 표시 옵션
- Tree 노드 더블 클릭 시 Content Browser Sync
- 순환 참조 표시
- Asset 디스크 크기 표시
- Unused Candidate 전체 스캔
- CSV Export

### 개발 문서

- [작업 지침](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/AGENTS.md)
- [빌드 가이드](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/BUILD.md)
- [문서 인덱스](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/README_KR.md)
- [프로젝트 계획](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/00_Overview/Project_Plan_KR.md)
- [MVP 범위](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/00_Overview/MVP_Scope_KR.md)
- [Feature 작업 계획](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/01_Planning/Feature_Work_Plan_KR.md)
- [최종 검증 요약](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/03_Verification/Final_Verification_Summary_KR.md)
- [검증 로그](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/03_Verification/Build_Verification_Log_KR.md)
- [플러그인 README](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Plugins/AssetReferenceInspector/README.md)
- [포트폴리오 시연 계획](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/05_Demo/Portfolio_Demo_KR.md)
- [포트폴리오 제출 요약](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/05_Demo/Portfolio_Submission_Summary_KR.md)

### 개발 당시 검증 자료

- [Dependencies Depth 1](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/03_Verification/Screenshots/feature_ari_dependencies_depth1/bp_dummy_dependencies.png)
- [Referencers Mode](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/03_Verification/Screenshots/feature_ari_referencers_mode/referencers_t_dummy_color.png)
- [Content Browser Sync](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/03_Verification/Screenshots/feature_ari_content_browser_sync/sync_m_dummy_from_dependencies.png)
- [CSV Export](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/03_Verification/Screenshots/feature_ari_csv_export/csv_export_dependencies_success.png)
- [Unused Candidate](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/03_Verification/Screenshots/feature_ari_unused_candidate_indicator/unused_candidates_all_project_content.png)

### Portfolio 적용 흐름

최종 시연 단계에서는 `Plugins/AssetReferenceInspector` 폴더를 기존 Portfolio 프로젝트의 `Plugins` 폴더로 복사해 적용합니다.

적용 후 Character, Weapon, Material, Texture 등 실사용 Asset의 Dependencies / Referencers 관계를 분석하고, Tree View와 CSV 결과를 시연 자료로 정리합니다.

실제 Portfolio 프로젝트 적용 결과는 [포트폴리오 시연 문서](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/05_Demo/Portfolio_Demo_KR.md)에 정리합니다.

### 상세 조작

1. Unreal Editor에서 `Window > Asset Reference Inspector`를 엽니다.
2. Content Browser에서 분석할 Asset을 선택합니다.
3. `Pick Selected Asset`을 클릭합니다.
4. Dependencies 또는 Referencers 모드를 선택합니다.
5. 필요하면 Max Depth, Path Filter, Class Filter, Engine / Plugin Content 표시 옵션을 조정합니다.
6. `Analyze`를 클릭합니다.
7. Tree View에서 참조 관계를 확인합니다.
8. Tree 노드를 더블 클릭해 Content Browser Sync를 수행합니다.
9. 필요하면 `Export CSV`로 현재 결과를 저장합니다. 저장 위치는 프로젝트의 `Saved/AssetReferenceInspector` 폴더입니다.

Unused Candidate 후보는 `Scan Unused Candidate`로 별도 스캔합니다.
