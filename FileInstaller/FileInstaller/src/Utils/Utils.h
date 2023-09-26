#pragma once
#include "../Path/DestinationPath/DestinationPath.h"
#include "../Path/SourcePath/SourcePath.h"

namespace Utils {
	std::unique_ptr<wchar_t[]> getDestinationFilePath(LPCWSTR destinationPath, LPCWSTR sourcePath);
}