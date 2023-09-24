#include <windows.h>
#include <memory>
#include "Installer.h"
#include "../Rollback/Actions/CopiedFile/CopiedFileAction.h"
#include "../Rollback/Actions/CreatedDirRollbackAction/CreatedDirRollbackAction.h"

// TODO: make singleton
Installer::Installer(std::shared_ptr<DestinationPath> destinationPath, std::vector<std::shared_ptr<SourcePath>> sourcePaths)
	: _destinationPath(destinationPath), _sourcePaths(sourcePaths) {
	
	_rollbackHandler = std::make_unique<RollbackHandler>();
}

void Installer::copy() {
	try {
		DestinationPath::SuccessResults result = _destinationPath->tryCreate();

		if (result == DestinationPath::CREATED_DIRECTORY) {
			_rollbackHandler->add_action(std::make_unique<CreatedDirRollbackAction>(_destinationPath->_path));
		}

		for (auto i = 0; i < _sourcePaths.size(); i++) {
			std::shared_ptr<SourcePath> currentSourcePath = _sourcePaths.at(i);
			currentSourcePath->copy_file(_destinationPath);

			_rollbackHandler->add_action(std::make_unique<CopiedFileAction>(currentSourcePath->_path, _destinationPath->_path));
		}
	}
	catch (...) {
		_rollbackHandler->rollback();
	}
}