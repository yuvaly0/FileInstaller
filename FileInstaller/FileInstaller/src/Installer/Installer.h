#pragma once
#include <string>
#include <vector>
#include "../File/File.h"

class Installer {
public:
	Installer();

	void copy();

private:
	bool _hasCreatedDestinationFolder;
	std::vector<std::string> _filePathsToCopy;
	std::string _destinationFolder;
};
