# CSV Export Format

이 문서는 `AssetReferenceInspector`의 CSV Export 문자열 생성 규칙을 정리한다.

CSV Export의 구현 위치는 `Private/Export/AssetReferenceCsvExporter`다.

## 기본 구조

CSV는 row와 field로 구성한다.

```text
field 구분자 = ,
row 구분자 = \n
field 범위 구분자 = "
```

현재 구현은 먼저 node 하나를 field 배열로 만든 뒤, field를 `,`로 합쳐 CSV row 하나를 만든다.

```cpp
FString::Join(Fields, TEXT(","));
```

이후 row 배열을 `\n`으로 합쳐 최종 CSV 문자열을 만든다.

```cpp
FString::Join(Rows, TEXT("\n"));
```

CSV를 읽는 도구는 `,`를 같은 row 안의 column 구분자로 보고, `\n`을 다음 row로 넘어가는 구분자로 본다.

## Field Escape

CSV field 값 안에 `,`, `"`, `\n`, `\r`이 들어가면 CSV parser가 값을 잘못 나눌 수 있다. 따라서 문자열 field는 `EscapeCsvField`를 거쳐 저장한다.

처리 규칙:

- field 안의 `"`는 `""`로 바꾼다.
- field 안에 `,`, `"`, `\n`, `\r` 중 하나라도 있으면 field 전체를 `"`로 감싼다.

예:

```text
My,Asset
```

CSV field:

```csv
"My,Asset"
```

예:

```text
My "Asset"
```

CSV field:

```csv
"My ""Asset"""
```

`\r`은 carriage return이다. Windows 줄바꿈은 보통 `\r\n`이므로, `\n`과 함께 검사한다.

## 현재 컬럼

CSV header는 다음 순서를 사용한다.

```text
AssetName,PackageName,Class,Path,Depth,SizeBytes,Mode,ParentPackage,IsCircular,IsUnusedCandidate
```

매핑:

```text
AssetName = Node.DisplayName
PackageName = Node.PackageName
Class = Node.ClassName
Path = PackageName의 folder path
Depth = Node.Depth
SizeBytes = Node.SizeBytes
Mode = Dependencies / Referencers / UnusedCandidates
ParentPackage = Node.ParentPackageName
IsCircular = true / false
IsUnusedCandidate = true / false
```

숫자와 boolean field는 현재 고정 형식 문자열이므로 별도 escape를 적용하지 않는다.

## Export 대상

`PackageName == NAME_None`인 node는 CSV row로 저장하지 않는다.

제외 예:

- `Unused Candidates` grouping node
- `No dependencies found` placeholder
- `No referencers found` placeholder
- `No unused candidates found` placeholder

단, 제외 node의 자식은 계속 순회한다. 따라서 `Unused Candidates` root node는 제외하지만, 그 아래 실제 후보 Asset node는 저장한다.

## 저장 위치

CSV 파일은 프로젝트 `Saved` 아래에 저장한다.

```text
Saved/AssetReferenceInspector/AssetReferenceReport_YYYYMMDD_HHMMSS.csv
```

저장 문자열은 UTF-8 without BOM으로 기록한다.

## 저장 경로 구성

저장 경로 구성은 다음 순서로 처리한다.

```text
FPaths::ProjectSavedDir()
-> Saved/ 경로 확보
-> AssetReferenceInspector 하위 폴더 경로 조합
-> IFileManager::MakeDirectory(..., true)
-> timestamp 기반 파일명 생성
-> FFileHelper::SaveStringToFile(...)
```

구현:

```cpp
const FString ExportDirectory = FPaths::ProjectSavedDir() / TEXT("AssetReferenceInspector");
```

`FPaths::ProjectSavedDir()`는 현재 Unreal 프로젝트의 `Saved/` 경로를 반환한다. 여기에 `AssetReferenceInspector` 하위 폴더를 붙여 CSV Export 전용 폴더를 만든다.

예:

```text
Portfolio_PlugIn/Saved/AssetReferenceInspector/
```

`/` 연산자는 Unreal의 `FString` 경로 조합에서 사용한다. 직접 `"\\"` 또는 `"/"`를 문자열로 붙이는 것보다 경로 조합 의도가 명확하다.

## 저장 폴더 생성

```cpp
if (!IFileManager::Get().MakeDirectory(*ExportDirectory, true))
{
	OutErrorMessage = FString::Printf(TEXT("Failed to create export directory: %s"), *ExportDirectory);
	return false;
}
```

`IFileManager::Get().MakeDirectory`는 폴더가 없으면 생성한다. 두 번째 인자 `true`는 필요한 중간 폴더까지 생성하라는 의미다.

이미 폴더가 있으면 일반적으로 성공으로 처리한다. 실패하는 경우는 권한 문제, 잘못된 경로, 같은 이름의 파일 존재, 디스크 오류 같은 파일 시스템 문제다.

실패 시에는 `OutErrorMessage`에 실패 경로를 포함한 메시지를 저장하고 `false`를 반환한다. Widget은 이 결과를 받아 실패 notification을 표시한다.

## 파일명 생성

```cpp
const FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
OutFilename = ExportDirectory / FString::Printf(TEXT("AssetReferenceReport_%s.csv"), *Timestamp);
```

`FDateTime::Now()`로 현재 시간을 가져오고, `YYYYMMDD_HHMMSS` 형식의 문자열로 변환한다.

예:

```text
20260719_213045
```

최종 파일명은 다음 형식이다.

```text
AssetReferenceReport_20260719_213045.csv
```

같은 폴더에 여러 번 export해도 파일명이 초 단위 timestamp를 포함하므로 기존 파일을 덮어쓸 가능성을 줄인다.

## 파일 저장

```cpp
const FString CsvContent = FString::Join(Rows, TEXT("\n"));
if (!FFileHelper::SaveStringToFile(CsvContent, *OutFilename, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
{
	OutErrorMessage = FString::Printf(TEXT("Failed to save CSV file: %s"), *OutFilename);
	return false;
}
```

`Rows` 배열은 CSV 한 줄씩을 보관한다. `FString::Join(Rows, TEXT("\n"))`으로 row들을 줄바꿈으로 연결해 최종 CSV 문자열을 만든다.

`FFileHelper::SaveStringToFile`은 CSV 문자열을 실제 파일로 저장한다. 현재 인코딩은 `ForceUTF8WithoutBOM`이다.

저장 실패 시에는 `OutErrorMessage`에 실패 파일 경로를 포함한 메시지를 저장하고 `false`를 반환한다. 성공 시에는 `OutFilename`에 생성된 파일 경로가 남아 성공 notification에 사용된다.
