
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

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
                                    // std::cout << line << std::endl;
                                    self->mIncomingMessage.consume(bytesTransferred);
                                    msgHandler(line);
                                    self->listenForIncomingMessages(msgHandler);
                                  });
  }

  void writeMessage(const std::string& message)
  {
    auto self = shared_from_this();
    std::size_t n = message.size();
    std::ostream os(&mOutBuffer);
    os.write(message.data(), n);
    boost::asio::async_write(mSocket,
                             mOutBuffer.data(),
                             [self](const boost::system::error_code& ec, std::size_t bytesTransferred)
                             {
                               if (!ec)
                               {
                                 self->mOutBuffer.consume(bytesTransferred);
                               }
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
  TcpConnection(boost::asio::io_context& ioContext) : mSocket(ioContext) {}

  boost::asio::streambuf mOutBuffer;
  tcp::socket mSocket;
  boost::asio::streambuf mIncomingMessage;
};
