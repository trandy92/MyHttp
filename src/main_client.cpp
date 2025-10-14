#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

#include "TcpConnection.h"

using boost::asio::ip::tcp;

int main()
{
  try
  {
    boost::asio::io_context io;
    auto tcpConnection = TcpConnection::create(io);
    tcpConnection->connect();
    tcpConnection->listenForIncomingMessages([](const std::string& msg)
                                             { std::cout << "[Received] " << msg << std::endl; });
    // io.run();
    std::jthread ioThread([&]() { io.run(); });

    while (true)
    {
      std::cout << "Enter a message" << std::endl;
      std::string msg;
      std::getline(std::cin, msg);
      tcpConnection->writeMessage(msg + '\n');
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
