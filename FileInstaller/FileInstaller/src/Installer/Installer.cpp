#include <windows.h>
#include "Installer.h"
#include "../Path/DestinationPath/DestinationPath.h"
#include "../Path/SourcePath/SourcePath.h"

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

			
	// 2. 
}