#include <iostream>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#include <stdexcept>
#include "DestinationPath.h"
#include "../../Exceptions/InstallerException.h"

DestinationPath::DestinationPath(LPCWSTR destinationPath) : Path(destinationPath) {};

DestinationPath::CopyResults DestinationPath::tryCreate() {
	const int isSuccess = SHCreateDirectoryExW(NULL, _path, NULL);

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

		case ERROR_BAD_PATHNAME: {
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

			// todo: add 'relative' to the log?
			_path = absolutePath;
			auto result = tryCreate();
			return result;
		}

		default: {
			throw InstallerException("could not create directory, unknown error");
		}
	}
}