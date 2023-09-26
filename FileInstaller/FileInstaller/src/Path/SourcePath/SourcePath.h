#pragma once
#include <memory>
#include "../Path.h"
#include "../DestinationPath/DestinationPath.h"

class SourcePath : public Path
{
public:
	SourcePath(LPCWSTR sourcePath);
	void copy_path(std::shared_ptr<DestinationPath> destinationPath);

private:
	void copy_file(std::unique_ptr<wchar_t[]> destinationFilePath);
	void copy_directory();
};

