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

    std::cout << "Connected to server.\n";
    io.run();

    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, "\n");
    std::istream is(&buffer);
    std::string line;
    std::getline(is, line);
    std::cout << "Received: " << line << "\n";

    std::string messageBack{"Hello Back"};

    boost::asio::write(socket, boost::asio::buffer(messageBack));

    // std::function<void()> receive =  [&](){
    // boost::asio::streambuf buffer;
    // boost::asio::read_until(socket, buffer, "\n");
    // , [&buffer, &socket](const boost::system::error_code& error, std::size_t bytesTransferred) {
    //   if(!error)
    //   {
    //     std::istream is(&buffer);
    //     std::string line;
    //     std::getline(is, line);
    //     std::cout << "Received: " << line << "\n";
    // std::string answer {"hey"};
    // boost::asio::async_write(socket, boost::asio::buffer(answer), [](const boost::system::error_code& error, std::size_t bytesTransferred){

    // });
      // };
    // });

    // receive();
    // };
    // receive();



  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
