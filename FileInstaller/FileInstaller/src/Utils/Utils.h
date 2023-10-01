#pragma once
#include <memory>
#include <vector>
#include <strsafe.h>

namespace Utils {
	std::unique_ptr<wchar_t[]> getDestinationFilePath(LPCWSTR destinationPath, LPCWSTR sourcePath);
	std::shared_ptr<wchar_t[]> getAbsolutePath(LPCWSTR relativePath);
	std::vector<std::shared_ptr<WCHAR[]>> getDirectoriesToBeCreated(LPCWSTR path);
	bool isPathExists(LPCWSTR path);

}