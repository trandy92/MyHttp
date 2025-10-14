#include <iostream>
#include <boost/asio.hpp>
#include <queue>
#include <mutex>

#include "TcpConnection.h"

using boost::asio::ip::tcp;

struct HttpGetRequest
{
};

class TcpServer
{
public:
  TcpServer() : mAcceptor(mIoContext, tcp::endpoint(tcp::v4(), 5555))
  {
    startAccept();
    mServingThread = std::thread([this]() { mIoContext.run(); });
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
              std::lock_guard lock(mMutex);
              mIncomingMessages.emplace(incomingMessage);
            });
      }
      startAccept();
    };
    mAcceptor.async_accept(newConnection->socket(), handleAccept);
  }

  std::vector<std::string> getLastMessages()
  {
    std::vector<std::string> lastMessages;
    std::lock_guard lock(mMutex);
    lastMessages.reserve(mIncomingMessages.size());
    while (!mIncomingMessages.empty())
    {
      lastMessages.emplace_back(std::move(mIncomingMessages.front()));
      mIncomingMessages.pop();
    }
    return lastMessages;
  }

private:
  std::thread mServingThread;
  std::mutex mMutex;
  boost::asio::io_context mIoContext;
  tcp::acceptor mAcceptor;
  std::vector<TcpConnection::pointer> mConnections;
  std::queue<std::string> mIncomingMessages;
};

// using HttpRequest

using HttpGetRequests = std::queue<HttpGetRequest>;

class HttpServer
{
public:
  HttpServer() {}
  void write(const std::string& msg) { mTcpServer.write(msg); }
  void listenForIncomingMessage()
  {
    while (true)
    {
      auto msgs = mTcpServer.getLastMessages();
      for (auto msg : msgs)
      {
        std::cout << "[Incoming] " << msg << std::endl;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  }

private:
  TcpServer mTcpServer;
  HttpGetRequests mGetRequests;
};

int main()
{
  try
  {
    HttpServer httpServer;
    std::jthread httpThread([&]() { httpServer.listenForIncomingMessage(); });
    std::cout << "Start chatting: \n";
    while (true)
    {
      std::string line;
      std::getline(std::cin, line);
      std::cout << "[Outgoing] " << line << std::endl;
      httpServer.write(line + '\n');
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
