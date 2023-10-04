#include <memory>
#include "./Installer/Installer.h"

int main() {
	LPCWSTR destinationPath = L".\\copyMe2\\copy3";
	
	std::vector<std::unique_ptr<Action>> actions;
	actions.push_back(std::make_unique<CreateDirectoryAction>(destinationPath));
	actions.push_back(std::make_unique<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\\FileInstaller\\copyMe\\1.txt", destinationPath));
	actions.push_back(std::make_unique<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\\FileInstaller\\copyMe\\bla", destinationPath));
	actions.push_back(std::make_unique<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\\FileInstaller\\copyMe\\bla2", destinationPath));

	auto installer = std::make_unique<Installer>(std::move(actions));
	installer->copy();
}