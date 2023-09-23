#include <windows.h>
#include "strsafe.h"
#include "SourcePath.h"

/*
int get_file_name(LPCWSTR filePath) {
	const int filePathLength = lstrlenW(filePath);

	for (int i = filePathLength - 1; i >= 0; i--) {
		if (lstrcmpiW("\\", filePathLength - 1 - 1))
	}
}
*/

SourcePath::SourcePath(LPCWSTR sourcePath) : Path(sourcePath) {};

bool SourcePath::copy_file(DestinationPath* destinationPath) {
	// LPCWSTR sourceFileName = StrChrW(L"..............", (WCHAR)".");
	wchar_t destinationFilePath[MAX_PATH] = L""; 
	HRESULT ht = StringCchCatW(destinationFilePath, MAX_PATH, (LPWSTR)destinationPath->getPath());
	ht = StringCchCatW(destinationFilePath, MAX_PATH, L"\\1.txt");
	
	if (FAILED(ht)) {
		// TOOD: log, couldn't create full destination path
		return false;
	}
	
	const BOOL result = CopyFileExW(_path, destinationFilePath, NULL, NULL, NULL, COPY_FILE_FAIL_IF_EXISTS);
	
	if (result != 0) {
		return true;
	}

	const DWORD copyFileError = GetLastError();
	
	if (copyFileError == ERROR_ACCESS_DENIED) {
		// TODO: log, could not copy file, not enough permissions (_path, destinationPath)
		return false;
	}
	
	return true;
}
