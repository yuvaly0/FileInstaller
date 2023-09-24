#include "CreatedDirRollbackAction.h"

CreatedDirRollbackAction::CreatedDirRollbackAction(LPCWSTR folderPath) : _folderPath(folderPath), RollbackAction() {};

void CreatedDirRollbackAction::rollback() {
	RemoveDirectoryW(_folderPath);
}