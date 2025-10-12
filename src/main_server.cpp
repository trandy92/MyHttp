#include <iostream>
#include <boost/asio.hpp>
#include <queue>
#include <mutex>

using boost::asio::ip::tcp;

struct HttpGetRequest
{
};

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
  typedef std::shared_ptr<TcpConnection> pointer;

  static pointer create(boost::asio::io_context& ioContext) { return pointer(new TcpConnection(ioContext)); }

  tcp::socket& socket() { return mSocket; }

  void listenForIncomingMessages(std::function<void(const std::string&)> msgHandler)
  {
    auto self = shared_from_this();
    boost::asio::async_read_until(mSocket,
                                  mIncomingMessage,
                                  '\n',
                                  [self, msgHandler](const boost::system::error_code& ec, std::size_t bytesTransferred)
                                  {
                                    std::istream is(&self->mIncomingMessage);
                                    std::string line;
                                    std::getline(is, line);
                                    std::cout << line << std::endl;
                                    msgHandler(line);
                                  });

    // mMessage = "Hello client!\n";

    // boost::asio::async_write(mSocket,
    //                          boost::asio::buffer(mMessage),
    //                          [self](const boost::system::error_code& ec, std::size_t bytesTransferred)
    //                          { std::cout << "send message " << self->mMessage << std::endl; });
  }

private:
  TcpConnection(boost::asio::io_context& ioContext) : mSocket(ioContext) {}

  tcp::socket mSocket;
  std::string mMessage;
  boost::asio::streambuf mIncomingMessage;
};

class TcpServer
{
public:
  TcpServer() : mAcceptor(mIoContext, tcp::endpoint(tcp::v4(), 5555))
  {
    startAccept();
    mServingThread = std::thread([this]() { mIoContext.run(); });
  }
  void startAccept()
  {
    TcpConnection::pointer newConnection = TcpConnection::create(mIoContext);
    mConnection.push_back(newConnection);
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
  std::vector<TcpConnection::pointer> mConnection;
  std::queue<std::string> mIncomingMessages;
};

// using HttpRequest

using HttpGetRequests = std::queue<HttpGetRequest>;

class HttpServer
{
public:
  HttpServer() {}
  void serve()
  {
    while (true)
    {
      auto msgs = mTcpServer.getLastMessages();
      for (auto msg : msgs)
      {
        std::cout << "[Incoming Message]" << msg << std::endl;
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
    httpServer.serve();
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
