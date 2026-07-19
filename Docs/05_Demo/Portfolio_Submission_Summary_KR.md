# Portfolio Submission Summary

이 문서는 `AssetReferenceInspector`를 포트폴리오 제출 자료에 설명하기 위한 짧은 요약문을 정리한다.

## 3줄 요약

Unreal Engine 5.4에서 Asset Registry와 Slate UI를 활용해 선택한 Asset의 Dependencies / Referencers 관계를 분석하는 Editor Plugin을 제작했다.

Demo Host 프로젝트에서 독립 Project Plugin으로 개발한 뒤 실제 액션 RPG Portfolio 프로젝트에 적용해 Character, Material, Texture 등 실사용 Asset 참조 관계를 Tree View와 CSV로 확인했다.

순환 참조 표시, Unused Candidate 후보 표시, Asset Size 표시, Content Browser Sync, Path / Class / Content Scope 필터를 구현해 에디터 내 Asset 관리와 분석 흐름을 포트폴리오 기능으로 정리했다.

## 이력서용 한 줄

UE 5.4 Editor Plugin으로 Asset Registry 기반 Dependencies / Referencers 분석 도구를 제작하고, Slate Tree View, 필터, 순환 참조 표시, CSV Export, 실제 Portfolio 프로젝트 적용 검증까지 수행했다.

## 포트폴리오 본문용

`AssetReferenceInspector`는 Unreal Editor에서 Content Browser의 선택 Asset을 기준으로 참조 관계를 분석하는 독립 Editor Plugin이다.

게임 런타임 기능이 아니라 에디터 확장 도구로 설계했으며, Portfolio 프로젝트 전용 gameplay class에 의존하지 않고 `FAssetData`, PackageName, Asset Registry API 중심으로 구현했다. 분석 결과는 Slate `STreeView`로 표시하며, Dependencies / Referencers 모드 전환, Max Depth, Path Filter, Asset Class Filter, Engine / Plugin Content 옵션을 제공한다.

실무형 기능으로는 DFS 현재 경로 기준 순환 참조 표시, referencer 수 0 기반 Unused Candidate 후보 표시, `.uasset`, `.uexp`, `.ubulk` 합산 기반 Asset Size 표시, CSV Export, Tree node 더블 클릭 시 Content Browser Sync를 구현했다. Demo Host에서 기능을 검증한 뒤 실제 액션 RPG Portfolio 프로젝트에 적용해 `BP_CPlayer`, Material, Texture 등 실사용 Asset의 참조 관계를 분석했다.

## 강조 포인트

- Unreal Editor Module과 Project Plugin 구조 이해
- Asset Registry 기반 PackageName 중심 분석
- Slate `STreeView` 기반 계층 UI 구현
- Dependencies / Referencers 양방향 관계 조회
- DFS 기반 Max Depth 제한과 순환 참조 방어
- Path / Class / Engine / Plugin Content 필터
- Content Browser API 연동
- CSV Export와 검증 산출물 관리
- Demo Host에서 개발 후 실제 Portfolio 프로젝트에 적용한 흐름

## 제한 사항 설명

- `Unused Candidate`는 삭제 가능 판정이 아니라 검토 후보 표시다.
- Soft Reference, Asset Manager, 동적 로딩 기반 사용 여부는 Asset Registry 결과만으로 단정하지 않는다.
- Asset Size는 cooked size나 runtime memory size가 아니라 에디터 프로젝트 디스크 파일 크기 기준 추정값이다.
- Node Graph UI는 MVP 범위에서 제외하고 Future Work로 남겼다.

## 관련 문서

- [프로젝트 README](../../README.md)
- [최종 검증 요약](../03_Verification/Final_Verification_Summary_KR.md)
- [Portfolio Demo](Portfolio_Demo_KR.md)
