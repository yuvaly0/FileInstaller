#include <memory>
#include "./Installer/Installer.h"

int main() {
	std::shared_ptr<DestinationPath> destinationPath = std::make_shared<DestinationPath>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe2");
	std::vector<std::shared_ptr<SourcePath>> sourcePaths = {
			std::make_shared<SourcePath>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\1.txt"),
			std::make_shared<SourcePath>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\10.txt")
	};

	std::unique_ptr<Installer> installer = std::make_unique<Installer>(destinationPath, sourcePaths);
	installer->copy();
}