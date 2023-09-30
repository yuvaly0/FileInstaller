#include <memory>
#include "./Installer/Installer.h"

int main() {
	LPCWSTR destinationPath = L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe2";
	// LPCWSTR destinationPath = L".\\copyMe2";
	std::vector<LPCWSTR> sourcePaths = {
		L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\1.txt",
		L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\bla",
		L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\bla2",
		L"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\bla3"
	};

	auto installer = std::make_unique<Installer>(destinationPath, sourcePaths);
	installer->copy();
}