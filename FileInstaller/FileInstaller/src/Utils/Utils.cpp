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
}