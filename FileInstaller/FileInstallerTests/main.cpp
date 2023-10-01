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
		bool create(bool isRelative = false) {
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

		bool createNested(bool isRelative = false) {
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

		bool createPartialNested(bool isRelative = false) {
			// pre test
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
	
	    // test that all if the directory exists already
	}

	namespace CopyPath {
		LPCWSTR relativeSourcePath = L".\\file";
		LPCWSTR relativeDestinationPath = L".\\copyMe2";

		void initialize() {
			auto initializeComResult = CoInitialize(NULL);

			if (FAILED(initializeComResult)) {
				throw std::exception("couldn't initiailze COM");
			}
		}

		bool copy(bool fromRelative = false, bool toRelative = false, bool isSourcePathDirectory = false) {
			// pre test
			initialize();

			auto destinationPathAction = std::make_unique<CreateDirectoryAction>(relativeDestinationPath);
			destinationPathAction->act();

			auto sourcePathAction = std::make_unique<CreateDirectoryAction>(relativeSourcePath);

			if (isSourcePathDirectory) {
				sourcePathAction->act();
			}
			else {
				auto handle = CreateFileW(relativeSourcePath, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

				if (handle == INVALID_HANDLE_VALUE) {
					throw std::exception("could not create test file");
				}

				CloseHandle(handle);
			}
			
			// test
			std::shared_ptr<wchar_t[]> absoluteDestinationPath = Utils::getAbsolutePath(relativeDestinationPath);
			std::shared_ptr<wchar_t[]> absoluteTestPathName = Utils::getAbsolutePath(relativeSourcePath);

			std::unique_ptr<CopyPathAction> action;

			if (fromRelative && toRelative) {
				action = std::make_unique<CopyPathAction>(relativeSourcePath, relativeDestinationPath);
			}
			else if (!fromRelative && toRelative) {
				action = std::make_unique<CopyPathAction>(absoluteTestPathName.get(), relativeDestinationPath);
			}
			else if (fromRelative && !toRelative) {
				action = std::make_unique<CopyPathAction>(relativeSourcePath, absoluteDestinationPath.get());
			}
			else {
				action = std::make_unique<CopyPathAction>(absoluteTestPathName.get(), absoluteDestinationPath.get());
			}

			action->act();

			const bool hasWorked = Utils::isPathExists(L".\\copyMe2\\file");

			if (hasWorked) {
				if (isSourcePathDirectory) {
					sourcePathAction->rollback();
				}
				else {
					DeleteFileW(relativeSourcePath);
				}
				
				action->rollback();
				destinationPathAction->rollback();
				CoUninitialize();
				return true;
			}

			CoUninitialize();
			return false;
		};
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

			return isWorking;
		}

		bool rollbackCreateDirectoryPartialNested(bool isRelative = false) {
			// pre test
			LPCWSTR preTestPath = L".\\copyMe2";
			auto preTestAction = std::make_unique<CreateDirectoryAction>(preTestPath);
			preTestAction->act();

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
				preTestAction->rollback();
				return true;
			}

			return false;
		}
	
		bool rollbackCopy(bool isDirectory = false) { return true; }
	}
}

int main() {
	if (!Tests::CreateDirectory::create()) {
		throw std::runtime_error("");
	}

	if (!Tests::CreateDirectory::create(true)) {
		throw std::runtime_error("");
	}

	if(!Tests::CreateDirectory::createNested()) {
		throw std::runtime_error("");
	}

	if (!Tests::CreateDirectory::createNested(true)) {
		throw std::runtime_error("");
	}

	if (!Tests::CreateDirectory::createPartialNested()) {
		throw std::runtime_error("");
	}

	if (!Tests::CreateDirectory::createPartialNested(true)) {
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

	if (!Tests::Rollback::rollbackCreateDirectoryPartialNested()) {
		throw std::runtime_error("");
	}

	if (!Tests::Rollback::rollbackCreateDirectoryPartialNested(true)) {
		throw std::runtime_error("");
	}

	if (!Tests::CopyPath::copy(false, false)) {
		throw std::exception("");
	}

	if (!Tests::CopyPath::copy(false, true)) {
		throw std::exception("");
	}

	if (!Tests::CopyPath::copy(true, false)) {
		throw std::exception("");
	}

	if (!Tests::CopyPath::copy(true, true)) {
		throw std::exception("");
	}

	if (!Tests::CopyPath::copy(false, false, true)) {
		throw std::exception("");
	}

	if (!Tests::CopyPath::copy(false, true, true)) {
		throw std::exception("");
	}

	if (!Tests::CopyPath::copy(true, false, true)) {
		throw std::exception("");
	}

	if (!Tests::CopyPath::copy(true, true, true)) {
		throw std::exception("");
	}
}