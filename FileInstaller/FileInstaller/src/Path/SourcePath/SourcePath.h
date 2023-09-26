#pragma once
#include <memory>
#include "../Path.h"
#include "../DestinationPath/DestinationPath.h"

class SourcePath : public Path
{
public:
	SourcePath(LPCWSTR sourcePath);
	void copy_path(std::shared_ptr<DestinationPath> destinationPath);

	bool _isDirectory;
private:
	void copy_file(LPCWSTR destinationPath);
	void copy_directory(LPCWSTR destinationFilePath);
};

