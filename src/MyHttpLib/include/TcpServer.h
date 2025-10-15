#pragma once

#include <string>

#include "TcpConnection.h"

namespace MyHttp
{

  class TcpServer
  {
  public:
    TcpServer();

    void write(const std::string& msg);
    void StartAccept();
    void SubscribeToMessages(std::function<void(const std::string& msg)> onMessageHandler);

  private:
    std::future<void> mServingFuture;
    std::mutex mMutex;
    boost::asio::io_context mIoContext;
    tcp::acceptor mAcceptor;

    std::vector<TcpConnection::pointer> mConnections;
    std::vector<std::function<void(const std::string& msg)>> mOnMessageHandlers;
  };
} // namespace MyHttp
