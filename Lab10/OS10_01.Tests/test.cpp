#include "pch.h"

#include <ostream>

#include "Api.h"
#include "HtHandle.h"

struct ElementState
{
	std::string key;
	std::string payload;

	friend std::ostream& operator<<(std::ostream& os, const ElementState& obj)
	{
		return os
			<< "key: " << obj.key
			<< " payload: " << obj.payload;
	}
};

struct CreateHtTest : testing::Test, testing::WithParamInterface<ElementState>
{
	static HT::HTHANDLE* htHandle;

	static void SetUpTestCase() {
		if (htHandle == nullptr)
		{
			htHandle = HT::Create(15, 10, 30, 30, "../input/test.ht");
		}
	}

	static void TearDownTestCase() {
		HT::Close(htHandle);
		htHandle = nullptr;
	}
};

HT::HTHANDLE* CreateHtTest::htHandle = nullptr;

TEST_P(CreateHtTest, CheckElementNotExist) {
	auto params = GetParam();
	HT::Element* element = HT::Get(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1));
	ASSERT_EQ(nullptr, element);
}

TEST_P(CreateHtTest, InsertElement) {
	auto params = GetParam();
	auto isInserted = HT::Insert(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1,
		params.payload.c_str(), params.payload.size() + 1));
	ASSERT_EQ(true, isInserted);
}

TEST_P(CreateHtTest, GetElement) {
	auto params = GetParam();
	HT::Element* element = HT::Get(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1));
	ASSERT_NE(nullptr, element);
}

TEST_P(CreateHtTest, UpdateElement) {
	auto params = GetParam();
	auto isUpdated = HT::Update(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1),
		params.payload.c_str(), params.payload.size() + 1);
	ASSERT_EQ(true, isUpdated);
}

TEST_F(CreateHtTest, SnapHt)
{
	auto isSnapSuccess = HT::Snap(htHandle);
	ASSERT_EQ(true, isSnapSuccess);
}

TEST_P(CreateHtTest, DeleteElement) {
	auto params = GetParam();
	auto isDeleted = HT::Delete(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1));
	ASSERT_EQ(true, isDeleted);
}

INSTANTIATE_TEST_CASE_P(Default, CreateHtTest, testing::Values(
	ElementState {"key0", "payload0"},
	ElementState{ "key1", "payload2" },
	ElementState{ "key2", "payload2" }
));

struct OpenHtTest : testing::Test, testing::WithParamInterface<ElementState>
{
	static HT::HTHANDLE* htHandle;

	static void SetUpTestCase() {
		if (htHandle == nullptr)
		{
			htHandle = HT::Open("../input/test.ht");
		}
	}

	static void TearDownTestCase() {
		HT::Close(htHandle);
		htHandle = nullptr;
	}
};

HT::HTHANDLE* OpenHtTest::htHandle = nullptr;

TEST_P(OpenHtTest, CheckElementNotExist) {
	auto params = GetParam();
	HT::Element* element = HT::Get(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1));
	ASSERT_EQ(nullptr, element);
}

TEST_P(OpenHtTest, InsertElement) {
	auto params = GetParam();
	auto isInserted = HT::Insert(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1,
		params.payload.c_str(), params.payload.size() + 1));
	ASSERT_EQ(true, isInserted);
}

TEST_P(OpenHtTest, CheckElementInserted) {
	auto params = GetParam();
	HT::Element* element = HT::Get(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1));
	ASSERT_NE(nullptr, element);
}

TEST_P(OpenHtTest, UpdateElement) {
	auto params = GetParam();
	params.payload += "-update";
	auto isUpdated = HT::Update(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1),
		params.payload.c_str(), params.payload.size() + 1);
	ASSERT_EQ(true, isUpdated);
}

TEST_P(OpenHtTest, CheckElementUpdated)
{
	auto params = GetParam();
	HT::Element* element = HT::Get(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1));
	params.payload += "-update";
	const char* payload = params.payload.c_str();
	for (int i = 0; i < params.payload.size(); ++i)
	{	
		ASSERT_EQ(payload[i], ((char*)element->payload)[i]);
	}
}

TEST_P(OpenHtTest, DeleteElement) {
	auto params = GetParam();
	auto isDeleted = HT::Delete(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1));
	ASSERT_EQ(true, isDeleted);
}

TEST_P(OpenHtTest, CheckElementDeleted) {
	auto params = GetParam();
	HT::Element* element = HT::Get(htHandle, new HT::Element(params.key.c_str(), params.key.size() + 1));
	ASSERT_EQ(nullptr, element);
}


INSTANTIATE_TEST_CASE_P(Default, OpenHtTest, testing::Values(
	ElementState{ "key0", "payload0" },
	ElementState{ "key1", "payload2" },
	ElementState{ "key2", "payload2" }
));

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}