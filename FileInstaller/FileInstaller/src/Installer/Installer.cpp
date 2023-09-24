#include <windows.h>
#include <memory>
#include "Installer.h"
#include "../Rollback/Actions/CopiedFile/CopiedFileAction.h"
#include "../Rollback/Actions/CreatedDirRollbackAction/CreatedDirRollbackAction.h"

// TODO: make singleton
Installer::Installer(DestinationPath* destinationPath, std::vector<SourcePath*> sourcePaths)
	: _destinationPath(destinationPath), _sourcePaths(sourcePaths) {
	
	_rollbackHandler = std::make_unique<RollbackHandler>();
}

void Installer::copy() {
	try {
		DestinationPath::SuccessResults result = _destinationPath->tryCreate();

		if (result == DestinationPath::CREATED_DIRECTORY) {
			_rollbackHandler->add_action(new CreatedDirRollbackAction(_destinationPath->_path));
		}

		for (auto i = 0; i < _sourcePaths.size(); i++) {
			SourcePath* currentSourcePath = _sourcePaths.at(i);

			if (!currentSourcePath->copy_file(_destinationPath)) {
				_rollbackHandler->rollback();
				return;
			}

			_rollbackHandler->add_action(new CopiedFileAction(currentSourcePath->_path, _destinationPath->_path));
		}
	}
	catch (...) {
		_rollbackHandler->rollback();
	}
}