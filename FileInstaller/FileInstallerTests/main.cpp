#include <windows.h>
#include <vector>
#include <memory>
#include <stdexcept>
#include "../FileInstaller/src/Actions/CreateDirectoryAction/CreateDirectoryAction.h"
#include "../FileInstaller/src/Installer/Installer.h"
#include "../FileInstaller/src/Utils/Utils.h"
#include "../FileInstaller/src/Exceptions/InstallerException.h"

#define GET_NAME(x) #x

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

			std::vector<std::shared_ptr<WCHAR[]>> directoriesToBeCreated = Utils::getDirectoriesToBeCreated(absolutePath.get());

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

	namespace Rollback {
		class A : public Action {
		public:
			void act() {
				throw InstallerException("");
			}
			void rollback() {}
		};

		bool rollbackCreateDirectoryRelative() {
			LPCWSTR relativePath = L".\\copyMe2";
			std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

			{
				std::vector<std::shared_ptr<Action>> actions = {
					std::make_shared<CreateDirectoryAction>(relativePath),
					std::make_shared<A>()
				};

				auto installer = std::make_unique<Installer>(actions);
				installer->copy();
			}

			bool isWorking = !TestUtils::Validate::directoryExists(absolutePath.get());

			return isWorking;
		}

		bool rollbackCreateDirectoryAbsolute() {
			LPCWSTR relativePath = L".\\copyMe2";
			std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

			{
				std::vector<std::shared_ptr<Action>> actions = {
					std::make_shared<CreateDirectoryAction>(absolutePath.get()),
					std::make_shared<A>()
				};

				auto installer = std::make_unique<Installer>(actions);
				installer->copy();
			}

			bool isWorking = !TestUtils::Validate::directoryExists(absolutePath.get());

			return isWorking;
		}

		bool rollbackCreateDirectoryNestedRelative() {
			LPCWSTR relativePath = L".\\copyMe2\\copyMe3";
			std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);

			std::vector<std::shared_ptr<WCHAR[]>> directoriesToBeCreated = Utils::getDirectoriesToBeCreated(absolutePath.get());

			{
				std::vector<std::shared_ptr<Action>> actions = {
								std::make_shared<CreateDirectoryAction>(relativePath),
								std::make_shared<A>()
				};

				auto installer = std::make_unique<Installer>(actions);
				installer->copy();
			}

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
	if (!Tests::CreateDirectory::CreateRelative()) {
		throw std::runtime_error(GET_NAME(validateCreateRelativeDirectory));
	}

	if (!Tests::CreateDirectory::CreateAbsolute()) {
		throw std::runtime_error(GET_NAME(validateCreateAbsoluteDirectory));
	}

	if(!Tests::CreateDirectory::CreateNestedRelative()) {
		throw std::runtime_error(GET_NAME(validateCreateNestedRelativeDirectory));
	}

	if (!Tests::Rollback::rollbackCreateDirectoryRelative()) {
		throw std::runtime_error(GET_NAME(rollbackCreateDirectoryRelative));
	}

	if (!Tests::Rollback::rollbackCreateDirectoryAbsolute()) {
		throw std::runtime_error(GET_NAME(rollbackCreateDirectoryAbsolute));
	}

	if (!Tests::Rollback::rollbackCreateDirectoryNestedRelative()) {
		throw std::runtime_error(GET_NAME(rollbackCreateDirectoryNestedRelative));
	}
}