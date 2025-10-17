#include <gtest/gtest.h>
#include "HttpResponse.h"
#include "HttpTypes.h"

using namespace MyHttp;

TEST(HttpResponseTests, EmptyResponse)
{
  auto response = HttpResponse::Builder{}.Build();
  ASSERT_EQ(response.toString(), "") << "An empty response should result in an empty string";
}

TEST(HttpResponseTests, OnlyHeader)
{
  const std::string expectedResponse = R"(Http/1.1 200 OK
Server: MyHttp
Content-Length: 5
Content-Type: text/html

hallo)";
  auto response = HttpResponse::Builder{}
                      .Version(ProtocolVersion::Version_1_1)
                      .Status(StatusCode::c_200)
                      .ReasonString("OK")
                      .Server()
                      .Type(ContentType::text_html)
                      .Content("hallo")
                      .ContentLength(5)
                      .Build();

  std::cout << response.toString();
  ASSERT_EQ(response.toString(), expectedResponse);
}
