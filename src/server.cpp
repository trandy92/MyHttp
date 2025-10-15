#include <iostream>
#include "HttpServer.h"

using namespace MyHttp;

int main()
{
  try
  {
    HttpServer httpServer;
    std::cout << "Start chatting: \n";
    while (true)
    {
      std::string line;
      std::getline(std::cin, line);
      std::cout << "[Outgoing] " << line << std::endl;
      httpServer.Write(line + "\r\n\r\n");
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
