#pragma once
#include <windows.h>
#include <memory>

class DestinationPath
{
public:
	DestinationPath(LPCWSTR destinationPath);

	enum CopyResults {
		CREATED_DIRECTORY = 1,
		DIDNT_CREATE_DIRECTORY
	};

	CopyResults tryCreate();
	
	std::shared_ptr<wchar_t[]> _path;
};

