#include <windows.h>
#include <vector>
#include <memory>
#include <stdexcept>
#include <fileapi.h>
#include "../FileInstaller/src/Actions/CreateDirectoryAction/CreateDirectoryAction.h"
#include "../FileInstaller/src/Installer/Installer.h"
#include "../FileInstaller/src/Utils/Utils.h"
#include "../FileInstaller/src/Exceptions/InstallerException.h"

namespace TestUtils {
	namespace Validate {
		bool directoryExists(LPCWSTR path) {
			const DWORD pathAttributes = GetFileAttributesW(path);
			auto isCreated = pathAttributes != INVALID_FILE_ATTRIBUTES;

			return isCreated;
		}

		bool directoriesDeleted(std::vector<std::shared_ptr<WCHAR[]>> paths) {
			for (auto it = paths.begin(); it != paths.end(); ++it) {
				if (directoryExists(((*it).get()))) {
					return false;
				}
			}

			return true;
		}
	}
	
	void deleteDirectory(LPCWSTR path) {
		RemoveDirectoryW(path);
	}

	void deleteNestedDirectory(std::vector<std::shared_ptr<WCHAR[]>> paths) {
		for (auto it = paths.rbegin(); it != paths.rend(); ++it) {
			deleteDirectory((*it).get());
		}
	}
}

namespace Tests {
	namespace CreateDirectory {
		bool Create(bool isRelative = false) {
			LPCWSTR relativePath = L".\\copyMe2";
			std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

			if (isRelative) {
				auto action = std::make_unique<CreateDirectoryAction>(relativePath);
 				action->act();
			}
			else {
				auto action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
				action->act();
			}

			bool isWorking = TestUtils::Validate::directoryExists(absolutePath.get());

			if (isWorking) {
				TestUtils::deleteDirectory(absolutePath.get());
				return true;
			}

			return false;
		}

		bool CreateNested(bool isRelative = false) {
			LPCWSTR relativePath = L".\\copyMe2\\copyMe3";
			std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

			std::vector<std::shared_ptr<WCHAR[]>> directoriesToBeCreated = Utils::getDirectoriesToBeCreated(absolutePath.get());

			if (isRelative) {
				auto action = std::make_unique<CreateDirectoryAction>(relativePath);
				action->act();
			}
			else {
				auto action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
				action->act();
			}

			bool isWorking = TestUtils::Validate::directoryExists(absolutePath.get());

			if (isWorking) {
				TestUtils::deleteNestedDirectory(directoriesToBeCreated);
				return true;
			}

			return false;
		}

		bool CreatePartialNested(bool isRelative = false) {
			LPCWSTR preTestPath = L".\\copyMe2";
			auto preTestAction = std::make_unique<CreateDirectoryAction>(preTestPath);
			preTestAction->act();


			LPCWSTR relativePath = L".\\copyMe2\\copyMe3";
			std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

			std::vector<std::shared_ptr<WCHAR[]>> directoriesToBeCreated = Utils::getDirectoriesToBeCreated(absolutePath.get());

			if (isRelative) {
				auto action = std::make_unique<CreateDirectoryAction>(relativePath);
				action->act();
			}
			else {
				auto action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
				action->act();
			}

			bool isWorking = TestUtils::Validate::directoryExists(absolutePath.get());

			if (isWorking) {
				TestUtils::deleteNestedDirectory(directoriesToBeCreated);
				preTestAction->rollback();
				return true;
			}

			return false;
		}
	}

	namespace Rollback {
		class A : public Action {
		public:
			void act() {
				throw InstallerException("");
			}
			void rollback() {}
		};

		bool rollbackCreateDirectory(bool isRelative = false) {
			LPCWSTR relativePath = L".\\copyMe2";
			std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);
			std::unique_ptr<CreateDirectoryAction> action;

			if (isRelative) {
				action = std::make_unique<CreateDirectoryAction>(relativePath);
			}
			else {
				action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
			}

			action->act();
			action->rollback();

			bool isWorking = !TestUtils::Validate::directoryExists(absolutePath.get());

			return isWorking;
		}

		bool rollbackCreateDirectoryNested(bool isRelative = false) {
			LPCWSTR relativePath = L".\\copyMe2\\copyMe3";
			std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

			std::vector<std::shared_ptr<WCHAR[]>> directoriesToBeCreated = Utils::getDirectoriesToBeCreated(absolutePath.get());
			std::unique_ptr<CreateDirectoryAction> action;

			if (isRelative) {
				action = std::make_unique<CreateDirectoryAction>(relativePath);
			}
			else {
				action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
			}
			
			action->act();
			action->rollback();

			bool isWorking = TestUtils::Validate::directoriesDeleted(directoriesToBeCreated);

			if (isWorking) {
				TestUtils::deleteNestedDirectory(directoriesToBeCreated);
				return true;
			}

			return false;
		}
	}
}

int main() {
	if (!Tests::CreateDirectory::Create()) {
		throw std::runtime_error("");
	}

	if (!Tests::CreateDirectory::Create(true)) {
		throw std::runtime_error("");
	}

	if(!Tests::CreateDirectory::CreateNested()) {
		throw std::runtime_error("");
	}

	if (!Tests::CreateDirectory::CreateNested(true)) {
		throw std::runtime_error("");
	}

	if (!Tests::CreateDirectory::CreatePartialNested()) {
		throw std::runtime_error("");
	}

	if (!Tests::CreateDirectory::CreatePartialNested(true)) {
		throw std::runtime_error("");
	}

	if (!Tests::Rollback::rollbackCreateDirectory()) {
		throw std::runtime_error("");
	}

	if (!Tests::Rollback::rollbackCreateDirectory(true)) {
		throw std::runtime_error("");
	}

	if (!Tests::Rollback::rollbackCreateDirectoryNested()) {
		throw std::runtime_error("");
	}

	if (!Tests::Rollback::rollbackCreateDirectoryNested(true)) {
		throw std::runtime_error("");
	}
}