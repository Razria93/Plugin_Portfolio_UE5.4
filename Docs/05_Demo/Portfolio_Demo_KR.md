# Portfolio Demo Plan

이 문서는 `AssetReferenceInspector`를 실제 Portfolio 프로젝트에 적용하기 위한 시연 흐름을 정리한다.

현재 문서는 시연 계획과 촬영 체크리스트다. 실제 Portfolio 프로젝트 복사 적용, 실사용 Asset 분석 결과, 최종 검증은 후속 Phase에서 별도로 기록한다.

## 목표

- Demo Host에서 구현한 Editor Plugin을 Portfolio 프로젝트에 적용하는 흐름을 설명한다.
- 실제 Asset의 Dependencies / Referencers 관계를 Tree View로 분석하는 시연 순서를 정한다.
- 포트폴리오 제출 시 강조할 기술 포인트와 제한 사항을 명확히 한다.

## 시연 메시지 후보

```text
Unreal Engine의 Asset Registry와 Slate UI를 활용해, 선택한 Asset의 Dependencies / Referencers를 분석하고 Tree View로 시각화하는 독립 Editor Plugin을 제작했다.

Demo Host 프로젝트에서 플러그인을 개발한 뒤 실제 액션 RPG Portfolio 프로젝트에 적용하여 Character, Weapon, Material 등 실사용 Asset 참조 관계를 분석하는 흐름을 준비했다.
```

## 적용 준비

대상:

- Demo Host: `Portfolio_PlugIn`
- 플러그인: `Plugins/AssetReferenceInspector`
- 적용 대상: 기존 Portfolio 프로젝트

준비 순서:

1. Demo Host에서 `Plugins/AssetReferenceInspector` 폴더를 확인한다.
2. 대상 Portfolio 프로젝트 루트에 `Plugins` 폴더를 준비한다.
3. `AssetReferenceInspector` 폴더를 대상 프로젝트 `Plugins` 아래로 복사한다.
4. 대상 프로젝트를 열고 Editor Target 빌드를 수행한다.
5. Unreal Editor에서 `Window > Asset Reference Inspector` 메뉴가 열리는지 확인한다.

## Demo Host 시연 순서

Demo Host에서는 검증용 Asset fixture를 기준으로 기능을 빠르게 보여준다.

1. `BP_Dummy` 선택
2. `Pick Selected Asset`
3. Dependencies mode에서 `Analyze`
4. `BP_Dummy -> M_Dummy -> T_Dummy_Color` Tree 표시 확인
5. Referencers mode로 전환
6. `T_Dummy_Color` 선택 후 `Analyze`
7. `T_Dummy_Color -> M_Dummy -> BP_Dummy` Tree 표시 확인
8. Path / Class Filter 적용
9. 순환 검증용 `BP_CycleA` 분석
10. `[Circular]` 표시 확인
11. `Scan Unused Candidate`
12. `Export CSV`

## Portfolio 프로젝트 시연 순서

실제 Portfolio 프로젝트에서는 다음 흐름을 우선 후보로 둔다.

1. 대상 Character Blueprint 선택
2. Dependencies 분석
3. Skeletal Mesh, Animation Blueprint, Material, Texture, Data Asset 관계 확인
4. Material 또는 Texture 선택
5. Referencers 분석
6. 해당 Asset이 어떤 Blueprint / Material Instance에서 사용되는지 확인
7. Path Filter로 `/Game/<PortfolioContentRoot>` 범위 제한
8. Class Filter로 `Material`, `Texture`, `Blueprint` 등 특정 Class 확인
9. Tree 노드 더블 클릭으로 Content Browser Sync 확인
10. CSV Export로 분석 결과 저장

## 촬영 체크리스트

- 플러그인 탭이 Window 메뉴에서 열리는 장면
- Content Browser에서 Asset을 선택하고 `Pick Selected Asset`을 누르는 장면
- Dependencies Tree가 펼쳐지는 장면
- Referencers Tree가 펼쳐지는 장면
- Max Depth / Path Filter / Class Filter가 결과를 제한하는 장면
- 순환 참조 후보에 `[Circular]`가 표시되는 장면
- Asset Size suffix가 표시되는 장면
- Unused Candidate 후보 목록이 표시되는 장면
- Tree 노드 더블 클릭 후 Content Browser가 해당 Asset으로 이동하는 장면
- CSV Export 성공 notification과 저장된 CSV 샘플

## 강조할 기술 포인트

- Unreal Editor 전용 Plugin 구조
- Asset Registry 기반 Dependencies / Referencers 조회
- DFS 기반 Max Depth 제한과 순환 방어
- Slate `STreeView` 기반 계층 UI
- Path / Class / Engine / Plugin Content 필터
- Content Browser 모듈과의 에디터 연동
- CSV Export와 디스크 파일 크기 계산
- 독립 Project Plugin으로 다른 UE 프로젝트에 복사 가능한 구조

## 제한 사항

- Soft Reference와 Asset Manager 규칙은 별도 정책 없이는 누락될 수 있다.
- Unused Candidate는 삭제 가능 판정이 아니라 검토 후보 표시다.
- Asset Size는 에디터 프로젝트의 디스크 파일 크기 기준 추정값이다.
- Plugin Content positive case 검증에는 Content mount를 가진 테스트 플러그인 Asset dependency fixture가 필요하다.
- Node Graph UI는 이번 MVP 범위가 아니라 Future Work다.
