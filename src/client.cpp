#include <iostream>
#include <boost/asio.hpp>

#include "TcpConnection.h"

using namespace MyHttp;

int main()
{
  try
  {
    boost::asio::io_context io;
    auto tcpConnection = TcpConnection::create(io);
    tcpConnection->connect();
    tcpConnection->listenForIncomingMessages([](const std::string& msg)
                                             { std::cout << "[Incoming] " << msg << std::endl; });
    auto fut = std::async(std::launch::async, [&]() { io.run(); });

    std::cout << "Start chatting" << std::endl;
    while (true)
    {
      std::string msg;
      std::getline(std::cin, msg);

      std::cout << "[Outgoing]" << msg << std::endl;
      tcpConnection->writeMessage(msg + "\r\n\r\n");
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
