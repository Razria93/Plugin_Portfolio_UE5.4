# AssetReferenceInspector 3주 개발 로드맵

## 1주차: 플러그인 골격, Asset Registry 조회, Tree UI

### 목표

UE 5.4에서 빌드 가능한 Editor Plugin 골격을 만들고, 선택 Asset 기준 Dependencies를 Tree View에 표시한다.

### 작업

- `Plugins/AssetReferenceInspector` 생성
- `AssetReferenceInspector.uplugin` 작성
- Editor 모듈 `AssetReferenceInspector` 생성
- `AssetReferenceInspector.Build.cs` 작성
- 플러그인 Startup / Shutdown 구현
- Nomad Tab Spawner 등록
- Slate 위젯 생성
- `STreeView` 더미 데이터 표시
- Content Browser 선택 Asset 가져오기
- Asset Registry에서 Dependencies 조회
- Depth 1~2 수준 Tree 표시

### 완료 기준

- UE 5.4 Editor 타깃 빌드가 성공한다.
- 에디터에서 Asset Reference Inspector 탭을 열 수 있다.
- 더미 Tree View가 표시된다.
- 선택 Asset의 Dependencies가 Tree View에 표시된다.

## 2주차: 분석 옵션과 탐색 편의 기능

### 목표

Dependencies / Referencers 전환, Max Depth, 필터, Content Browser Sync를 추가한다.

### 작업

- Referencers 모드 추가
- Dependencies / Referencers 전환 UI 추가
- Max Depth 입력 추가
- Asset Class 필터 추가
- Path 필터 추가
- Engine Content 표시 여부 옵션 추가
- Plugin Content 표시 여부 옵션 추가
- Tree 노드 더블 클릭 시 Content Browser Sync 구현
- 분석 결과 갱신 흐름 정리

### 완료 기준

- Dependencies와 Referencers 모드를 전환할 수 있다.
- Max Depth가 결과 Tree에 반영된다.
- Class / Path 필터가 결과에 반영된다.
- Engine Content와 Plugin Content 표시 여부를 제어할 수 있다.
- Tree 노드를 더블 클릭하면 Content Browser에서 해당 Asset이 선택된다.

## 3주차: 실무형 기능, 문서화, 포트폴리오 적용

### 목표

순환 참조, Unused Candidate, Asset Size, CSV Export를 추가하고 Portfolio 프로젝트 적용 시연을 준비한다.

### 작업

- 현재 DFS 경로 기준 순환 참조 탐지
- 순환 참조 노드 UI 표시
- Unused Candidate 판정 추가
- Asset 디스크 크기 표시
- `.uasset`, `.uexp`, `.ubulk` 합산 시도
- CSV Export 구현
- README 작성
- Known Limitations 작성
- Demo Host 프로젝트에서 시연 Asset 분석
- 기존 Portfolio 프로젝트의 `Plugins` 폴더에 플러그인 복사 적용
- BP_Player, Character, Weapon, Material 등 실제 Asset 분석
- 시연 영상 촬영 체크리스트 작성

### 완료 기준

- 순환 참조가 Tree에서 식별된다.
- Unused Candidate가 삭제 확정이 아닌 후보로 표시된다.
- Asset 크기가 디스크 기준 추정값으로 표시된다.
- CSV Export 파일이 생성된다.
- README에 설치, 실행, 사용 방법이 정리된다.
- 실제 Portfolio 프로젝트에서 플러그인 탭이 열리고 Asset 분석이 가능하다.

## 리스크와 대응

### Asset Registry 분석 한계

Asset Registry는 코드 동적 로딩, 문자열 기반 경로, 일부 Asset Manager 규칙을 완전히 보장하지 않는다.

대응:

- README에 Known Limitations로 명시한다.
- UI에서는 확정 표현을 피한다.
- Unused Asset 대신 Unused Candidate 표현을 사용한다.

### 기능 범위 확장

Node Graph UI나 프로젝트 전체 리포트까지 포함하면 3주 범위를 초과할 가능성이 높다.

대응:

- Tree View MVP를 먼저 완료한다.
- Graph UI는 Future Work로 남긴다.

### 대규모 프로젝트 성능

Portfolio 프로젝트에 적용할 때 참조 깊이와 Asset 수가 많으면 UI가 느려질 수 있다.

대응:

- Max Depth 기본값을 낮게 둔다.
- 필터를 먼저 적용한다.
- 필요 시 비동기 분석은 후속 개선으로 분리한다.

