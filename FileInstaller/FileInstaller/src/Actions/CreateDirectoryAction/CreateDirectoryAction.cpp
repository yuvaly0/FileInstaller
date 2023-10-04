#include <iostream>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#include <stdexcept>
#include <strsafe.h>
#include "./CreateDirectoryAction.h"
#include "../../Exceptions/InstallerException.h"
#include "../../Utils/Utils.h"

CreateDirectoryAction::CreateDirectoryAction(LPCWSTR destinationPath) {
	_path.reset(new WCHAR[MAX_PATH], std::default_delete<WCHAR[]>());
	_path[0] = L'\0';
	StringCchCopyExW(_path.get(), MAX_PATH, destinationPath, NULL, NULL, STRSAFE_NULL_ON_FAILURE);

	_hasCreatedDirectory = false;
}

void CreateDirectoryAction::initialize() {
	if (!_path) {
		throw InstallerException("could not copy initial destination");
	}

	bool isPathRelative = PathIsRelativeW(_path.get());

	if (isPathRelative) {
		_path = Utils::getAbsolutePath(_path.get());
	}

	_directoriesToBeRemoved = Utils::getDirectoriesToBeCreated(_path.get());
}

void CreateDirectoryAction::act() {
	initialize();

	const int isSuccess = SHCreateDirectoryExW(NULL, _path.get(), NULL);

	if (isSuccess == ERROR_SUCCESS) {
		_hasCreatedDirectory = true;
		return;
	}

	// TODO: add parameter to the exception and level (error, info)
	const DWORD createDirectoryError = GetLastError();
	
	if (createDirectoryError == ERROR_FILE_EXISTS || 
		createDirectoryError == ERROR_ALREADY_EXISTS ||
		// SHCreateDirectoryEx also verifies that the files are visible. If they are not visible, ... If hwnd is set to NULL ... the function returns ERROR_CANCELLED.
		createDirectoryError == ERROR_CANCELLED
		) {
		_hasCreatedDirectory = false;
		return;
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

void CreateDirectoryAction::rollback() {
	if (_hasCreatedDirectory) {
		for (auto it = _directoriesToBeRemoved.rbegin(); it != _directoriesToBeRemoved.rend(); ++it) {
			RemoveDirectoryW((*it).get());
		}
		
	}
}