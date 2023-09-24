#include <windows.h>
#include "Installer.h"

// TODO: make singleton
Installer::Installer() {
	_destinationPath = new DestinationPath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise");
	_sourcePaths = {
		new SourcePath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\1.txt")
	};
	_hasCreatedDestinationFolder = false;
}

void Installer::copy() {
	/*
	if (!_destinationPath->tryCreate()) {
		revert();
	}
	*/

	for (auto i = 0; i < _sourcePaths.size(); i++) {
		SourcePath* currentSourcePath = _sourcePaths.at(i);
		
		if (!currentSourcePath->copy_file(_destinationPath)) {
			revert();
			return;
		}
	}
}

void Installer::revert() {

}