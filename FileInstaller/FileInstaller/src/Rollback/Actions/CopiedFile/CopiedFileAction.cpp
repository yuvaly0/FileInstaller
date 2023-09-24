#include "shlwapi.h"
#include "strsafe.h"
#include "CopiedFileAction.h"

CopiedFileAction::CopiedFileAction(LPCWSTR sourcePath, LPCWSTR destinationPath) 
	: _sourcePath(sourcePath), _destionationPath(destinationPath), RollbackAction() {};

void CopiedFileAction::rollback() {
	LPCWSTR sourceFileName = PathFindFileName(_sourcePath);

	// todo: read more if MAX_PATH is the ideal way
	// no need to check return value, we've done this before and it works
	wchar_t destinationFilePath[MAX_PATH] = L"";
	StringCchCatW(destinationFilePath, MAX_PATH, (LPWSTR)_destionationPath);
	StringCchCatW(destinationFilePath, MAX_PATH, L"\\");
	StringCchCatW(destinationFilePath, MAX_PATH, sourceFileName);

	DeleteFileW(destinationFilePath);
}
