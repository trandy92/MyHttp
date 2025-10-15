#include <boost/asio.hpp>
#include "TcpServer.h"
#include "Utils.h"

namespace MyHttp
{
  using boost::asio::ip::tcp;
  TcpServer::TcpServer() : mAcceptor(mIoContext, tcp::endpoint(tcp::v4(), 5555))
  {
    StartAccept();

    mServingFuture = RunReallyAsync([this]() { mIoContext.run(); });
  }
} // namespace MyHttp
