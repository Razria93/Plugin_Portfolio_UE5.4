# Screenshot Workflow

이 문서는 검증 스크린샷의 임시 보관, Docs 승격, GitHub 게시 기준을 정의한다.

## 기본 원칙

검증 스크린샷은 기능 동작을 설명하는 핵심 증거일 때만 Git에 포함한다.

임시 캡처, 중복 이미지, 실패한 캡처 후보는 `LocalNotes/screenshots`에만 둔다. PR 본문, 검증 로그, 과거 PR 보강 댓글에 사용할 최종 이미지만 `Docs/03_Verification/Screenshots/<branch-name>/`로 승격한다.

## 보관 위치

임시 보관:

```text
LocalNotes/screenshots/
```

Git 포함 대상:

```text
Docs/03_Verification/Screenshots/<branch-name>/
```

예:

```text
Docs/03_Verification/Screenshots/feature_ari_dependencies_depth1/
  bp_dummy_dependencies.png
  m_dummy_dependencies.png
  t_dummy_color_dependencies.png
```

## 파일명 규칙

스크린샷 파일명은 snake_case를 사용하고 공백을 두지 않는다.

권장 기준:

- 검증 대상 asset 또는 기능 이름을 포함한다.
- before / after 비교가 필요하면 접미사로 구분한다.
- 최종 결과 이미지가 별도로 필요하면 `final_result`를 사용한다.

예:

```text
game_filter_before.png
game_filter_after.png
cycle_guard_before.png
cycle_guard_after.png
final_result.png
```

## 승격 기준

다음 조건을 만족할 때만 `Docs/03_Verification/Screenshots/<branch-name>/`로 승격한다.

- PR 또는 검증 로그에서 실제로 참조할 이미지다.
- 기능 완료 조건을 설명하는 데 필요하다.
- 같은 의미의 이미지가 중복되지 않는다.
- 파일명이 기능과 검증 의도를 설명한다.

정책 판단을 보여줘야 하는 경우 before / after 이미지를 함께 남긴다. 예를 들어 필터 적용 전후, 순환 방어 전후처럼 구현 선택의 근거가 되는 장면은 두 이미지를 모두 보관할 수 있다.

## Docs 링크 기준

`Build_Verification_Log_KR.md` 같은 저장소 내부 문서에서는 상대 경로를 사용한다.

```markdown
![Description](Screenshots/<screenshot-folder>/<image>.png)
```

## GitHub 링크 기준

GitHub PR / Issue / 댓글로 게시될 draft와 public 본문에는 상대 경로가 아니라 repo 이미지 직접 주소를 사용한다.

PR 작성/검토 중에는 브랜치 기준 직접 주소를 사용한다.

```markdown
![Description](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/<git-branch>/Docs/03_Verification/Screenshots/<screenshot-folder>/<image>.png?raw=true)
```

merge 이후 장기 보존용 public 본문이나 과거 PR 보강 댓글에는 `main` 기준 직접 주소를 사용한다.

```markdown
![Description](https://github.com/Razria93/Plugin_Portfolio_UE5.4/blob/main/Docs/03_Verification/Screenshots/<screenshot-folder>/<image>.png?raw=true)
```

GitHub public 본문의 `Screenshots` 섹션 구조, 이미지 설명 문장, 이미지 URL 검수 기준은 `Docs/04_Governance/GitHub_Public_Body_Validation_Rules_KR.md`를 따른다.

## 과거 PR 보강

이미 merge된 과거 PR은 본문을 재작성하지 않고 댓글로 검증 스크린샷을 보강한다.

과거 PR 보강 댓글에는 다음을 명시한다.

- 현재 프로젝트 상태에서 해당 PR의 완료 조건을 재확인한 기록
- Docs에 보관된 이미지의 `main` 기준 직접 링크
- 이미지가 무엇을 증명하는지에 대한 짧은 설명
