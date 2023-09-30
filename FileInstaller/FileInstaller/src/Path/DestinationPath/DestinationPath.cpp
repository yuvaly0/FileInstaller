#include <iostream>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#include <stdexcept>
#include "DestinationPath.h"
#include "../../Exceptions/InstallerException.h"
#include <strsafe.h>

DestinationPath::DestinationPath(LPCWSTR destinationPath) {
	_path.reset(new wchar_t[MAX_PATH], std::default_delete<wchar_t[]>());
	_path[0] = L'\0';
	
	bool isPathRelative = PathIsRelativeW(destinationPath);

	if (isPathRelative) {
		const DWORD amountCharsCopied = GetFullPathNameW(destinationPath, MAX_PATH, _path.get(), NULL);

		if (amountCharsCopied == 0) {
			const DWORD getFullPathNameError = GetLastError();

			if (getFullPathNameError == ERROR_INVALID_PARAMETER) {
				throw InstallerException("could not create directory, invalid or malformed relative path");
			}
		}
	}
	else {
		HRESULT concatResult = StringCchCatW(_path.get(), MAX_PATH, (LPWSTR)destinationPath);

		if (FAILED(concatResult)) {
			throw InstallerException("Could not initialize destinationPath");
		}
	}
};

DestinationPath::CopyResults DestinationPath::tryCreate() {
	const int isSuccess = SHCreateDirectoryExW(NULL, _path.get(), NULL);

	if (isSuccess == ERROR_SUCCESS) {
		return DestinationPath::CREATED_DIRECTORY;
	}

	// TODO: add parameter to the exception and level (error, info)
	const DWORD createDirectoryError = GetLastError();
	
	if (createDirectoryError == ERROR_FILE_EXISTS || 
		createDirectoryError == ERROR_ALREADY_EXISTS ||
		// the directory created but one or more of the intermediate folders do not exist
		createDirectoryError == ERROR_CANCELLED
		) {
		return DestinationPath::DIDNT_CREATE_DIRECTORY;
	}

	switch (createDirectoryError) {
		case ERROR_PATH_NOT_FOUND: {
			throw InstallerException("could not create directory, not reachable or malformed");
		}

		case ERROR_FILENAME_EXCED_RANGE: {
			throw InstallerException("could not create directory, folder name exceeds range");
		}

		case ERROR_ACCESS_DENIED: {
			throw InstallerException("could not create directory, not enough premissions");
		}

		default: {
			throw InstallerException("could not create directory, unknown error");
		}
	}
}