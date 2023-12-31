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

private:
	void copy_file();
	void copy_directory();
	void initialize();

	void rollback_copy_file();
	void rollback_copy_directory();

	bool _isDirectory;
	std::shared_ptr<WCHAR[]> _sourcePath;
	std::unique_ptr<WCHAR[]> _destinationFilePath;
	std::shared_ptr<WCHAR[]> _destinationPath;
};

