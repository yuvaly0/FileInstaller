#pragma once
#include "../Path.h"

class SourcePath : public Path
{
public:
	SourcePath(LPCWSTR sourcePath);
	bool verify();
};

