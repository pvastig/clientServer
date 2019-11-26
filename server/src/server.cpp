#include <iostream>

#include <boost/asio.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;

std::string readData(ip::tcp::socket& socket) {
  asio::streambuf buf;
  asio::read_until(socket, buf, "\n");
  std::string data = asio::buffer_cast<char const*>(buf.data());
  return data;
}

void sendData(ip::tcp::socket& socket, std::string const& message) {
  std::string const msg = message + "\n";
  asio::write(socket, asio::buffer(message));
}

int main() {
  asio::io_service ioService;
  // listen for new connection
  ip::tcp::acceptor acceptor(ioService, ip::tcp::endpoint(ip::tcp::v4(), 1234));
  // socket creation
  ip::tcp::socket socket_(ioService);
  // waiting for connection
  acceptor.accept(socket_);
  // read operation
  auto const message = readData(socket_);
  std::cout << message << std::endl;
  // write operation
  sendData(socket_, "Hello From Server!");
  std::cout << "Server sent Hello message to Client!" << std::endl;
  return 0;
}
