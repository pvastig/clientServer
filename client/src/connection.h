#pragma once

#include "util.h"

#include <string>

#include <boost/asio.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;

class Connection {
 public:
  Connection(std::string const& ip, unsigned short port);
  ~Connection();
  void sendMsg(std::string const& s);
  std::string readMsg();

  Connection(Connection const&) = delete;
  Connection& operator=(Connection) = delete;
  Connection(Connection&&)          = delete;
  Connection& operator=(Connection&&) = delete;

 private:
  asio::io_service m_ioService;
  ip::tcp::socket m_socket{m_ioService};
  util::RAII<ip::tcp::socket> m_raii{m_socket};
};
