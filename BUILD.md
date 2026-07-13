# Build Guide

## 기준 환경

| 항목 | 기준 |
| --- | --- |
| Engine | Unreal Engine 5.4 |
| OS | Windows |
| IDE | Visual Studio |
| Target | `Portfolio_PlugInEditor` |
| Platform | Win64 |
| Configuration | Development |

## Editor Target 빌드

PowerShell에서 저장소 루트 기준으로 실행합니다.

```powershell
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" Portfolio_PlugInEditor Win64 Development -Project="C:\UE5_Portfolio\Portfolio_UE5.4_verGit\Portfolio_PlugIn\Portfolio_PlugIn.uproject" -WaitMutex -FromMsBuild
```

## 확인 순서

1. `git status --short`로 작업 범위를 확인합니다.
2. `.uproject`와 `Plugins/AssetReferenceInspector` 구조를 확인합니다.
3. Editor Target을 빌드합니다.
4. 에디터에서 플러그인 탭이 열리는지 확인합니다.
5. Content Browser에서 Asset을 선택한 뒤 분석 기능을 확인합니다.
6. 결과를 `Docs/03_Verification/`에 기록합니다.

## 빌드 실패 분석 기준

- 첫 번째 컴파일 에러를 우선 확인합니다.
- `.uplugin`의 모듈 Type, LoadingPhase를 확인합니다.
- `Build.cs`에 필요한 Editor 모듈 의존성이 있는지 확인합니다.
- Runtime 모듈에 Editor 전용 API가 들어가지 않았는지 확인합니다.
- 직접 빌드하지 못한 항목은 `미확인`으로 기록합니다.

## Git 제외 대상

다음 경로는 생성물이므로 커밋하지 않습니다.

- `Binaries/`
- `DerivedDataCache/`
- `Intermediate/`
- `Saved/`
- `.vs/`

