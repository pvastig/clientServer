#include <iostream>

#include <boost/asio.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;

int main() {
  asio::io_service ioService;
  // socket creation
  ip::tcp::socket socket(ioService);
  // connection
  socket.connect(
      ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 1234));
  // request/message from client
  std::string const msg = "Hello from Client!\n";
  boost::system::error_code error;
  write(socket, asio::buffer(msg), error);
  if (!error) {
    std::cout << "Client sent hello message!" << std::endl;
  } else {
    std::cout << "send failed: " << error.message() << std::endl;
  }
  // getting response from server
  asio::streambuf receiveBuffer;
  read(socket, receiveBuffer, asio::transfer_all(), error);
  if (error && error != asio::error::eof) {
    std::cout << "receive failed: " << error.message() << std::endl;
  } else {
    char const* data = asio::buffer_cast<const char*>(receiveBuffer.data());
    std::cout << data << std::endl;
  }
  return 0;
}
