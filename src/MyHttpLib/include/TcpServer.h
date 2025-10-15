#pragma once

#include <string>

#include "TcpConnection.h"

namespace MyHttp
{

  class TcpServer
  {
  public:
    TcpServer();
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
    std::vector<std::function<void(const std::string& msg)>> mOnMessageHandlers;
  };
} // namespace MyHttp
