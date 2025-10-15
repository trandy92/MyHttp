#include "TcpConnection.h"
#include <iostream>

void TcpConnection::listenForIncomingMessages(std::function<void(const std::string&)> msgHandler)
{
  auto self = shared_from_this();
  boost::asio::async_read_until(mSocket,
                                mIncomingMessage,
                                "\r\n\r\n",
                                [self, msgHandler](const boost::system::error_code& ec, std::size_t bytesTransferred)
                                {
                                  if (!ec && self->mSocket.is_open())
                                  {
                                    std::istream is(&self->mIncomingMessage);
                                    std::string header("\0", bytesTransferred);
                                    is.read(&header[0], bytesTransferred);
                                    self->mIncomingMessage.consume(bytesTransferred);
                                    msgHandler(header);
                                    self->listenForIncomingMessages(msgHandler);
                                  }
                                });
}
void TcpConnection::writeMessage(const std::string& message)
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
void TcpConnection::connect()
{
  tcp::resolver resolver(mSocket.get_executor());
  auto endpoints = resolver.resolve("127.0.0.1", "5555");
  boost::asio::connect(mSocket, endpoints);
  std::cout << "Connected to server.\n";
}
