#pragma once
#include <windows.h>

class Path
{
public:
	Path(LPCWSTR path);

protected:
	LPCWSTR _path;
};

