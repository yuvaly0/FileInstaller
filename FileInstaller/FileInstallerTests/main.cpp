#include <windows.h>
#include <vector>
#include <memory>
#include <stdexcept>
#include "../FileInstaller/src/Actions/CreateDirectoryAction/CreateDirectoryAction.h"
#include "../FileInstaller/src/Installer/Installer.h"
#include "../FileInstaller/src/Utils/Utils.h"

#define GET_NAME(x) #x

namespace TestUtils {
	bool validateDirectoryCreated(LPCWSTR path) {
		const DWORD pathAttributes = GetFileAttributesW(path);
		auto isCreated = pathAttributes != INVALID_FILE_ATTRIBUTES;

		return isCreated;
	}

	void deleteDirectory(LPCWSTR path) {
		RemoveDirectoryW(path);
	}
}

bool validateCreateRelativeDirectory() {
	LPCWSTR relativePath = L".\\copyMe2";
	std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

	std::vector<std::shared_ptr<Action>> actions = {
		std::make_shared<CreateDirectoryAction>(relativePath)
	};

	auto installer = std::make_unique<Installer>(actions);
	installer->copy();

	bool isWorking = TestUtils::validateDirectoryCreated(absolutePath.get());

	if (isWorking) {
		TestUtils::deleteDirectory(absolutePath.get());
		return true;
	}

	return false;
}

bool validateCreateAbsoluteDirectory() {
	LPCWSTR relativePath = L".\\copyMe2";
	std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

	std::vector<std::shared_ptr<Action>> actions = {
		std::make_shared<CreateDirectoryAction>(absolutePath.get())
	};

	auto installer = std::make_unique<Installer>(actions);
	installer->copy();

	bool isWorking = TestUtils::validateDirectoryCreated(absolutePath.get());

	if (isWorking) {
		TestUtils::deleteDirectory(absolutePath.get());
		return true;
	}

	return false;
}

bool validateCreateNestedRelativeDirectory() {
	LPCWSTR relativePath = L".\\copyMe2\\copyMe3";
	std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

	std::vector<std::shared_ptr<Action>> actions = {
		std::make_shared<CreateDirectoryAction>(relativePath)
	};

	auto installer = std::make_unique<Installer>(actions);
	installer->copy();

	bool isWorking = TestUtils::validateDirectoryCreated(absolutePath.get());

	if (isWorking) {
		TestUtils::deleteDirectory(absolutePath.get());
		return true;
	}

	return false;
}

int main() {
	if (!validateCreateRelativeDirectory()) {
		throw std::runtime_error(GET_NAME(validateCreateRelativeDirectory));
	}

	if (!validateCreateAbsoluteDirectory()) {
		throw std::runtime_error(GET_NAME(validateCreateAbsoluteDirectory));
	}

	if(!validateCreateNestedRelativeDirectory()) {
		throw std::runtime_error(GET_NAME(validateCreateNestedRelativeDirectory));
	}
}