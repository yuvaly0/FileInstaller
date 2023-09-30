#pragma once
#include <windows.h>
#include <memory>

class CreateDirectoryAction
{
public:
	CreateDirectoryAction(LPCWSTR destinationPath);

	enum CopyResults {
		CREATED_DIRECTORY = 1,
		DIDNT_CREATE_DIRECTORY
	};

	CopyResults tryCreate();
	
	std::shared_ptr<wchar_t[]> _path;

private:
	void initialize();
};

