# AssetReferenceInspector Demo Host

이 저장소는 Unreal Engine 5.4용 Editor Plugin `AssetReferenceInspector`를 개발하기 위한 Demo Host 프로젝트입니다.

목표는 Content Browser에서 선택한 Asset의 Dependencies / Referencers 관계를 Asset Registry로 분석하고, Slate Tree View 기반 UI로 표시하는 독립 Project Plugin을 만드는 것입니다.

## 빠른 링크

- [Contributor Guide](AGENTS.md)
- [Build Guide](BUILD.md)
- [Docs Index](Docs/README_KR.md)
- [Project Plan](Docs/00_Project/AssetReferenceInspector_Project_Plan_KR.md)
- [MVP Scope](Docs/01_Planning/AssetReferenceInspector_MVP_Scope_KR.md)
- [3 Week Roadmap](Docs/01_Planning/AssetReferenceInspector_Three_Week_Roadmap_KR.md)

## 저장소 구조

```text
Portfolio_PlugIn/
  Portfolio_PlugIn.uproject
  Config/
  Source/Portfolio_PlugIn/
  Plugins/
    AssetReferenceInspector/
  Docs/
```

현재 저장소 루트는 Demo Host 프로젝트입니다. 실제 플러그인 구현은 `Plugins/AssetReferenceInspector` 아래에 둡니다.

## 개발 원칙

- Engine 설치 경로의 `Engine/Plugins`에는 작업하지 않습니다.
- 본 Portfolio 프로젝트 전용 타입에 의존하지 않습니다.
- MVP는 Slate Tree View 기반입니다.
- Node Graph UI는 Future Work로 둡니다.
- `Unused Asset`은 삭제 가능 판정이 아니라 `Unused Candidate`로만 표현합니다.
- Asset Size는 런타임 메모리가 아니라 디스크 파일 기준 추정값으로 표시합니다.

## 현재 상태

- Demo Host UE 프로젝트 생성 완료
- 계획 문서 및 작업 규칙 작성 완료
- `Plugins/AssetReferenceInspector` 구현은 다음 단계에서 진행 예정

## 최종 적용 흐름

1. Demo Host 프로젝트에서 플러그인을 개발하고 검증합니다.
2. `Plugins/AssetReferenceInspector` 폴더를 기존 Portfolio 프로젝트의 `Plugins` 폴더로 복사합니다.
3. 실제 Portfolio Asset을 대상으로 참조 관계를 분석하고 시연 자료를 정리합니다.

