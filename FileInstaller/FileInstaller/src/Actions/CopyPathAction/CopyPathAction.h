#pragma once
#include <memory>
#include "../CreateDirectoryAction/CreateDirectoryAction.h"
#include "../Action.h"

class CopyPathAction : public Action 
{
public:
	CopyPathAction(LPCWSTR sourcePath, LPCWSTR destinationPath);

	void act() override;
	void rollback() override;

	bool _isDirectory;
	LPCWSTR _sourcePath;
private:
	void copy_file();
	void copy_directory();
	void initialize();

	void rollback_copy_file();
	void rollback_copy_directory();

	std::unique_ptr<wchar_t[]> _destinationFilePath;
	LPCWSTR _destinationPath;
};

