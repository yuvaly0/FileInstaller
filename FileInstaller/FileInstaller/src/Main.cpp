#include <memory>
#include "./Installer/Installer.h"

int main() {
	DestinationPath* destinationPath = new DestinationPath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe2");
	std::vector<SourcePath*> sourcePaths = {
			new SourcePath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\1.txt"),
			new SourcePath(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\10.txt")
	};

	std::unique_ptr<Installer> installer = std::make_unique<Installer>(destinationPath, sourcePaths);
	installer->copy();
}