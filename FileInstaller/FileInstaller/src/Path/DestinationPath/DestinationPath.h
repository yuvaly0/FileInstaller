#pragma once
#include <windows.h>
#include "../Path.h"

class DestinationPath : public Path
{
public:
	DestinationPath(LPCWSTR destinationPath);

	bool verify();
};

