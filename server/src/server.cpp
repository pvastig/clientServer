#include "server.h"
#include "connection.h"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;

Server::Server(std::string_view ip, unsigned short port)
    : m_implConn{std::make_unique<Connection>(ip.data(), port)},
      m_acceptor(m_implConn->ioService(),
                 ip::tcp::endpoint(ip::address::from_string(ip.data()), port)) {
  startAccept();
}

void Server::run() const {
  m_implConn->ioService().run();
}

void Server::accept(Connection::ptr connection,
                    boost::system::error_code const& error) {
  if (!error) {
    connection->start();
  }
  startAccept();
}

void Server::startAccept() {
  Connection::ptr connection = Connection::create(m_implConn->ioService());
  m_acceptor.async_accept(connection->socket(),
                          boost::bind(&Server::accept, this, connection,
                                      asio::placeholders::error));
}

Server::~Server() = default;
