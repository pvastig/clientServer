#pragma once

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

 private:
  asio::io_service m_ioService;
  ip::tcp::socket m_socket{m_ioService};

 private:
  struct RAII {
    explicit RAII(ip::tcp::socket& socket) : m_socket(socket) {}
    ~RAII() {
      if (m_socket.is_open())
        m_socket.close();
    }

    ip::tcp::socket& m_socket;
  };
  RAII m_raii{m_socket};
};
