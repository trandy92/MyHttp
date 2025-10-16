#include <gtest/gtest.h>
#include "ResourceManager.h"
#include "Utils.h"
#include <gmock/gmock.h>

using namespace MyHttp;

class MockFilesystem : public MyHttp::MyHttpFilesystem
{
public:
  MOCK_METHOD(bool, Exists, (std::string), (const, override));
};

TEST(ResourceManager, LoadResource_Success)
{
  MockFilesystem fs;
  EXPECT_CALL(fs, Exists("/")).Times(1).WillOnce(testing::Return(false));

  ResourceManager rm(fs, ResourceConfig{.resourcesDir = "/"});
  auto result = rm.LoadResource("/");
  GenericVisit(
      result,
      [](Resource r) { ASSERT_TRUE(false); },
      [](ResourceLoadError err) { EXPECT_EQ(err.errorMessage, "Resources dir not found."); });
}
