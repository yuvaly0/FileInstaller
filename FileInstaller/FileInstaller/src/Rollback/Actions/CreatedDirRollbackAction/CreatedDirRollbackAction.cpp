#include "CreatedDirRollbackAction.h"

CreatedDirRollbackAction::CreatedDirRollbackAction(std::shared_ptr<wchar_t[]> destinationPath) 
	: _destinationPath(destinationPath), RollbackAction() {};

void CreatedDirRollbackAction::rollback() {
	RemoveDirectoryW(_destinationPath.get());
}