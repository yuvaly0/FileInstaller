#include <windows.h>
#include <vector>
#include <memory>
#include <stdexcept>
#include <fileapi.h>
#include <gtest/gtest.h>
#include <tuple>
#include <filesystem>
#include <fstream>
#include "../FileInstaller/src/Actions/CreateDirectoryAction/CreateDirectoryAction.h"
#include "../FileInstaller/src/Installer/Installer.h"
#include "../FileInstaller/src/Utils/Utils.h"
#include "../FileInstaller/src/Exceptions/InstallerException.h"

namespace fs = std::filesystem;

namespace TestUtils {
	namespace Validate {
		bool directoriesDeleted(std::vector<std::shared_ptr<WCHAR[]>> paths) {
			for (auto it = paths.begin(); it != paths.end(); ++it) {
				if (Utils::isPathExists(((*it).get()))) {
					return false;
				}
			}

			return true;
		}
	}

	void deleteNestedDirectory(std::vector<std::shared_ptr<WCHAR[]>> paths) {
		for (auto it = paths.rbegin(); it != paths.rend(); ++it) {
			fs::remove((*it).get());
		}
	}
}

class CreateDirectoryFixture : public ::testing::TestWithParam<bool> {};

TEST_P(CreateDirectoryFixture, Create) {
	// pre test
	bool isRelative = GetParam();

	LPCWSTR relativePath = L".\\copyMe2";
	std::shared_ptr<WCHAR[]> absolutePath = Utils::getAbsolutePath(relativePath);

	// test
	if (isRelative) {
		auto action = std::make_unique<CreateDirectoryAction>(relativePath);
		action->act();
	}
	else {
		auto action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
		action->act();
	}

	bool doesDirectoryExist = Utils::isPathExists(absolutePath.get());

	EXPECT_TRUE(doesDirectoryExist);

	// post test
	if (doesDirectoryExist) {
		fs::remove(absolutePath.get());
	}
}

TEST_P(CreateDirectoryFixture, CreateNested) {
	// pre test
	bool isRelative = GetParam();

	LPCWSTR relativePath = L".\\copyMe2\\copyMe3";
	std::shared_ptr<WCHAR[]> absolutePath = Utils::getAbsolutePath(relativePath);

	std::vector<std::shared_ptr<WCHAR[]>> directoriesToBeCreated = Utils::getDirectoriesToBeCreated(absolutePath.get());

	// test
	if (isRelative) {
		auto action = std::make_unique<CreateDirectoryAction>(relativePath);
		action->act();
	}
	else {
		auto action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
		action->act();
	}

	bool doesDirectoryExist = Utils::isPathExists(absolutePath.get());

	EXPECT_TRUE(doesDirectoryExist);

	// post test
	if (doesDirectoryExist) {
		TestUtils::deleteNestedDirectory(directoriesToBeCreated);
	}
}

TEST_P(CreateDirectoryFixture, CreatePartialNested) {
	// pre test
	bool isRelative = GetParam();

	LPCWSTR preTestPath = L".\\copyMe2";
	fs::create_directory(preTestPath);

	LPCWSTR relativePath = L".\\copyMe2\\copyMe3";
	std::shared_ptr<WCHAR[]> absolutePath = Utils::getAbsolutePath(relativePath);

	std::vector<std::shared_ptr<WCHAR[]>> directoriesToBeCreated = Utils::getDirectoriesToBeCreated(absolutePath.get());

	// test
	if (isRelative) {
		auto action = std::make_unique<CreateDirectoryAction>(relativePath);
		action->act();
	}
	else {
		auto action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
		action->act();
	}

	bool doesDirectoryExist = Utils::isPathExists(absolutePath.get());

	EXPECT_TRUE(doesDirectoryExist);

	// post test
	if (doesDirectoryExist) {
		TestUtils::deleteNestedDirectory(directoriesToBeCreated);
		fs::remove(preTestPath);
	}
}

TEST_P(CreateDirectoryFixture, CreateWhenAlreadyExists) {
	// pre test
	bool isRelative = GetParam();

	LPCWSTR relativePath = L".\\copyMe2";
	fs::create_directory(relativePath);

	// test
	std::shared_ptr<WCHAR[]> absolutePath = Utils::getAbsolutePath(relativePath);
	std::unique_ptr<CreateDirectoryAction> action;

	if (isRelative) {
		action = std::make_unique<CreateDirectoryAction>(relativePath);
	}
	else {
		action = std::make_unique<CreateDirectoryAction>(absolutePath.get());
	}

	EXPECT_NO_THROW(action->act());

	// post test
	fs::remove(relativePath);
}

INSTANTIATE_TEST_SUITE_P(MyValues, CreateDirectoryFixture, ::testing::Values(true, false));

class CopyPathActionFixture : public ::testing::TestWithParam<std::tuple<bool, bool, bool>> {
protected:
	LPCWSTR _relativeSourcePath = L".\\file";
	LPCWSTR _relativeDestinationPath = L".\\copyMe2";

	static void SetUpTestCase() {
		auto initializeComResult = CoInitialize(NULL);

		if (FAILED(initializeComResult)) {
			throw std::exception("couldn't initiailze COM");
		}
	}

	static void TearDownTestCase() {
		CoUninitialize();
	}

	void SetUp() override {
		fs::create_directory(CopyPathActionFixture::_relativeDestinationPath);
	}
};

TEST_P(CopyPathActionFixture, Copy) {
	// pre test
	bool fromRelative = std::get<0>(GetParam());
	bool toRelative = std::get<1>(GetParam());
	bool isSourcePathDirectory = std::get<2>(GetParam());

	if (isSourcePathDirectory) {
		fs::create_directory(_relativeSourcePath);
	}
	else {
		std::ofstream handle(_relativeSourcePath);
		handle.close();
	}

	// test
	std::shared_ptr<WCHAR[]> absoluteDestinationPath = Utils::getAbsolutePath(_relativeDestinationPath);
	std::shared_ptr<WCHAR[]> absoluteTestPathName = Utils::getAbsolutePath(_relativeSourcePath);

	std::unique_ptr<CopyPathAction> action;

	if (fromRelative && toRelative) {
		action = std::make_unique<CopyPathAction>(_relativeSourcePath, _relativeDestinationPath);
	}
	else if (!fromRelative && toRelative) {
		action = std::make_unique<CopyPathAction>(absoluteTestPathName.get(), _relativeDestinationPath);
	}
	else if (fromRelative && !toRelative) {
		action = std::make_unique<CopyPathAction>(_relativeSourcePath, absoluteDestinationPath.get());
	}
	else {
		action = std::make_unique<CopyPathAction>(absoluteTestPathName.get(), absoluteDestinationPath.get());
	}

	action->act();

	const bool doesPathExists = Utils::isPathExists(L".\\copyMe2\\file");
	
	EXPECT_TRUE(doesPathExists);

	// post test
	if (doesPathExists) {
		fs::remove(_relativeSourcePath);
		action->rollback();
		fs::remove(_relativeDestinationPath);

	}
}

TEST_P(CopyPathActionFixture, rollbackCopy) {
	// pre test
	bool fromRelative = std::get<0>(GetParam());
	bool toRelative = std::get<1>(GetParam());
	bool isSourcePathDirectory = std::get<2>(GetParam());

	if (isSourcePathDirectory) {
		fs::create_directory(_relativeSourcePath);
	}
	else {
		std::ofstream handle(_relativeSourcePath);
		handle.close();
	}

	// test
	std::shared_ptr<WCHAR[]> absoluteDestinationPath = Utils::getAbsolutePath(_relativeDestinationPath);
	std::shared_ptr<WCHAR[]> absoluteTestPathName = Utils::getAbsolutePath(_relativeSourcePath);

	std::unique_ptr<CopyPathAction> action;

	if (fromRelative && toRelative) {
		action = std::make_unique<CopyPathAction>(_relativeSourcePath, _relativeDestinationPath);
	}
	else if (!fromRelative && toRelative) {
		action = std::make_unique<CopyPathAction>(absoluteTestPathName.get(), _relativeDestinationPath);
	}
	else if (fromRelative && !toRelative) {
		action = std::make_unique<CopyPathAction>(_relativeSourcePath, absoluteDestinationPath.get());
	}
	else {
		action = std::make_unique<CopyPathAction>(absoluteTestPathName.get(), absoluteDestinationPath.get());
	}

	action->act();
	action->rollback();

	const bool doesPathExists = Utils::isPathExists(L".\\copyMe2\\file");

	EXPECT_FALSE(doesPathExists);

	// post test
	if (!doesPathExists) {
		fs::remove(_relativeSourcePath);
		fs::remove(_relativeDestinationPath);
	}
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
	// pre test
	bool isRelative = GetParam();

	// test
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

	bool doesDirectoryExists = Utils::isPathExists(absolutePath.get());

	EXPECT_FALSE(doesDirectoryExists);
}

TEST_P(RollbackFixture, rollbackCreateDirectoryNested) {
	// pre test
	bool isRelative = GetParam();

	// test
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
	// pre test
	bool isRelative = GetParam();

	LPCWSTR preTestPath = L".\\copyMe2";
	fs::create_directory(preTestPath);

	// test
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

	// post test
	if (areDirectoriesDeleted) {
		fs::remove(preTestPath);
	}
}

TEST_P(RollbackFixture, rollbackWhenAlreadyExisted) {
	//pre test
	bool isRelative = GetParam();

	LPCWSTR relativePath = L".\\copyMe2";
	fs::create_directory(relativePath);

	// test
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

	const bool doesDirectoryExists = Utils::isPathExists(relativePath);

	EXPECT_TRUE(doesDirectoryExists);

	// post test
	if (doesDirectoryExists) {
		fs::remove(relativePath);
	}
}

INSTANTIATE_TEST_SUITE_P(rollback, RollbackFixture, ::testing::Values(true, false));


TEST(E2E, runningActions) {
	LPCWSTR destinationPath = L".\\copyMe2\\copy3";

	std::vector<std::shared_ptr<Action>> actions = {
		std::make_shared<CreateDirectoryAction>(destinationPath)
	};

	auto installer = std::make_unique<Installer>(actions);
	installer->copy();

	const bool doesDirectoryExists = Utils::isPathExists(destinationPath);
	
	if (doesDirectoryExists) {
		actions.at(0)->rollback();
	}

	EXPECT_TRUE(doesDirectoryExists);
};

TEST(E2E, rollbackActions) {
	// pre test
	LPCWSTR sourcePath = L".\\test1";
	std::ofstream file(sourcePath);
	file.close();

	// test
	LPCWSTR destinationPath = L".\\copyMe2\\copy3";

	class RollbackMeAction : public Action {
		void act() { throw InstallerException(""); }
		void rollback() {};
	};

	std::vector<std::shared_ptr<Action>> actions = {
		std::make_shared<CreateDirectoryAction>(destinationPath),
		std::make_shared<CopyPathAction>(sourcePath, destinationPath),
		std::make_shared<RollbackMeAction>()
	};

	// We create a scope so the dtor of Installer will be called in the end
	{
		auto installer = std::make_unique<Installer>(actions);
		installer->copy();
	}

	const auto doesCopiedSourcePathExists = Utils::isPathExists(L".\\copyMe2\\copy3\\test1");
	const auto doesDestinationPathExists = Utils::isPathExists(destinationPath);

	EXPECT_FALSE(doesCopiedSourcePathExists);
	EXPECT_FALSE(doesDestinationPathExists);

	// post test
	fs::remove(sourcePath);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}