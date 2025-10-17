#include <gtest/gtest.h>
#include "HttpResponse.h"
#include "HttpTypes.h"

using namespace MyHttp;

TEST(HttpResponseTests, EmptyContent)
{
  const std::string expectedResponse = R"(Http/1.1 200 OK
Content-Type: text/html
Content-Length: 0)";
  auto response =
      HttpResponse::Builder(ProtocolVersion::Version_1_1, StatusCode::c_200, "OK", ContentType::text_html).Build();
  ASSERT_EQ(response.toString(), expectedResponse);
}

TEST(HttpResponseTests, OnlyHeader)
{
  const std::string expectedResponse = R"(Http/1.1 200 OK
Content-Type: text/html
Server: MyHttp
Content-Length: 5

hallo)";
  auto response = HttpResponse::Builder(ProtocolVersion::Version_1_1, StatusCode::c_200, "OK", ContentType::text_html)
                      .Server()
                      .Content("hallo")
                      .Build();

  std::cout << response.toString();
  ASSERT_EQ(response.toString(), expectedResponse);
}
