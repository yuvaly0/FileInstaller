#pragma once
#include <windows.h>

class Path
{
public:
	Path(LPCWSTR path);
	virtual bool verify() = 0;

protected:
	LPCWSTR _path;
};

