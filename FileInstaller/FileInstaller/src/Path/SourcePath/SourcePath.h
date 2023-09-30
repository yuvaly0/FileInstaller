#pragma once
#include <memory>
#include "../Path.h"
#include "../DestinationPath/DestinationPath.h"

class SourcePath
{
public:
	SourcePath(LPCWSTR sourcePath, std::shared_ptr<wchar_t[]> destinationPath);

	void copy_path(std::shared_ptr<DestinationPath> destinationPath);

	bool _isDirectory;
	LPCWSTR _sourcePath;
private:
	void copy_file();
	void copy_directory();

	std::unique_ptr<wchar_t[]> _destinationFilePath;
	std::shared_ptr<wchar_t[]> _destinationPath;
};

