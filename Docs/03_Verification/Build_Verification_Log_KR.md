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

#### Screenshots

![Nomad Tab Open](Screenshots/feature_ari_nomad_tab/nomad_tab.png)

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

### AssetReferenceInspector 기본 Slate UI Shell

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
[3/6] Compile [x64] SAssetReferenceInspectorWidget.cpp
[4/6] Link [x64] UnrealEditor-AssetReferenceInspector-0006.lib
[5/6] Link [x64] UnrealEditor-AssetReferenceInspector-0006.dll
[6/6] WriteMetadata Portfolio_PlugInEditor.target
Total execution time: 2.69 seconds
```

#### 확인 범위

- `Private/UI/SAssetReferenceInspectorWidget` 추가 상태에서 빌드 확인
- `OnSpawnPluginTab()`이 `SAssetReferenceInspectorWidget`을 탭 내부 루트 위젯으로 사용하는 상태에서 빌드 확인

#### 에디터 UI 확인

- Window 메뉴 클릭 후 `Asset Reference Inspector` 탭 오픈 확인
- `SAssetReferenceInspectorWidget` 기반 기본 UI Shell 표시 확인
- `Selected Asset: None` placeholder 표시 확인
- `Pick Selected Asset` / `Analyze` 버튼 placeholder 표시 확인
- `Mode: Dependencies / Referencers` placeholder 표시 확인
- 결과 영역 placeholder 표시 확인

#### Screenshots

![Slate UI Shell](Screenshots/feature_ari_ui_shell/ui_shell.png)

### AssetReferenceInspector STreeView 더미 데이터

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
[3/7] Compile [x64] Module.AssetReferenceInspector.cpp
[4/7] Compile [x64] SAssetReferenceInspectorWidget.cpp
[5/7] Link [x64] UnrealEditor-AssetReferenceInspector-0004.lib
[6/7] Link [x64] UnrealEditor-AssetReferenceInspector-0004.dll
[7/7] WriteMetadata Portfolio_PlugInEditor.target
Total execution time: 5.97 seconds
```

#### 확인 범위

- 결과 영역 placeholder를 `STreeView`로 교체한 상태에서 빌드 확인
- `BP_Player` 기준 더미 계층 데이터를 위젯 내부에서 생성하는 상태에서 빌드 확인
- `OnGenerateTreeRow`, `OnGetTreeChildren` 콜백 연결 상태에서 빌드 확인

#### 에디터 UI 확인

- `Asset Reference Inspector` 탭 내부 결과 영역이 `STreeView`로 표시됨을 확인
- `BP_Player` 루트 노드 표시 확인
- `SK_Player`, `ABP_Player`, `M_Player`, `PlayerConfig` 자식 노드 표시 확인
- `M_Player` 하위 `Player_D`, `Player_L` 노드 표시 확인
- Tree row 선택 하이라이트 표시 확인
- expand / collapse UI 표시 확인

#### Screenshots

![Tree View Dummy](Screenshots/feature_ari_tree_view_dummy/tree_view_dummy.png)

### AssetReferenceInspector Content Browser 선택 Asset 표시

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
[3/7] Compile [x64] Module.AssetReferenceInspector.cpp
[4/7] Compile [x64] SAssetReferenceInspectorWidget.cpp
[5/7] Link [x64] UnrealEditor-AssetReferenceInspector-0006.lib
[6/7] Link [x64] UnrealEditor-AssetReferenceInspector-0006.dll
[7/7] WriteMetadata Portfolio_PlugInEditor.target
Total execution time: 6.71 seconds
```

#### 확인 범위

- `ContentBrowser`, `AssetRegistry` private dependency 추가 상태에서 빌드 확인
- `Pick Selected Asset` 버튼에 `OnClicked` 핸들러를 연결한 상태에서 빌드 확인
- `IContentBrowserSingleton::GetSelectedAssets` 사용 상태에서 빌드 확인
- 선택 Asset 이름과 PackageName 표시용 텍스트 바인딩 상태에서 빌드 확인

#### 에디터 UI 확인

- Content Browser에서 `BP_Dummy` 선택 후 `Pick Selected Asset` 클릭 시 `Selected Asset: BP_Dummy (/Game/BP_Dummy)` 표시 확인
- Content Browser 선택 Asset이 없는 상태에서 `Pick Selected Asset` 클릭 시 `Selected Asset: None` 표시 확인
- 선택 Asset 표시 변경 후 기존 더미 `STreeView` 표시 유지 확인

#### Screenshots

![Selected Asset](Screenshots/feature_ari_selected_asset/selected_asset.png)

![Selected Asset None](Screenshots/feature_ari_selected_asset/selected_asset_none.png)

### AssetReferenceInspector Dependencies Depth 1 조회

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
[3/7] Compile [x64] Module.AssetReferenceInspector.cpp
[4/7] Compile [x64] SAssetReferenceInspectorWidget.cpp
[5/7] Link [x64] UnrealEditor-AssetReferenceInspector-0001.lib
[6/7] Link [x64] UnrealEditor-AssetReferenceInspector-0001.dll
[7/7] WriteMetadata Portfolio_PlugInEditor.target
Total execution time: 8.24 seconds
```

#### 확인 범위

- `Analyze` 버튼에 `OnClicked` 핸들러를 연결한 상태에서 빌드 확인
- `FAssetRegistryModule`과 `IAssetRegistry::GetDependencies` 사용 상태에서 빌드 확인
- `FAssetReferenceTreeNode` 기반 Tree View 표시 구조로 변경한 상태에서 빌드 확인
- 조회 결과를 `TreeView->RequestTreeRefresh()`로 갱신하는 상태에서 빌드 확인

#### 에디터 UI 확인

- 확인 완료.
- `BP_Dummy` 선택 후 `Pick Selected Asset`, `Analyze` 클릭 시 Tree 루트에 `BP_Dummy`가 표시되는 것을 확인했다.
- `BP_Dummy`의 Dependencies 결과에 `/Script/NavigationSystem`, `Cube`, `M_Dummy`가 표시되는 것을 확인했다.
- `M_Dummy` 선택 후 `Analyze` 클릭 시 Tree 루트에 `M_Dummy`가 표시되고, 자식 노드로 `T_Dummy_Color`가 표시되는 것을 확인했다.
- `T_Dummy_Color` 선택 후 `Analyze` 클릭 시 Tree 루트에 `T_Dummy_Color`가 표시되고, 자식 노드로 `/Script/InterchangeEngine`이 표시되는 것을 확인했다.
- 선택 Asset이 없는 상태에서 `Analyze` 클릭 시 안전하게 처리되는지는 아직 미확인이다.

#### Screenshots

![BP_Dummy Dependencies](Screenshots/feature_ari_dependencies_depth1/bp_dummy_dependencies.png)

![M_Dummy Dependencies](Screenshots/feature_ari_dependencies_depth1/m_dummy_dependencies.png)

![T_Dummy_Color Dependencies](Screenshots/feature_ari_dependencies_depth1/t_dummy_color_dependencies.png)
