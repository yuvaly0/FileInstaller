#pragma once
#include <memory>
#include "../Path.h"
#include "../DestinationPath/DestinationPath.h"

class SourcePath : public Path
{
public:
	SourcePath(LPCWSTR sourcePath);
	bool copy_file(std::shared_ptr<DestinationPath> destinationPath);
};

