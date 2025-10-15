#include <iostream>
#include <boost/asio.hpp>
#include <queue>
#include <mutex>
#include <stdexcept>

#include "HttpRequestFactory.h"
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
      StartAccept();

      mServingFuture = RunReallyAsync([this]() { mIoContext.run(); });
    }

    void write(const std::string& msg)
    {
      for (auto conn : mConnections)
      {
        conn->writeMessage(msg);
      }
    }

    void StartAccept()
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
        StartAccept();
      };
      mAcceptor.async_accept(newConnection->socket(), handleAccept);
    }

    void SubscribeToMessages(std::function<void(const std::string& msg)> onMessageHandler)
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
      mTcpServer.SubscribeToMessages([](const std::string& msg) { std::cout << "[Incoming] " << msg << std::endl; });
      mTcpServer.SubscribeToMessages(
          [](const std::string& msg)
          {
            auto request = ParseRequest(msg);
            GenericVisit(
                request,
                [](HttpRequest request)
                {
                  static_assert(static_cast<int>(ProtocolVersion::kNoEntries) == 1);
                  switch (request.version)
                  {
                  case ProtocolVersion::Version_1_1:
                    break;
                  default:
                    throw std::invalid_argument("Http Protocol Version is not yet supported.");
                    break;
                  }

                  static_assert(static_cast<int>(Method::kNoEntries) == 4);
                  switch (request.method)
                  {
                  case Method::Get:
                  {
                    // request.resource
                  }
                  break;
                  case Method::Post:
                    break;
                  case Method::Put:
                    break;
                  case Method::Update:
                    break;
                  case Method::kNoEntries:
                    throw std::invalid_argument("This should not happen.");
                  }
                },
                [](HttpRequestParseError err) { std::cerr << "Failed to parse Http Request" << std::endl; });
          });
    }
    void Write(const std::string& msg) { mTcpServer.write(msg); }

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
      httpServer.Write(line + "\r\n\r\n");
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
