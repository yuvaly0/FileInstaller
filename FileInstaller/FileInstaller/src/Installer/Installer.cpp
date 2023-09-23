#include <windows.h>
#include "Installer.h"

// TODO: make singleton
Installer::Installer() {
	_destinationPath = new DestinationPath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise");
	_sourcePaths = {
		new SourcePath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\1.txt"),
		new SourcePath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\2.txt"),
		new SourcePath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\3.txt"),
		new SourcePath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\4.txt"),
		new SourcePath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\5.txt"),
		new SourcePath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\6.txt")
	};
	_hasCreatedDestinationFolder = false;
}

void Installer::copy() {
	if (!_destinationPath->tryCreate()) {
		revert();
	}

	for (auto i = 0; i < _sourcePaths.size(); i++) {
		SourcePath* currentSourcePath = _sourcePaths.at(i);
		
		if (!currentSourcePath->pre_copy_verify()) {
			revert();
			return;
		}

		bool hasSuccedded = copy_file(currentSourcePath);
		
		if (!hasSuccedded) {
			revert();
			return;
		}
	}
}

bool Installer::copy_file(Path* sourcePath) {
	return true;
}

void Installer::revert() {

}