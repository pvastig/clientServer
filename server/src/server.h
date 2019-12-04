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
  void run() const;
  ~Server();

 private:
  std::unique_ptr<Connection> m_implConn;
  ip::tcp::acceptor m_acceptor;

 private:
  void startAccept();
  void accept(Connection::ptr connection,
              boost::system::error_code const &error);
};
