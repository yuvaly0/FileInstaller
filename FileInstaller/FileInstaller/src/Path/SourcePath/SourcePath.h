#pragma once
#include <memory>
#include "../Path.h"
#include "../DestinationPath/DestinationPath.h"

class SourcePath : public Path
{
public:
	SourcePath(LPCWSTR sourcePath, LPCWSTR destinationPath);
	~SourcePath();

	void copy_path(std::shared_ptr<DestinationPath> destinationPath);

	bool _isDirectory;
private:
	void copy_file();
	void copy_directory();

	std::unique_ptr<wchar_t[]> _destinationFilePath;
	LPCWSTR _destinationPath;
	LPCWSTR _sourcePath;
};

