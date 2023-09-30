#include "CopiedFileAction.h"
#include "../../../Utils/Utils.h"

CopiedFileAction::CopiedFileAction(LPCWSTR sourcePath, std::shared_ptr<wchar_t[]> destinationPath)
	: _sourcePath(sourcePath), _destinationPath(destinationPath), RollbackAction() {};

void CopiedFileAction::rollback() {
	// no need to check return value, we've done this before and it worked
	std::unique_ptr<wchar_t[]> destinationFilePath = Utils::getDestinationFilePath(_destinationPath.get(), _sourcePath);

	DeleteFileW(destinationFilePath.get());
}
