# 검증 기록

이 폴더는 `AssetReferenceInspector` 개발 중 수행한 빌드, 에디터 실행, 기능 검증 결과를 기록한다.

직접 확인하지 않은 항목은 성공으로 적지 않고 `미확인`으로 남긴다.

## 기록 대상

- UE 5.4 Editor Target 빌드 결과
- 플러그인 탭 오픈 여부
- Content Browser 선택 Asset 연동
- Dependencies / Referencers 분석 결과
- Max Depth 및 필터 동작
- CSV Export 결과
- 실제 Portfolio 프로젝트 적용 결과

## 스크린샷 보관 기준

검증 스크린샷은 기능 동작을 설명하는 핵심 증거일 때만 Git에 포함한다.

임시 캡처, 중복 이미지, 실패한 캡처 후보는 `LocalNotes/screenshots`에만 둔다. PR 본문, 검증 로그, 과거 PR 보강 댓글에 사용할 최종 이미지만 `Docs/03_Verification/Screenshots/<branch-name>/`로 승격한다.

파일명은 snake_case를 사용하고 공백을 두지 않는다.

예:

```text
Docs/03_Verification/Screenshots/feature_ari_dependencies_depth1/
  bp_dummy_dependencies.png
  m_dummy_dependencies.png
  t_dummy_color_dependencies.png
```

현재 또는 미래 PR은 PR 본문 `Screenshots` 섹션에서 Docs 이미지를 링크한다.

이미 merge된 과거 PR은 본문을 재작성하지 않고 댓글로 검증 스크린샷을 보강한다. 이때 댓글에는 현재 프로젝트 상태에서 해당 PR의 완료 조건을 재확인한 기록임을 명시하고, Docs에 보관된 이미지 링크를 사용한다.

## 권장 파일

- `Build_Verification_Log_KR.md`
- `Editor_UI_Verification_KR.md`
- `Portfolio_Project_Application_Log_KR.md`
