#include <memory>
#include "./Installer/Installer.h"

int main() {
	LPCWSTR destinationPath = L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe2";
	std::vector<std::shared_ptr<Action>> actions = {
		std::make_shared<CreateDirectoryAction>(destinationPath),
		std::make_shared<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\1.txt", destinationPath),
		std::make_shared<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\bla", destinationPath),
		std::make_shared<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\bla4", destinationPath),
		std::make_shared<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\bla3", destinationPath)
	};

	auto installer = std::make_unique<Installer>(actions);
	installer->copy();
}