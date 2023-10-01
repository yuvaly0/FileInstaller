#include <vector>
#include "shlwapi.h"
#include "strsafe.h"
#include "Utils.h"
#include "../Exceptions/InstallerException.h"

namespace Utils {
	std::unique_ptr<wchar_t[]> getDestinationFilePath(LPCWSTR destinationPath, LPCWSTR sourcePath) {
		LPCWSTR sourceFileName = PathFindFileName(sourcePath);

		auto destinationFilePath = std::make_unique<wchar_t[]>(MAX_PATH);
		HRESULT firstConcatResult = StringCchCatW(destinationFilePath.get(), MAX_PATH, (LPWSTR)destinationPath);
		LPWSTR addBackslashPtrResult = PathAddBackslashW(destinationFilePath.get());
		HRESULT secondConcatResult = StringCchCatW(destinationFilePath.get(), MAX_PATH, sourceFileName);

		if (FAILED(firstConcatResult) || FAILED(secondConcatResult) || !addBackslashPtrResult) {
			return nullptr;
		}

		return destinationFilePath;
	}

	std::shared_ptr<wchar_t[]> getAbsolutePath(LPCWSTR relativePath) {
		std::shared_ptr<wchar_t[]> absolutePath(new wchar_t[MAX_PATH], std::default_delete<wchar_t[]>());
		const DWORD amountCharsCopied = GetFullPathNameW(relativePath, MAX_PATH, absolutePath.get(), NULL);

		if (amountCharsCopied == 0) {
			const DWORD getFullPathNameError = GetLastError();

			if (getFullPathNameError == ERROR_INVALID_PARAMETER) {
				throw InstallerException("could not create directory, invalid or malformed relative path");
			}

			throw InstallerException("unknown error while validating relative path");
		}

		return absolutePath;
	}

	// todo: check who implements and he should use this
	bool isPathExists(LPCWSTR path) {
		const DWORD pathAttributes = GetFileAttributesW(path);
		auto isCreated = pathAttributes != INVALID_FILE_ATTRIBUTES;

		return isCreated;
	}

	std::vector<std::shared_ptr<WCHAR[]>> getDirectoriesToBeCreated(LPCWSTR path) {
		std::vector<std::shared_ptr<WCHAR[]>> directoriesToBeCreated = {};
		LPCWSTR cursor = path;
		WCHAR currentPath[MAX_PATH] = {};

		// find the directories in the heirarchy that doesnt exist
		while (*cursor) {
			if (*cursor == L'\\' || *(cursor + 1) == L'\0') {
				auto length = cursor - path + 1;
				lstrcpyn(currentPath, path, length + 1);

				if (!isPathExists(currentPath)) {
					std::shared_ptr<WCHAR[]> toBeCopiedPath(new WCHAR[MAX_PATH], std::default_delete<WCHAR[]>());
					lstrcpyn(toBeCopiedPath.get(), path, length + 1);
					directoriesToBeCreated.push_back(toBeCopiedPath);
				}
			}

			cursor++;
		}

		return directoriesToBeCreated;
	}
}