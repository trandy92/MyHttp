#include <iostream>
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
    tcpConnection->listenForIncomingMessages(
        [](const std::string& msg)
        {
          auto x = msg;
          std::cout << "[Incoming] " << msg << std::endl;
        });
    std::jthread ioThread([&]() { io.run(); });

    std::cout << "Start chatting" << std::endl;
    while (true)
    {
      std::string msg;
      std::getline(std::cin, msg);

      std::cout << "[Outgoing]" << msg << std::endl;
      tcpConnection->writeMessage(msg + '\n');
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
