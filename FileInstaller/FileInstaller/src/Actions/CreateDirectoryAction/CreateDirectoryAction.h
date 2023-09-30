#pragma once
#include <windows.h>
#include <memory>
#include "../Action.h"

class CreateDirectoryAction : public Action
{
public:
	CreateDirectoryAction(LPCWSTR destinationPath);

	enum CopyResults {
		CREATED_DIRECTORY = 1,
		DIDNT_CREATE_DIRECTORY
	};

	CopyResults tryCreate();
	void rollback() override;

	std::shared_ptr<wchar_t[]> _path;

private:
	void initialize();

	bool _hasCreatedDirectory;
};

