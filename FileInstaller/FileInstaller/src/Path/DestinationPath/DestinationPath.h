#pragma once
#include <windows.h>
#include "../Path.h"

class DestinationPath : public Path
{
public:
	DestinationPath(LPCWSTR destinationPath);

	enum CopyResults {
		CREATED_DIRECTORY = 1,
		DIDNT_CREATE_DIRECTORY
	};

	CopyResults tryCreate();
};

