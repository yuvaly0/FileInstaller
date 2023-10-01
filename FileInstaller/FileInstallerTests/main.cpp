#include <windows.h>
#include <vector>
#include <memory>
#include <stdexcept>
#include <fileapi.h>
#include <gtest/gtest.h>
#include <tuple>
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

class CreateDirectoryFixture : public ::testing::TestWithParam<bool> {};

TEST_P(CreateDirectoryFixture, Create) {
	bool isRelative = GetParam();

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

	bool doesDirectoryExist = Utils::isPathExists(absolutePath.get());

	if (doesDirectoryExist) {
		TestUtils::deleteDirectory(absolutePath.get());
	}

	EXPECT_TRUE(doesDirectoryExist);
}

TEST_P(CreateDirectoryFixture, CreateNested) {
	bool isRelative = GetParam();

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

	bool doesDirectoryExist = TestUtils::Validate::directoryExists(absolutePath.get());

	if (doesDirectoryExist) {
		TestUtils::deleteNestedDirectory(directoriesToBeCreated);
	}

	EXPECT_TRUE(doesDirectoryExist);
}

TEST_P(CreateDirectoryFixture, CreatePartialNested) {
	bool isRelative = GetParam();

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

	bool doesDirectoryExist = TestUtils::Validate::directoryExists(absolutePath.get());

	if (doesDirectoryExist) {
		TestUtils::deleteNestedDirectory(directoriesToBeCreated);
		preTestAction->rollback();		
	}

	EXPECT_TRUE(doesDirectoryExist);
}

TEST_P(CreateDirectoryFixture, CreateWhenAlreadyExists) {
	bool isRelative = GetParam();

	LPCWSTR relativePath = L".\\copyMe2";
	std::shared_ptr<wchar_t[]> absolutePath = Utils::getAbsolutePath(relativePath);
	auto preTestCreation = std::make_unique<CreateDirectoryAction>(relativePath);
	preTestCreation->act();

	std::unique_ptr<CreateDirectoryAction> action;

	if (isRelative) {
		action = std::make_unique<CreateDirectoryAction>(relativePath);
	}
	else {
		action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
	}

	EXPECT_NO_THROW(action->act());

	preTestCreation->rollback();
}

INSTANTIATE_TEST_SUITE_P(MyValues, CreateDirectoryFixture, ::testing::Values(true, false));

class CopyPathActionFixture : public ::testing::TestWithParam<std::tuple<bool, bool, bool>> {};

TEST_P(CopyPathActionFixture, Copy) {
	bool fromRelative = std::get<0>(GetParam());
	bool toRelative = std::get<1>(GetParam());
	bool isSourcePathDirectory = std::get<2>(GetParam());

 	LPCWSTR relativeSourcePath = L".\\file";
	LPCWSTR relativeDestinationPath = L".\\copyMe2";

	auto initializeComResult = CoInitialize(NULL);

	if (FAILED(initializeComResult)) {
		throw std::exception("couldn't initiailze COM");
	}

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

	const bool doesPathExists = Utils::isPathExists(L".\\copyMe2\\file");

	if (doesPathExists) {
		if (isSourcePathDirectory) {
			sourcePathAction->rollback();
		}
		else {
			DeleteFileW(relativeSourcePath);
		}

		action->rollback();
		destinationPathAction->rollback();
	}

	CoUninitialize();
	
	EXPECT_TRUE(doesPathExists);
}

TEST_P(CopyPathActionFixture, rollbackCopy) {
	bool fromRelative = std::get<0>(GetParam());
	bool toRelative = std::get<1>(GetParam());
	bool isSourcePathDirectory = std::get<2>(GetParam());

	// pre test
	LPCWSTR relativeSourcePath = L".\\file";
	LPCWSTR relativeDestinationPath = L".\\copyMe2";

	auto initializeComResult = CoInitialize(NULL);

	if (FAILED(initializeComResult)) {
		throw std::exception("couldn't initiailze COM");
	}

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
	std::shared_ptr<WCHAR[]> absoluteDestinationPath = Utils::getAbsolutePath(relativeDestinationPath);
	std::shared_ptr<WCHAR[]> absoluteTestPathName = Utils::getAbsolutePath(relativeSourcePath);

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
	action->rollback();

	const bool doesPathExists = Utils::isPathExists(L".\\copyMe2\\file");

	if (!doesPathExists) {
		if (isSourcePathDirectory) {
			sourcePathAction->rollback();
		}
		else {
			DeleteFileW(relativeSourcePath);
		}

		destinationPathAction->rollback();
	}

	CoUninitialize();
	EXPECT_FALSE(doesPathExists);
}

INSTANTIATE_TEST_SUITE_P(CopyPath, CopyPathActionFixture,
	::testing::Values(
		std::make_tuple(false, false, false),
		std::make_tuple(false, true, false),
		std::make_tuple(true, false, false),
		std::make_tuple(true, true, false),
		std::make_tuple(false, false, true),
		std::make_tuple(false, true, true),
		std::make_tuple(true, false, true),
		std::make_tuple(true, true, true)
	)
);

class RollbackFixture : public ::testing::TestWithParam<bool> {};

TEST_P(RollbackFixture, rollbackCreateDirectory) {
	bool isRelative = GetParam();

	LPCWSTR relativePath = L".\\copyMe2";
	std::shared_ptr<WCHAR[]> absolutePath = Utils::getAbsolutePath(relativePath);
	std::unique_ptr<CreateDirectoryAction> action;

	if (isRelative) {
		action = std::make_unique<CreateDirectoryAction>(relativePath);
	}
	else {
		action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
	}

	action->act();
	action->rollback();

	bool doesDirectoryExists = TestUtils::Validate::directoryExists(absolutePath.get());

	EXPECT_FALSE(doesDirectoryExists);
}

TEST_P(RollbackFixture, rollbackCreateDirectoryNested) {
	bool isRelative = GetParam();

	LPCWSTR relativePath = L".\\copyMe2\\copyMe3";
	std::shared_ptr<WCHAR[]> absolutePath = Utils::getAbsolutePath(relativePath);

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

	bool areDirectoriesDeleted = TestUtils::Validate::directoriesDeleted(directoriesToBeCreated);

	EXPECT_TRUE(areDirectoriesDeleted);
}

TEST_P(RollbackFixture, rollbackCreateDirectoryPartialNested) {
	bool isRelative = GetParam();

	// pre test
	LPCWSTR preTestPath = L".\\copyMe2";
	auto preTestAction = std::make_unique<CreateDirectoryAction>(preTestPath);
	preTestAction->act();

	LPCWSTR relativePath = L".\\copyMe2\\copyMe3";
	std::shared_ptr<WCHAR[]> absolutePath = Utils::getAbsolutePath(relativePath);

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

	bool areDirectoriesDeleted = TestUtils::Validate::directoriesDeleted(directoriesToBeCreated);

	if (areDirectoriesDeleted) {
		preTestAction->rollback();
	}

	EXPECT_TRUE(areDirectoriesDeleted);
}

TEST_P(RollbackFixture, rollbackWhenAlreadyExisted) {
	bool isRelative = GetParam();

	LPCWSTR relativePath = L".\\copyMe2";
	std::shared_ptr<WCHAR[]> absolutePath = Utils::getAbsolutePath(relativePath);
	auto preTestCreation = std::make_unique<CreateDirectoryAction>(relativePath);
	preTestCreation->act();

	std::unique_ptr<CreateDirectoryAction> action;

	if (isRelative) {
		action = std::make_unique<CreateDirectoryAction>(relativePath);
	}
	else {
		action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
	}

	action->act();
	action->rollback();

	const bool doesDirectoryExists = Utils::isPathExists(relativePath);

	if (doesDirectoryExists) {
		preTestCreation->rollback();
	}

	EXPECT_TRUE(doesDirectoryExists);
}

INSTANTIATE_TEST_SUITE_P(rollback, RollbackFixture, ::testing::Values(true, false));

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}