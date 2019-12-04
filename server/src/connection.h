#pragma once

#include "../common/util.h"

#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;

class Connection : public boost::enable_shared_from_this<Connection> {
 public:
  using ptr = boost::shared_ptr<Connection>;
  Connection(asio::io_service& io_service);
  Connection(std::string const& ip, unsigned short port);

  static ptr create(asio::io_service& io_service) {
    return ptr(new Connection(io_service));
  }

  asio::io_service& ioService() {
    return m_ioService;
  }

  ip::tcp::socket& socket() {
    return m_socket;
  }

  void start();
  void stop();

  void read(boost::system::error_code const& error, size_t bytes_transferred);
  void write(boost::system::error_code const& error, size_t bytes_transferred);

 private:
  asio::io_service m_ioService;
  ip::tcp::socket m_socket{m_ioService};
  static const int maxLength = 1024;
  char m_data[maxLength];
  RAII<ip::tcp::socket> m_raii{m_socket};
};
