#include <boost/asio.hpp>
#include "TcpServer.h"
#include "Utils.h"

namespace MyHttp
{
  using boost::asio::ip::tcp;
  TcpServer::TcpServer() : mAcceptor(mIoContext, tcp::endpoint(tcp::v4(), 5555))
  {
    StartAccept();

    mServingFuture = RunReallyAsync([this]() { mIoContext.run(); });
  }

  void TcpServer::Write(const std::string& msg)
  {
    for (auto conn : mConnections)
    {
      conn->writeMessage(msg);
    }
  }
  void TcpServer::StartAccept()
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

  void TcpServer::SubscribeToMessages(std::function<void(const std::string& msg)> onMessageHandler)
  {
    mOnMessageHandlers.push_back(onMessageHandler);
  }
} // namespace MyHttp
