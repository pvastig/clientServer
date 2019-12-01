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

  Connection(Connection const&) = delete;
  Connection& operator=(Connection) = delete;
  Connection(Connection&&)          = delete;
  Connection& operator=(Connection&&) = delete;

 private:
  asio::io_service m_ioService;
  ip::tcp::socket m_socket{m_ioService};

 private:
  class RAII {
   public:
    explicit RAII(ip::tcp::socket& socket) : m_socket(socket) {}
    ~RAII() {
      if (m_socket.is_open())
        m_socket.close();
    }

   private:
    ip::tcp::socket& m_socket;
  };
  RAII m_raii{m_socket};
};
