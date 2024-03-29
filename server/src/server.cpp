#include "server.h"
#include "connection.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;

Server::Server(std::string_view ip, unsigned short port)
    : m_acceptor(m_conn.ioService(),
                 ip::tcp::endpoint(ip::address::from_string(ip.data()), port)) {
  startAccept();
}

void Server::run() {
  m_conn.ioService().run();
}

void Server::accept(Connection::ptr const& connection,
                    boost::system::error_code const& error) {
  if (!error) {
    connection->start();
  }
  startAccept();
}

void Server::startAccept() {
  Connection::ptr connection = Connection::create(m_conn.ioService());
  m_acceptor.async_accept(connection->socket(),
                          boost::bind(&Server::accept, this, connection,
                                      asio::placeholders::error));
}

Server::~Server() = default;
