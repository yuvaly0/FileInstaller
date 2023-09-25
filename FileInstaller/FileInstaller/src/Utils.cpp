#include "shlwapi.h"
#include "strsafe.h"
#include "Utils.h"

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
}