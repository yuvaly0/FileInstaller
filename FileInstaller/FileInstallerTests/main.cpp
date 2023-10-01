#include <windows.h>
#include <vector>
#include <memory>
#include <stdexcept>
#include "../FileInstaller/src/Actions/CreateDirectoryAction/CreateDirectoryAction.h"
#include "../FileInstaller/src/Installer/Installer.h"
#include "../FileInstaller/src/Utils/Utils.h"

#define GET_NAME(x) #x

namespace TestUtils {
	namespace Validate {
		bool directoryExists(LPCWSTR path) {
			const DWORD pathAttributes = GetFileAttributesW(path);
			auto isCreated = pathAttributes != INVALID_FILE_ATTRIBUTES;

			return isCreated;
		}
	}
	
	void deleteDirectory(LPCWSTR path) {
		RemoveDirectoryW(path);
	}

	void deleteNestedDirectory(std::vector<std::shared_ptr<WCHAR>> paths) {
		for (auto it = paths.rbegin(); it != paths.rend(); ++it) {
			deleteDirectory((*it).get());
		}
	}
}

namespace Tests {
	namespace CreateDirectory {
		bool CreateRelative() {
			LPCWSTR relativePath = L".\\copyMe2";
			std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

			std::vector<std::shared_ptr<Action>> actions = {
				std::make_shared<CreateDirectoryAction>(relativePath)
			};

			auto installer = std::make_unique<Installer>(actions);
			installer->copy();

			bool isWorking = TestUtils::Validate::directoryExists(absolutePath.get());

			if (isWorking) {
				TestUtils::deleteDirectory(absolutePath.get());
				return true;
			}

			return false;
		}

		bool CreateAbsolute() {
			LPCWSTR relativePath = L".\\copyMe2";
			std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

			std::vector<std::shared_ptr<Action>> actions = {
				std::make_shared<CreateDirectoryAction>(absolutePath.get())
			};

			auto installer = std::make_unique<Installer>(actions);
			installer->copy();

			bool isWorking = TestUtils::Validate::directoryExists(absolutePath.get());

			if (isWorking) {
				TestUtils::deleteDirectory(absolutePath.get());
				return true;
			}

			return false;
		}

		bool CreateNestedRelative() {
			LPCWSTR relativePath = L".\\copyMe2\\copyMe3";
			std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

			WCHAR currentPath[MAX_PATH] = {};
			std::vector<std::shared_ptr<WCHAR>> directoriesToBeCreated = {};
			LPCWSTR cursor = absolutePath.get();

			// find the directories in the heirarchy that doesnt exist
			while (*cursor) {
				if (*cursor == L'\\' || *(cursor + 1) == L'\0') {
					auto length = cursor - absolutePath.get() + 1;
					lstrcpyn(currentPath, absolutePath.get(), length + 1);

					if (!TestUtils::Validate::directoryExists(currentPath)) {
						std::shared_ptr<WCHAR> toBeCopiedPath(new WCHAR[MAX_PATH], std::default_delete<WCHAR[]>());
						lstrcpyn(toBeCopiedPath.get(), absolutePath.get(), length + 1);
						directoriesToBeCreated.push_back(toBeCopiedPath);
					}
				}

				cursor++;
			}

			std::vector<std::shared_ptr<Action>> actions = {
				std::make_shared<CreateDirectoryAction>(relativePath)
			};

			auto installer = std::make_unique<Installer>(actions);
			installer->copy();

			bool isWorking = TestUtils::Validate::directoryExists(absolutePath.get());

			if (isWorking) {
				TestUtils::deleteNestedDirectory(directoriesToBeCreated);
				return true;
			}

			return false;
		}
	}
}

int main() {
	if (!Tests::CreateDirectory::CreateRelative()) {
		throw std::runtime_error(GET_NAME(validateCreateRelativeDirectory));
	}

	if (!Tests::CreateDirectory::CreateAbsolute()) {
		throw std::runtime_error(GET_NAME(validateCreateAbsoluteDirectory));
	}

	if(!Tests::CreateDirectory::CreateNestedRelative()) {
		throw std::runtime_error(GET_NAME(validateCreateNestedRelativeDirectory));
	}
}