#include <memory>
#include "./Installer/Installer.h"

int main() {
	LPCWSTR destinationPath = L".\\copyMe2\\copy3";
	
	// if we initialize "actions" using initialize list, std::vector might try to copy the values which 
	// will result in an error in the case of unique_ptr
	std::vector<std::unique_ptr<Action>> actions;
	actions.push_back(std::make_unique<CreateDirectoryAction>(destinationPath));
	actions.push_back(std::make_unique<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\\FileInstaller\\copyMe\\1.txt", destinationPath));
	actions.push_back(std::make_unique<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\\FileInstaller\\copyMe\\bla", destinationPath));
	actions.push_back(std::make_unique<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\\FileInstaller\\copyMe\\bla2", destinationPath));

	auto installer = std::make_unique<Installer>(std::move(actions));
	installer->copy();
}