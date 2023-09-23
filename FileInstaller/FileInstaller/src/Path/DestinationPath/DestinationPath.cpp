#include <iostream>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#include "DestinationPath.h"

DestinationPath::DestinationPath(LPCWSTR destinationPath) : Path(destinationPath) {};

bool DestinationPath::pre_copy_verify() {
	DWORD destinationFileAttributes = GetFileAttributes(_path);

	// TODO: we might get ERROR_CANCELLED and it's ok, check REMARKS
	// TODO: first go recursively over the path to know where we should rollback if needed
	const int isSuccess = SHCreateDirectoryExW(NULL, _path, NULL);
	if (isSuccess == ERROR_SUCCESS) {
		return true;
	}

	const DWORD createDirectoryError = GetLastError();
	if (createDirectoryError == ERROR_FILE_EXISTS || createDirectoryError == ERROR_ALREADY_EXISTS) {
		return true;
	}

	if (createDirectoryError == ERROR_PATH_NOT_FOUND) {
		// TODO: log, location is not reachable or malformed or exceeds range (_path)
		return false;
	}

	if (createDirectoryError == ERROR_FILENAME_EXCED_RANGE) {
		// TODO: log, folder name exceeds range
		return false;
	}

	if (createDirectoryError == ERROR_BAD_PATHNAME) {
		
		bool isPathRelative = PathIsRelativeW(_path);

		if (!isPathRelative) {
			// TODO: log, invalid or malformed absolute path
			return false;
		}
		
		WCHAR absolutePath[MAX_PATH];
		const DWORD amountCharsCopied = GetFullPathNameW(_path, MAX_PATH, absolutePath, NULL);
 		
		if (amountCharsCopied == 0) {
			const DWORD getFullPathNameError = GetLastError();

			if (getFullPathNameError == ERROR_INVALID_PARAMETER) {
				// TODO: log, invalid or malformed relative (_path)
				return false;
			}
		}

		_path = std::move(absolutePath);
	}

	return true;
}