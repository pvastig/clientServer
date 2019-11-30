#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <boost/asio.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;

class Connection;

class Server {
 public:
  Server(Server const &) = delete;
  Server &operator=(Server) = delete;
  Server(Server &&)         = delete;
  Server &operator=(Server &&) = delete;

  Server(std::string_view ip, unsigned short port);
  void run();

 private:
  ip::tcp::endpoint m_endPoint;
  asio::io_service m_ioService;

 private:
  std::unique_ptr<Connection> m_Impl;
};
