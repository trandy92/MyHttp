#include <gtest/gtest.h>
#include "HttpRequestFactory.h"

TEST(HttpRequestFactory, DoesSomethingCorrect)
{
  auto request = ParseRequest("Get myResource 1.1");
  EXPECT_EQ(request.method, Method::Get);
  EXPECT_EQ(request.resource, "myResource");
  EXPECT_EQ(request.version, ProtocolVersion::Version_1_1);
}
