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
	void copy_file(std::unique_ptr<wchar_t[]> destinationPath);
	void copy_directory(LPCWSTR destinationPathParent, std::unique_ptr<wchar_t[]> destinationFilePath);
};

