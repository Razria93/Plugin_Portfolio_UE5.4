# Debug Overlay Snapshot / HUD Flow

이 문서는 `FDebugOverlaySnapshotStore`와 `ACDebugOverlayHUD`가 디버그 오버레이 데이터를 수집하고 화면에 표시하는 흐름을 정리한다.

두 소스의 책임은 분리되어 있다.

```text
게임플레이 코드
-> FDebugOverlaySnapshotStore::RecordXXX
-> World별 Snapshot / EventRing 저장
-> ACDebugOverlayHUD::DrawHUD
-> Snapshot 복사
-> Canvas HUD 텍스트 표시
```

## 전체 구조

`FDebugOverlaySnapshotStore`는 디버그 증거 데이터를 저장하는 Store다. 실행 판단, 전투 결과, AI 요청 결과를 World별로 기록한다.

`ACDebugOverlayHUD`는 Store에 저장된 Snapshot과 현재 Pawn Component 상태를 읽어서 화면에 텍스트로 그리는 View다.

```text
FDebugOverlayWorldStore
├─ Snapshot
│  ├─ LastExecution
│  ├─ LastCombat
│  ├─ LastAI
│  └─ RecentEvents
├─ EventRing
├─ NextEventIndex
└─ EventCount
```

`LastExecution`, `LastCombat`, `LastAI`는 각 영역의 최신 상태 요약 1개를 보관한다. `EventRing`은 최근 이벤트 원본을 최대 32개까지 저장한다. `Snapshot.RecentEvents`는 그중 HUD에 표시할 최근 이벤트 복사본이며 최대 5개다.

## Console Variable

디버그 오버레이는 Shipping 빌드에서 비활성화된다. Non-shipping 빌드에서는 console variable로 수집과 표시를 분리한다.

```text
Portfolio.DebugOverlay.Enabled
```

HUD 표시 여부를 제어한다. `1`이면 `ACDebugOverlayHUD::DrawHUD`가 디버그 텍스트를 그린다.

```text
Portfolio.DebugOverlay.Collect
```

Snapshot 수집 여부를 제어한다. `1`이면 `RecordXXX` API들이 실제 Store에 데이터를 기록한다.

```text
Portfolio.DebugOverlay.EventLogLimit
```

HUD에 표시할 최근 이벤트 줄 수를 정한다. 값은 `0~5` 범위로 clamp된다.

## World Store

Store는 `TMap<TObjectKey<UWorld>, FDebugOverlayWorldStore>`로 관리된다. PIE, Editor World, Game World처럼 여러 World가 있을 수 있으므로 World 단위로 Snapshot을 분리한다.

`ResolveWorld`는 입력으로 들어온 `UObject`를 `UWorld`로 정규화한다. 입력 자체가 `UWorld`면 그대로 사용하고, Actor나 Component 같은 객체면 `GetWorld()`로 World를 얻는다.

`FindStore`는 기존 Store만 찾고, `FindOrAddStore`는 없으면 새 Store를 만든다. 기록 API는 `FindOrAddStore`를 사용하고, 조회 API는 `FindStore`를 사용한다.

## Event Ring

`AddEventInternal`은 이벤트 로그를 ring buffer에 추가한다.

```cpp
if (InStore.EventRing.Num() < DebugOverlayEventStoreCapacity)
{
	InStore.EventRing.Add(InEntry);
}
else
{
	InStore.EventRing[InStore.NextEventIndex] = InEntry;
}
```

처음 32개까지는 배열에 추가한다. 32개가 찬 뒤에는 `NextEventIndex` 위치를 새 이벤트로 덮어쓴다.

```cpp
InStore.NextEventIndex = (InStore.NextEventIndex + 1) % DebugOverlayEventStoreCapacity;
```

다음 기록 위치를 한 칸 이동한다. `% DebugOverlayEventStoreCapacity` 때문에 마지막 index 다음에는 다시 0번으로 돌아간다.

```cpp
InStore.EventCount = FMath::Min(InStore.EventCount + 1, DebugOverlayEventStoreCapacity);
```

현재 유효 이벤트 수를 증가시키되 최대 32를 넘지 않게 한다.

마지막으로 `GetRecentEventsCopyFromStore`를 호출해 HUD 표시용 최근 이벤트를 `Snapshot.RecentEvents`에 복사한다.

```text
EventRing = 최대 32개 원본 이벤트 저장소
Snapshot.RecentEvents = HUD 표시용 최근 N개 복사본
```

## Snapshot 기록 API

`RecordExecutionDecision`은 실행 판단 결과를 `Snapshot.LastExecution`에 기록한다. Domain, Decision, ApplyMode, RejectReason을 summary 문자열로 묶고, Event Log에는 `Execution` category로 추가한다.

`RecordWeaponCollisionWindow`는 무기 collision window 상태를 `Snapshot.LastCombat`에 기록한다. HitWindowId, HitWindowState, CollisionName, Reason을 요약한다.

`RecordCombatTargetPacket`은 `FCombatSignalTargetPacket` 기준 전투 판정 결과를 기록한다. SourceActor, TargetActor, DamageCauser, DefenseOutcome, FinalTakenDamage, CommittedDamage, Accepted 여부를 사용한다.

`RecordCombatResult`는 수신자 기준 최종 전투 결과를 기록한다. `FCombatResultPacket`의 SourceActor, TargetActor, DamageCauser, DefenseOutcome, bDamageCommitted, CommittedDamage를 사용한다.

`RecordAICombatTask`는 AI combat task 요청 결과를 `Snapshot.LastAI`에 기록한다. AIController, OwnerPawn, TargetActor, Intent, RequestResult, RejectReason을 요약한다.

`AddEvent`는 범용 이벤트 추가 API다. 특정 Snapshot 영역을 갱신하지 않고 Event Log만 추가하고 싶을 때 사용한다.

## Snapshot 조회와 초기화

`GetSnapshotCopy`는 World 기준 Snapshot 복사본을 반환한다. 출력 인자는 먼저 빈 `FDebugOverlaySnapshot`으로 초기화된다. Store가 있으면 Snapshot을 복사하고, 표시 개수 기준으로 `RecentEvents`를 다시 구성한 뒤 `true`를 반환한다.

`GetRecentEventsCopy`는 Snapshot 전체가 아니라 최근 이벤트 로그만 복사해 반환한다.

`Reset`은 특정 World의 Store를 제거한다. `ResetAll`은 모든 World Store를 제거한다.

## HUD 표시 흐름

`ACDebugOverlayHUD::DrawHUD`는 Unreal `AHUD`의 draw 함수 override다. HUD가 그려지는 시점마다 호출된다.

```cpp
if (!FDebugOverlaySnapshotStore::IsEnabled()) return;
```

표시 옵션이 꺼져 있으면 바로 종료한다.

```cpp
const APawn* pawn = GetOwningPawn();
```

HUD를 소유한 Pawn을 얻는다.

```cpp
FDebugOverlaySnapshot snapshot;
const bool bHasSnapshot = FDebugOverlaySnapshotStore::GetSnapshotCopy(GetWorld(), snapshot);
```

현재 World의 Snapshot 복사본을 얻는다. HUD는 Store를 직접 수정하지 않고 복사본만 읽는다.

출력 라인은 `TArray<FString>`에 모은 뒤 마지막에 `DrawText`로 순서대로 그린다.

```text
[Debug Overlay P0]
ExecutionState
ActiveAction
ActiveReaction
GuardOverlay
RuntimeLODTier

[Recent Execution]
[Recent Combat]
[Recent AI]
[Event Log]
```

## Pawn Component 표시

HUD는 현재 Pawn에서 gameplay component를 찾아 상태를 문자열로 만든다.

`FindComponent<TComponent>`는 `Pawn->FindComponentByClass<TComponent>()`를 감싼 helper다. Pawn이 유효하지 않으면 `nullptr`을 반환한다.

`FormatExecutionState`는 `UCStateComponent`의 현재 execution state를 enum 문자열로 표시한다.

`FormatActiveAction`은 `UCActionComponent`가 active 상태인지 확인하고, active라면 action type과 index를 표시한다.

`FormatActiveReaction`은 `UCReactionComponent`의 active reaction type을 표시한다.

`FormatGuardOverlay`는 `UCDefenseComponent`의 guard 관련 boolean 상태를 한 줄로 요약한다.

`FormatRuntimeLODTier`는 현재 `N/A`를 반환한다. 이후 runtime LOD 정보를 붙이기 위한 확장 지점이다.

## Capture State 표시

Snapshot 값은 항상 존재한다고 가정하지 않는다. `EDebugOverlayCaptureState`로 값의 상태를 구분한다.

```text
Captured
Unavailable
Stale
NotCaptured
```

`ValueOrNotCaptured`는 capture state가 `Captured`이고 값이 비어 있지 않을 때만 실제 값을 표시한다. 그렇지 않으면 capture state 문자열을 표시한다.

`HasFinalTakenDamageEvidence`는 `FinalTakenDamage`를 표시할 근거가 있는지 확인한다. `Summary`에 `Final=`이 포함되어 있을 때만 실제 damage 값을 표시한다. 이는 아직 기록되지 않은 0과 실제 기록된 0을 혼동하지 않기 위한 방어 처리다.

## Canvas 출력

HUD 라인은 `DrawText`로 그린다.

```cpp
DrawText(line, FLinearColor::White, DebugOverlayOriginX, y, nullptr, DebugOverlayFontScale, false);
```

인자 의미는 다음과 같다.

- `line`: 출력 문자열
- `FLinearColor::White`: 텍스트 색상
- `DebugOverlayOriginX`: X 위치
- `y`: Y 위치
- `nullptr`: 기본 폰트 사용
- `DebugOverlayFontScale`: 폰트 크기 배율
- `false`: position scale 비활성

한 줄을 그린 뒤 `DebugOverlayLineHeight`만큼 y를 증가시켜 다음 줄을 아래에 출력한다.

## 설계 요약

이 구조의 핵심은 수집과 표시를 분리하는 것이다.

- Store는 World별 Snapshot과 Event Log를 관리한다.
- HUD는 Store의 복사본을 읽어 Canvas에 표시한다.
- 카테고리별 최신 상태는 `LastExecution`, `LastCombat`, `LastAI`로 유지한다.
- 시간순 이벤트 흐름은 `EventRing`과 `RecentEvents`로 유지한다.
- Shipping 빌드에서는 전체 기능이 비활성화된다.
- 수집 여부와 표시 여부는 console variable로 분리된다.
