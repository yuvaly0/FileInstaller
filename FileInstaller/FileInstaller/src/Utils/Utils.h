#pragma once
#include <memory>
#include <strsafe.h>

namespace Utils {
	std::unique_ptr<wchar_t[]> getDestinationFilePath(LPCWSTR destinationPath, LPCWSTR sourcePath);
	std::shared_ptr<wchar_t[]> getAbsolutePath(LPCWSTR relativePath);
}