
#pragma once

#include <boost/asio.hpp>

namespace MyHttp
{
  using boost::asio::ip::tcp;

  class TcpConnection : public std::enable_shared_from_this<TcpConnection>
  {
  public:
    typedef std::shared_ptr<TcpConnection> pointer;

    static pointer create(boost::asio::io_context& ioContext) { return pointer(new TcpConnection(ioContext)); }

    tcp::socket& socket() { return mSocket; }

    void listenForIncomingMessages(std::function<void(const std::string&)> msgHandler);

    void writeMessage(const std::string& message);

    void connect();

  private:
    TcpConnection(boost::asio::io_context& ioContext) : mSocket(ioContext) {}

    boost::asio::streambuf mOutBuffer;
    tcp::socket mSocket;
    boost::asio::streambuf mIncomingMessage;
  };
} // namespace MyHttp
