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
    mMessage = "Hello client!\n";
    boost::asio::async_write(mSocket, boost::asio::buffer(mMessage),
      [this](const boost::system::error_code& ec,
        std::size_t bytesTransferred) {
        auto x = 5;
    });
  }
private:
  TcpConnection(boost::asio::io_context& io_context) : mSocket(io_context)
  {

  }

  tcp::socket mSocket;
  std::string mMessage;
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

    //// 1. Create an acceptor that listens on port 5555
    //tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 5555));

    //std::cout << "Server listening on port 5555...\n";

    //// 2. Wait for a client to connect
    //tcp::socket socket(io);
    //acceptor.accept(socket);

    //std::cout << "Client connected!\n";

    //// 3. Receive data
    //for (;;) {
    //  char data[1024];
    //  boost::system::error_code error;

    //  size_t length = socket.read_some(boost::asio::buffer(data), error);

    //  if (error == boost::asio::error::eof) {
    //    std::cout << "Connection closed by client.\n";
    //    break;
    //  }
    //  else if (error) {
    //    throw boost::system::system_error(error);
    //  }

    //  std::cout << "Received: " << std::string(data, length) << "\n";
    //}
  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}