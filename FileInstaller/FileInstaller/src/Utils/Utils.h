#pragma once
#include <memory>
#include <vector>
#include <strsafe.h>

namespace Utils {
	std::unique_ptr<WCHAR[]> getDestinationFilePath(LPCWSTR destinationPath, LPCWSTR sourcePath);
	std::shared_ptr<WCHAR[]> getAbsolutePath(LPCWSTR relativePath);
	std::vector<std::shared_ptr<WCHAR[]>> getDirectoriesToBeCreated(LPCWSTR path);
	bool isPathExists(LPCWSTR path);

}