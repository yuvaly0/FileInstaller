#pragma once
#include <windows.h>

class Path
{
public:
	Path(LPCWSTR path);
	virtual bool pre_copy_verify() = 0;

protected:
	LPCWSTR _path;
};

