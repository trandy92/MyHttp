#include <gtest/gtest.h>
#include "ResourceManager.h"
#include "Utils.h"
#include <gmock/gmock.h>

using namespace MyHttp;

class MockFilesystem : public MyHttp::MyHttpFilesystem
{
public:
  MOCK_METHOD(bool, Exists, (std::string_view), (const, override));
  MOCK_METHOD(std::string, GetContent, (std::string_view), (const, override));
};

TEST(ResourceManager, LoadResource_ConfiguredRootDoesNotExist)
{
  MockFilesystem fs;
  EXPECT_CALL(fs, Exists(std::string_view("."))).Times(1).WillOnce(testing::Return(false));

  ResourceManager rm(fs, ResourceConfig{.resourcesDir = "."});
  auto result = rm.LoadResource("/");
  GenericVisit(
      result,
      [](Resource r) { ASSERT_TRUE(false); },
      [](ResourceLoadError err) { EXPECT_EQ(err.errorMessage, "Resources dir not found."); });
}

TEST(ResourceManager, LoadResource_Success)
{
  MockFilesystem fs;
  EXPECT_CALL(fs, Exists(std::string_view("."))).WillOnce(testing::Return(true));
  EXPECT_CALL(fs, Exists(std::string_view("./index.html"))).WillOnce(testing::Return(true));
  EXPECT_CALL(fs, GetContent(std::string_view("./index.html"))).WillOnce(testing::Return("Some Content"));

  ResourceManager rm(fs, ResourceConfig{.resourcesDir = "."});
  auto result = rm.LoadResource("/");
  GenericVisit(
      result,
      [](Resource r)
      {
        EXPECT_EQ(r.content, "Some Content");
        EXPECT_EQ(r.path, "./index.html");
      },
      [](ResourceLoadError err)
      {
        EXPECT_EQ(err.reason, LoadErrorReason::ConfiguredRootDoesNotExist);
        EXPECT_EQ(err.errorMessage, "Resources dir not found.");
      });
}

TEST(ResourceManager, LoadResource_ResourceNotFound)
{
  MockFilesystem fs;
  EXPECT_CALL(fs, Exists(std::string_view("."))).WillOnce(testing::Return(true));
  EXPECT_CALL(fs, Exists(std::string_view("./index.html"))).WillOnce(testing::Return(false));

  ResourceManager rm(fs, ResourceConfig{.resourcesDir = "."});
  auto result = rm.LoadResource("/");
  GenericVisit(
      result,
      [](Resource r) { ASSERT_TRUE(false) << "If resource is not found, no resource can be loaded."; },
      [](ResourceLoadError err)
      {
        EXPECT_EQ(err.reason, LoadErrorReason::ResourceNotFound);
        EXPECT_EQ(err.errorMessage, "index.html not found.");
      });
}
