#include <iostream>
#include <fstream>
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

    mMessage = "Hello server!\n";
    boost::asio::async_write(mSocket, boost::asio::buffer(mMessage),
      [this](const boost::system::error_code& ec,
        std::size_t bytesTransferred) {
        auto x = 5;
        std:: cout << "send message " << mMessage << std::endl;
    });


  }
  void connect()
  {
    tcp::resolver resolver(mSocket.get_executor());
    auto endpoints = resolver.resolve("127.0.0.1", "5555");
    boost::asio::connect(mSocket, endpoints);
    std::cout << "Connected to server.\n";

  }
private:
  TcpConnection(boost::asio::io_context& io_context) : mSocket(io_context)
  {
  }

  tcp::socket mSocket;
  std::string mMessage;
  boost::asio::streambuf mIncomingMessage;
};

int main() {
  try {
    boost::asio::io_context io;
    auto tcpConnection = TcpConnection::create(io);
    tcpConnection->connect();
    tcpConnection->start();
    io.run();
    std::this_thread::sleep_for(std::chrono::seconds(3));

  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
