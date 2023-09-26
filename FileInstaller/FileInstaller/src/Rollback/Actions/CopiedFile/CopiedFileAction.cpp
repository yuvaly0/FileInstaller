#include "CopiedFileAction.h"
#include "../../../Utils/Utils.h"

CopiedFileAction::CopiedFileAction(LPCWSTR sourcePath, LPCWSTR destinationPath) 
	: _sourcePath(sourcePath), _destinationPath(destinationPath), RollbackAction() {};

void CopiedFileAction::rollback() {
	// no need to check return value, we've done this before and it worked
	std::unique_ptr<wchar_t[]> destinationFilePath = Utils::getDestinationFilePath(_destinationPath, _sourcePath);

	DeleteFileW(destinationFilePath.get());
}
