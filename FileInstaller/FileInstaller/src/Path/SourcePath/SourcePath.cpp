#include <windows.h>
#include "strsafe.h"
#include "shlwapi.h"
#include "SourcePath.h"
#include "../../Exceptions/InstallerException.h"

SourcePath::SourcePath(LPCWSTR sourcePath) : Path(sourcePath) {};

void SourcePath::copy_file(std::shared_ptr<DestinationPath> destinationPath) {
	LPCWSTR sourceFileName = PathFindFileName(_path);

	// todo: read more if MAX_PATH is the ideal way
	wchar_t destinationFilePath[MAX_PATH] = L""; 
	HRESULT ht = StringCchCatW(destinationFilePath, MAX_PATH, (LPWSTR)destinationPath->_path);
	PathAddBackslashW(destinationFilePath);
	ht = StringCchCatW(destinationFilePath, MAX_PATH, sourceFileName);

	if (FAILED(ht)) {
		throw InstallerException("couldn't copy file, file path exceeded max size, check to allocate greater path size");
	}
	
	const int result = CopyFileExW(_path, destinationFilePath, NULL, NULL, NULL, COPY_FILE_FAIL_IF_EXISTS);
	
	if (result != 0) {
 		return;
	}

	const DWORD copyFileError = GetLastError();

	switch (copyFileError)
	{
		case ERROR_FILE_NOT_FOUND: {
			throw InstallerException("couldn't copy file, file not found");
		}
			
		case ERROR_ACCESS_DENIED: {
			throw InstallerException("couldn't copy file, not enough permissions");
		}
			
		case ERROR_ENCRYPTION_FAILED: {
			throw InstallerException("couldn't copy encrypted file");
		}

		case ERROR_FILE_EXISTS: {
			throw InstallerException("couldn't copy file, exists in destination directory");
		}
			
		default: {
			throw InstallerException("couldn't copy file, unkown error");
		}
	}		
}
