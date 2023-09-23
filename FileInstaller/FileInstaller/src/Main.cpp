#include <iostream>
#include "./Installer/Installer.h"

int main() {
	// TODO: unique_ptr
	Installer* installer = new Installer();
	installer->copy();
}