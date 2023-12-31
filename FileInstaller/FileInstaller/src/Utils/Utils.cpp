#include <vector>
#include "shlwapi.h"
#include "strsafe.h"
#include "Utils.h"
#include "../Exceptions/InstallerException.h"

namespace Utils {
	std::unique_ptr<WCHAR[]> getDestinationFilePath(LPCWSTR destinationPath, LPCWSTR sourcePath) {
		LPCWSTR sourceFileName = PathFindFileName(sourcePath);

		auto destinationFilePath = std::make_unique<WCHAR[]>(MAX_PATH);
		HRESULT firstConcatResult = StringCchCatW(destinationFilePath.get(), MAX_PATH, (LPWSTR)destinationPath);
		LPWSTR addBackslashPtrResult = PathAddBackslashW(destinationFilePath.get());
		HRESULT secondConcatResult = StringCchCatW(destinationFilePath.get(), MAX_PATH, sourceFileName);

		if (FAILED(firstConcatResult) || FAILED(secondConcatResult) || !addBackslashPtrResult) {
			return nullptr;
		}

		return destinationFilePath;
	}

	std::shared_ptr<WCHAR[]> getAbsolutePath(LPCWSTR relativePath) {
		std::shared_ptr<WCHAR[]> absolutePath(new WCHAR[MAX_PATH], std::default_delete<WCHAR[]>());
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
				const int length = static_cast<int>(cursor - path + 1);
				lstrcpynW(currentPath, path, length + 1);

				if (!isPathExists(currentPath)) {
					std::shared_ptr<WCHAR[]> toBeCopiedPath(new WCHAR[MAX_PATH], std::default_delete<WCHAR[]>());
					lstrcpynW(toBeCopiedPath.get(), path, length + 1);
					directoriesToBeCreated.push_back(toBeCopiedPath);
				}
			}

			cursor++;
		}

		return directoriesToBeCreated;
	}
}