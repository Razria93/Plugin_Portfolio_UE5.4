# 빌드 검증 기록

## 2026-07-13

### AssetReferenceInspector Editor Plugin 골격

#### 대상

- 프로젝트: `Portfolio_PlugIn`
- 타깃: `Portfolio_PlugInEditor`
- 플랫폼: `Win64`
- 구성: `Development`
- Engine: Unreal Engine 5.4

#### 명령

```powershell
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" Portfolio_PlugInEditor Win64 Development -Project="C:\UE5_Portfolio\Portfolio_UE5.4_verGit\Portfolio_PlugIn\Portfolio_PlugIn.uproject" -WaitMutex -FromMsBuild
```

#### 결과

성공.

UBT 출력 기준:

```text
[1/4] Compile [x64] AssetReferenceInspectorModule.cpp
[2/4] Link [x64] UnrealEditor-AssetReferenceInspector.lib
[3/4] Link [x64] UnrealEditor-AssetReferenceInspector.dll
[4/4] WriteMetadata Portfolio_PlugInEditor.target
Total execution time: 4.38 seconds
```

#### 확인 범위

- Demo Host Editor Target 빌드 확인
- `AssetReferenceInspector` Editor Plugin 최소 골격 포함 상태에서 빌드 확인

### AssetReferenceInspector Nomad Tab 등록

#### 대상

- 프로젝트: `Portfolio_PlugIn`
- 타깃: `Portfolio_PlugInEditor`
- 플랫폼: `Win64`
- 구성: `Development`
- Engine: Unreal Engine 5.4

#### 명령

```powershell
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" Portfolio_PlugInEditor Win64 Development -Project="C:\UE5_Portfolio\Portfolio_UE5.4_verGit\Portfolio_PlugIn\Portfolio_PlugIn.uproject" -WaitMutex -FromMsBuild
```

#### 결과

성공.

UBT 출력 기준:

```text
Target is up to date
Total execution time: 0.77 seconds
```

#### 확인 범위

- `AssetReferenceInspector` 모듈에 Nomad Tab Spawner 등록/해제 코드 포함 상태에서 빌드 확인
- `SDockTab`, `SBox`, `STextBlock` 기반 최소 Slate 콘텐츠 포함 상태에서 빌드 확인

#### 미확인

- 메뉴/툴바 진입점은 이번 피처 범위가 아니므로 에디터 UI에서 직접 탭을 여는 검증은 수행하지 않음

### AssetReferenceInspector 에디터 창 열기

#### 대상

- 프로젝트: `Portfolio_PlugIn`
- 타깃: `Portfolio_PlugInEditor`
- 플랫폼: `Win64`
- 구성: `Development`
- Engine: Unreal Engine 5.4

#### 명령

```powershell
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" Portfolio_PlugInEditor Win64 Development -Project="C:\UE5_Portfolio\Portfolio_UE5.4_verGit\Portfolio_PlugIn\Portfolio_PlugIn.uproject" -WaitMutex -FromMsBuild
```

#### 결과

성공.

일반 실행은 UBT 로그 파일 백업 단계에서 `UnauthorizedAccessException`으로 실패했다. 동일 명령을 권한 상승으로 재실행해 실제 컴파일/링크를 확인했다.

UBT 출력 기준:

```text
[2/7] Compile [x64] AssetReferenceInspectorCommands.cpp
[4/7] Compile [x64] AssetReferenceInspectorModule.cpp
[5/7] Link [x64] UnrealEditor-AssetReferenceInspector-0002.lib
[6/7] Link [x64] UnrealEditor-AssetReferenceInspector-0002.dll
[7/7] WriteMetadata Portfolio_PlugInEditor.target
Total execution time: 5.47 seconds
```

#### 확인 범위

- `FAssetReferenceInspectorCommands` 추가 상태에서 빌드 확인
- `FUICommandList` 기반 메뉴 Command 매핑 포함 상태에서 빌드 확인
- `ToolMenus` 기반 `LevelEditor.MainMenu.Window` 메뉴 진입점 등록 코드 포함 상태에서 빌드 확인
- 메뉴 Command에서 `FGlobalTabmanager::TryInvokeTab` 호출 코드 포함 상태에서 빌드 확인

#### 에디터 UI 확인

- Window 메뉴 클릭 후 `Asset Reference Inspector` 탭 오픈 확인
- 탭 내부 중앙에 `Asset Reference Inspector` 텍스트 표시 확인

### AssetReferenceInspector Commands Public API 정리

#### 대상

- 프로젝트: `Portfolio_PlugIn`
- 타깃: `Portfolio_PlugInEditor`
- 플랫폼: `Win64`
- 구성: `Development`
- Engine: Unreal Engine 5.4

#### 명령

```powershell
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" Portfolio_PlugInEditor Win64 Development -Project="C:\UE5_Portfolio\Portfolio_UE5.4_verGit\Portfolio_PlugIn\Portfolio_PlugIn.uproject" -WaitMutex -FromMsBuild
```

#### 결과

성공.

일반 실행은 UBT 로그 파일 백업 단계에서 `UnauthorizedAccessException`으로 실패했다. 동일 명령을 권한 상승으로 재실행해 실제 컴파일/링크를 확인했다.

UBT 출력 기준:

```text
[2/7] Compile [x64] AssetReferenceInspectorCommands.cpp
[4/7] Compile [x64] Module.AssetReferenceInspector.cpp
[5/7] Link [x64] UnrealEditor-AssetReferenceInspector-0001.lib
[6/7] Link [x64] UnrealEditor-AssetReferenceInspector-0001.dll
[7/7] WriteMetadata Portfolio_PlugInEditor.target
Total execution time: 5.18 seconds
```

#### 확인 범위

- `AssetReferenceInspectorCommands.h`를 `Public`에서 `Private`로 이동한 상태에서 빌드 확인
- Slate / Commands 관련 의존성을 `PrivateDependencyModuleNames`에 유지한 상태에서 빌드 확인

## 2026-07-14

### 코드 구성 규칙 반영

#### 대상

- 프로젝트: `Portfolio_PlugIn`
- 타깃: `Portfolio_PlugInEditor`
- 플랫폼: `Win64`
- 구성: `Development`
- Engine: Unreal Engine 5.4

#### 명령

```powershell
& "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" Portfolio_PlugInEditor Win64 Development -Project="C:\UE5_Portfolio\Portfolio_UE5.4_verGit\Portfolio_PlugIn\Portfolio_PlugIn.uproject" -WaitMutex -FromMsBuild
```

#### 결과

성공.

일반 실행은 UBT 로그 파일 백업 단계에서 `UnauthorizedAccessException`으로 실패했다. 동일 명령을 권한 상승으로 재실행해 실제 컴파일/링크를 확인했다.

UBT 출력 기준:

```text
[2/7] Compile [x64] AssetReferenceInspectorCommands.cpp
[4/7] Compile [x64] AssetReferenceInspectorModule.cpp
[5/7] Link [x64] UnrealEditor-AssetReferenceInspector-0003.lib
[6/7] Link [x64] UnrealEditor-AssetReferenceInspector-0003.dll
[7/7] WriteMetadata Portfolio_PlugInEditor.target
Total execution time: 3.84 seconds
```

#### 확인 범위

- `AssetReferenceInspectorModule.h` 섹션 정리 후 빌드 확인
- `AssetReferenceInspectorModule.cpp` include 그룹 정리 후 빌드 확인
- `AssetReferenceInspectorCommands.h` 섹션 정리 후 빌드 확인
- `AssetReferenceInspectorCommands.cpp` `UI_COMMAND` 포맷 정리 후 빌드 확인
