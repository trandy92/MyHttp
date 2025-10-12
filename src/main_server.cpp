#include <iostream>
#include <boost/asio.hpp>
#include <queue>

using boost::asio::ip::tcp;

struct HttpGetRequest
{
};

// using HttpRequest

using HttpGetRequests = std::queue<HttpGetRequest>;

class HttpServer
{
public:
  HttpServer() {}
  void serve()
  {
    // mTcpServer.subscribe
  }

private:
  TcpServer mTcpServer;
  HttpGetRequests mGetRequests;
};

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
  typedef std::shared_ptr<TcpConnection> pointer;

  static pointer create(boost::asio::io_context& io_context) { return pointer(new TcpConnection(io_context)); }

  tcp::socket& socket() { return mSocket; }

  void start()
  {
    auto self = shared_from_this();
    boost::asio::async_read_until(mSocket,
                                  mIncomingMessage,
                                  '\n',
                                  [self](const boost::system::error_code& ec, std::size_t bytesTransferred)
                                  {
                                    std::istream is(&self->mIncomingMessage);
                                    std::string line;
                                    std::getline(is, line);
                                    std::cout << line << std::endl;
                                  });

    mMessage = "Hello client!\n";

    boost::asio::async_write(mSocket,
                             boost::asio::buffer(mMessage),
                             [self](const boost::system::error_code& ec, std::size_t bytesTransferred)
                             { std::cout << "send message " << self->mMessage << std::endl; });
  }

private:
  TcpConnection(boost::asio::io_context& io_context) : mSocket(io_context) {}

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
    mIoContext.run();
  }
  void startAccept()
  {
    TcpConnection::pointer newConnection = TcpConnection::create(mIoContext);
    mConnection.push_back(newConnection);
    auto handleAccept = [this, newConnection](const boost::system::error_code& error)
    {
      if (!error)
      {
        newConnection->start();
      }
      startAccept();
    };
    mAcceptor.async_accept(newConnection->socket(), handleAccept);
  }

private:
  boost::asio::io_context mIoContext;
  tcp::acceptor mAcceptor;
  std::vector<TcpConnection::pointer> mConnection;
};

int main()
{
  try
  {
    HttpServer httpServer();

    std::this_thread::sleep_for(std::chrono::seconds(3));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
