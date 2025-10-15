#include <iostream>
#include <boost/asio.hpp>
#include <queue>
#include <mutex>

#include "MyHttpLib/include/Utils.h"
#include "TcpConnection.h"

namespace MyHttp
{
  using boost::asio::ip::tcp;

  class TcpServer
  {
  public:
    TcpServer() : mAcceptor(mIoContext, tcp::endpoint(tcp::v4(), 5555))
    {
      startAccept();

      mServingFuture = runReallyAsync([this]() { mIoContext.run(); });
    }

    void write(const std::string& msg)
    {
      for (auto conn : mConnections)
      {
        conn->writeMessage(msg);
      }
    }

    void startAccept()
    {
      TcpConnection::pointer newConnection = TcpConnection::create(mIoContext);
      mConnections.push_back(newConnection);
      auto handleAccept = [this, newConnection](const boost::system::error_code& error)
      {
        if (!error)
        {
          newConnection->listenForIncomingMessages(
              [&](const std::string& incomingMessage)
              {
                for (auto handler : mOnMessageHandlers)
                {
                  handler(incomingMessage);
                }
              });
        }
        startAccept();
      };
      mAcceptor.async_accept(newConnection->socket(), handleAccept);
    }

    void subscribeToMessages(std::function<void(const std::string& msg)> onMessageHandler)
    {
      mOnMessageHandlers.push_back(onMessageHandler);
    }

  private:
    std::future<void> mServingFuture;
    std::mutex mMutex;
    boost::asio::io_context mIoContext;
    tcp::acceptor mAcceptor;
    std::vector<TcpConnection::pointer> mConnections;
    std::queue<std::string> mIncomingMessages;
    std::vector<std::function<void(const std::string& msg)>> mOnMessageHandlers;
  };

  // using HttpRequest

  class HttpServer
  {
  public:
    HttpServer()
    {
      mTcpServer.subscribeToMessages([](const std::string& msg) { std::cout << "[Incoming] " << msg << std::endl; });
    }
    void write(const std::string& msg) { mTcpServer.write(msg); }

  private:
    TcpServer mTcpServer;
  };
} // namespace MyHttp

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
      httpServer.write(line + "\r\n\r\n");
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
