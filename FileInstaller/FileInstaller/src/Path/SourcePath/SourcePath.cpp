#include "SourcePath.h"

SourcePath::SourcePath(LPCWSTR sourcePath) : Path(sourcePath) {};

bool SourcePath::verify() {
	return true;
};
