#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
  typedef std::shared_ptr<TcpConnection> pointer;

  static pointer create(boost::asio::io_context& io_context)
  {
    return pointer(new TcpConnection(io_context));
  }

  tcp::socket& socket()
  {
    return mSocket;
  }

  void start()
  {

    auto self = shared_from_this();
    boost::asio::async_read_until(mSocket, mIncomingMessage, '\n', [self](const boost::system::error_code& ec,
        std::size_t bytesTransferred) {
          std::istream is(&self->mIncomingMessage);
          std::string line;
          std::getline(is, line);
          std::cout << line << std::endl;
    });


    mMessage = "Hello client!\n";

    boost::asio::async_write(mSocket, boost::asio::buffer(mMessage),
      [self](const boost::system::error_code& ec,
        std::size_t bytesTransferred) {
        std:: cout << "send message " << self->mMessage << std::endl;
    });


  }
private:
  TcpConnection(boost::asio::io_context& io_context) : mSocket(io_context)
  {

  }

  tcp::socket mSocket;
  std::string mMessage;
  boost::asio::streambuf mIncomingMessage;
};

class TcpServer
{
public:
  TcpServer(boost::asio::io_context& io_context) : mIoContext(io_context),
    mAcceptor(io_context, tcp::endpoint(tcp::v4(), 5555)) {
    startAccept();
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
  boost::asio::io_context& mIoContext;
  tcp::acceptor mAcceptor;
  std::vector<TcpConnection::pointer> mConnection;
};



int main() {
  try {
    boost::asio::io_context io;
    TcpServer server(io);
    io.run();
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}