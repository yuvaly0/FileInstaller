#include <iostream>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#include <stdexcept>
#include "DestinationPath.h"

DestinationPath::DestinationPath(LPCWSTR destinationPath) : Path(destinationPath) {};

DestinationPath::SuccessResults DestinationPath::tryCreate() {
	DWORD destinationFileAttributes = GetFileAttributes(_path);

	// TODO: refactor return logic to exceptions instead of false and log in the installer
	// TODO: we might get ERROR_CANCELLED and it's ok, check REMARKS
	const int isSuccess = SHCreateDirectoryExW(NULL, _path, NULL);
	if (isSuccess == ERROR_SUCCESS) {
		return DestinationPath::CREATED_DIRECTORY;
	}

	// TODO: refactor to switch case to handle default
	const DWORD createDirectoryError = GetLastError();
	if (createDirectoryError == ERROR_FILE_EXISTS || createDirectoryError == ERROR_ALREADY_EXISTS) {
		return DestinationPath::DIDNT_CREATE_DIRECTORY;
	}

	if (createDirectoryError == ERROR_PATH_NOT_FOUND) {
		// TODO: log, location is not reachable or malformed or exceeds range (_path)
		throw std::exception();
	}

	if (createDirectoryError == ERROR_FILENAME_EXCED_RANGE) {
		// TODO: log, folder name exceeds range
		throw std::exception();
	}

	if (createDirectoryError == ERROR_ACCESS_DENIED) {
		throw std::exception();
	}

	if (createDirectoryError == ERROR_BAD_PATHNAME) {
		
		bool isPathRelative = PathIsRelativeW(_path);

		if (!isPathRelative) {
			// TODO: log, invalid or malformed absolute path
			throw std::exception();
		}
		
		WCHAR absolutePath[MAX_PATH];
		const DWORD amountCharsCopied = GetFullPathNameW(_path, MAX_PATH, absolutePath, NULL);
 		
		if (amountCharsCopied == 0) {
			const DWORD getFullPathNameError = GetLastError();

			if (getFullPathNameError == ERROR_INVALID_PARAMETER) {
				// TODO: log, invalid or malformed relative (_path)
				throw std::exception();
			}
		}

		_path = std::move(absolutePath);
	}

	throw std::exception();
}