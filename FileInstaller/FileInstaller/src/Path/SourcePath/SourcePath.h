#pragma once
#include "../Path.h"
#include "../DestinationPath/DestinationPath.h"

class SourcePath : public Path
{
public:
	SourcePath(LPCWSTR sourcePath);
	bool copy_file(DestinationPath* destinationPath);
};

