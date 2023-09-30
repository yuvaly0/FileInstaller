#include <iostream>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#include <stdexcept>
#include <strsafe.h>
#include "./CreateDirectoryAction.h"
#include "../../Exceptions/InstallerException.h"

CreateDirectoryAction::CreateDirectoryAction(LPCWSTR destinationPath) {
	_path.reset(new wchar_t[MAX_PATH], std::default_delete<wchar_t[]>());
	_path[0] = L'\0';
	StringCchCatW(_path.get(), MAX_PATH, destinationPath);
}

void CreateDirectoryAction::initialize() {
	bool isPathRelative = PathIsRelativeW(_path.get());

	if (isPathRelative) {
		const DWORD amountCharsCopied = GetFullPathNameW(_path.get(), MAX_PATH, _path.get(), NULL);

		if (amountCharsCopied == 0) {
			const DWORD getFullPathNameError = GetLastError();

			if (getFullPathNameError == ERROR_INVALID_PARAMETER) {
				throw InstallerException("could not create directory, invalid or malformed relative path");
			}
		}
	}
}

CreateDirectoryAction::CopyResults CreateDirectoryAction::tryCreate() {
	initialize();

	const int isSuccess = SHCreateDirectoryExW(NULL, _path.get(), NULL);

	if (isSuccess == ERROR_SUCCESS) {
		return CreateDirectoryAction::CREATED_DIRECTORY;
	}

	// TODO: add parameter to the exception and level (error, info)
	const DWORD createDirectoryError = GetLastError();
	
	if (createDirectoryError == ERROR_FILE_EXISTS || 
		createDirectoryError == ERROR_ALREADY_EXISTS ||
		// the directory created but one or more of the intermediate folders do not exist
		createDirectoryError == ERROR_CANCELLED
		) {
		return CreateDirectoryAction::DIDNT_CREATE_DIRECTORY;
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