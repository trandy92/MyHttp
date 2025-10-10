#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main() {
  try {
    //std::this_thread::sleep_for(std::chrono::seconds(3));
    boost::asio::io_context io;

    // 1. Resolve server address and port
    tcp::resolver resolver(io);
    auto endpoints = resolver.resolve("127.0.0.1", "5555");

    // 2. Connect
    tcp::socket socket(io);
    boost::asio::connect(socket, endpoints);
 
    boost::asio::streambuf buffer;
    boost::asio::async_read_until(socket, buffer, "\n", [&buffer](const boost::system::error_code& error, std::size_t bytesTransferred) {
      if(!error)
      {
        std::istream is(&buffer);
        std::string line;
        std::getline(is, line);
        std::cout << "Received: " << line << "\n";

      }
    });
    std::cout << "Connected to server.\n";
    io.run();

  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
