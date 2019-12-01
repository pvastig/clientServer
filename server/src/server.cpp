#include "server.h"
#include "connection.h"

#include <iostream>

#include <boost/asio.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;

/*class Server {
 public:
  // constructor for accepting connection from client
  Server(asio::io_service& io_service)
      : m_ioService(io_service),
        m_acceptor(
            io_service,
            ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 1271)) {
    startAccept();
  }
  void handleAccept(Connection::ptr connection,
                    const boost::system::error_code& err) {
    if (!err) {
      connection->start();
    }
    startAccept();
  }

 private:
  boost::asio::io_service& m_ioService;
  ip::tcp::acceptor m_acceptor;
  void startAccept() {
    Connection::ptr connection = Connection::create(m_ioService);

    // asynchronous accept operation and wait for a new connection.
    m_acceptor.async_accept(connection->socket(),
                            boost::bind(&Server::handleAccept, this, connection,
                                        asio::placeholders::error));
  }
};

int main(int argc, char* argv[]) {
  try {
     boost::asio::io_service io_service;
     Server server(io_service);
     io_service.run();
} catch (std::exception& e) {
  // std::cerr << e.what() << std::endl;
}
return 0;
}
*/

Server::Server(std::string_view ip, unsigned short port)
    : m_implConn{std::make_unique<Connection>(ip.data(), port)} {}

void Server::run() const {}

Server::~Server() = default;
