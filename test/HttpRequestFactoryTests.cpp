#include <gtest/gtest.h>
#include "HttpRequestFactory.h"
#include "Utils.h"

constexpr auto exampleGetRequest = R"(GET / HTTP/1.1
Host: localhost:5555
Connection: keep-alive
sec-ch-ua: "Google Chrome";v="141", "Not?A_Brand";v="8", "Chromium";v="141"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Windows"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/141.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Sec-Fetch-Site: none
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: de-DE,de;q=0.9,en-US;q=0.8,en;q=0.7
Cookie: com.wibu.cm.webadmin.lang=de-DE
)";

using namespace MyHttp;

TEST(HttpRequestFactory, DoesSomethingCorrect)
{
  auto request = ParseRequest(exampleGetRequest);
  GenericVisit(
      request,
      [](const HttpRequest& res)
      {
        EXPECT_EQ(res.method, Method::Get);
        EXPECT_EQ(res.resource, "/");
        EXPECT_EQ(res.version, ProtocolVersion::Version_1_1);
      },
      [](const HttpRequestParseError& res) { ASSERT_TRUE(false); });
}
