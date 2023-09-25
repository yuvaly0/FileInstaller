#include "CopiedFileAction.h"
#include "../../../Utils.h"

CopiedFileAction::CopiedFileAction(LPCWSTR sourcePath, LPCWSTR destinationPath) 
	: _sourcePath(sourcePath), _destionationPath(destinationPath), RollbackAction() {};

void CopiedFileAction::rollback() {
	// no need to check return value, we've done this before and it worked
	std::unique_ptr<wchar_t[]> destinationFilePath = Utils::getDestinationFilePath(_destionationPath, _sourcePath);

	DeleteFileW(destinationFilePath.get());
}
