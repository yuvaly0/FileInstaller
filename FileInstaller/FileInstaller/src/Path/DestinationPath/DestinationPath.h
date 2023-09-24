#pragma once
#include <windows.h>
#include "../Path.h"

class DestinationPath : public Path
{
public:
	DestinationPath(LPCWSTR destinationPath);

	enum SuccessResults {
		CREATED_DIRECTORY = 1,
		DIDNT_CREATE_DIRECTORY,
		TEMP
	};

	SuccessResults tryCreate();
};

