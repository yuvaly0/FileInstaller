#include <iostream>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#include <stdexcept>
#include "DestinationPath.h"
#include "../../Exceptions/InstallerException.h"

DestinationPath::DestinationPath(LPCWSTR destinationPath) : Path(destinationPath) {};

DestinationPath::SuccessResults DestinationPath::tryCreate() {
	// TODO: refactor return logic to exceptions instead of false and log in the installer
	// TODO: we might get ERROR_CANCELLED and it's ok, check REMARKS
	const int isSuccess = SHCreateDirectoryExW(NULL, _path, NULL);
	if (isSuccess == ERROR_SUCCESS) {
		return DestinationPath::CREATED_DIRECTORY;
	}

	// TODO: refactor to switch case to handle default
	// TODO: add parameter to the exception and level (error, info)
	const DWORD createDirectoryError = GetLastError();
	if (createDirectoryError == ERROR_FILE_EXISTS || createDirectoryError == ERROR_ALREADY_EXISTS) {
		return DestinationPath::DIDNT_CREATE_DIRECTORY;
	}

	if (createDirectoryError == ERROR_PATH_NOT_FOUND) {
		throw InstallerException("could not create directory, not reachable or malformed");
	}

	if (createDirectoryError == ERROR_FILENAME_EXCED_RANGE) {
		throw InstallerException("could not create directory, folder name exceeds range");
	}

	if (createDirectoryError == ERROR_ACCESS_DENIED) {
		throw InstallerException("could not create directory, not enough premissions");
	}

	if (createDirectoryError == ERROR_BAD_PATHNAME) {
		
		bool isPathRelative = PathIsRelativeW(_path);

		if (!isPathRelative) {
			throw InstallerException("could not create directory, invalid or malformed absolute path");
		}
		
		WCHAR absolutePath[MAX_PATH];
		const DWORD amountCharsCopied = GetFullPathNameW(_path, MAX_PATH, absolutePath, NULL);
 		
		if (amountCharsCopied == 0) {
			const DWORD getFullPathNameError = GetLastError();

			if (getFullPathNameError == ERROR_INVALID_PARAMETER) {
				throw InstallerException("could not create directory, invalid or malformed relative path");
			}
		}

		_path = std::move(absolutePath);
	}

	throw InstallerException("could not create directory, unknown error");
}