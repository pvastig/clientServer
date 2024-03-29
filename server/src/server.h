#pragma once

#include <memory>
#include <string_view>

#include "connection.h"

class Connection;

class Server {
 public:
  Server(Server const &) = delete;
  Server &operator=(Server) = delete;
  Server(Server &&)         = delete;
  Server &operator=(Server &&) = delete;

  Server(std::string_view ip, unsigned short port);
  void run();
  ~Server();

 private:
  Connection m_conn;
  ip::tcp::acceptor m_acceptor;

 private:
  void startAccept();
  void accept(Connection::ptr const &connection,
              boost::system::error_code const &error);
};
