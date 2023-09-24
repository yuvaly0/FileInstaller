#include <memory>
#include "./Installer/Installer.h"

int main() {
	std::unique_ptr<Installer> installer = std::make_unique<Installer>();
	installer->copy();
}