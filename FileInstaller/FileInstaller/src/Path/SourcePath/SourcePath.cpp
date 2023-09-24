#include <windows.h>
#include "strsafe.h"
#include "shlwapi.h"
#include "SourcePath.h"

SourcePath::SourcePath(LPCWSTR sourcePath) : Path(sourcePath) {};

void SourcePath::copy_file(std::shared_ptr<DestinationPath> destinationPath) {
	LPCWSTR sourceFileName = PathFindFileName(_path);

	// todo: read more if MAX_PATH is the ideal way
	wchar_t destinationFilePath[MAX_PATH] = L""; 
	HRESULT ht = StringCchCatW(destinationFilePath, MAX_PATH, (LPWSTR)destinationPath->_path);
	ht = StringCchCatW(destinationFilePath, MAX_PATH, L"\\");
	ht = StringCchCatW(destinationFilePath, MAX_PATH, sourceFileName);
	
	if (FAILED(ht)) {
		// TOOD: log, couldn't create full destination path
		throw std::exception();
	}
	
	const int result = CopyFileExW(_path, destinationFilePath, NULL, NULL, NULL, COPY_FILE_FAIL_IF_EXISTS);
	
	if (result != 0) {
 		return;
	}

	const DWORD copyFileError = GetLastError();

	switch (copyFileError) 
	{
		case ERROR_FILE_NOT_FOUND:
			// todo: log, source file not found
			throw std::exception();

		case ERROR_ACCESS_DENIED:
			// TODO: log, could not copy file, not enough permissions (_path, destinationPath)
			throw std::exception();

		case ERROR_ENCRYPTION_FAILED:
			// todo: log, could not copy encrypted files
			throw std::exception();
		case ERROR_FILE_EXISTS:
			// todo: log, same file exists in destination directory
			throw std::exception();
		default:
			// todo: handle unknown error
			throw std::exception();
	}
}
