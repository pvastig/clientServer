#pragma once

#include "util.h"

#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/system/error_code.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;
namespace sys  = boost::system;

class Connection : public boost::enable_shared_from_this<Connection> {
 public:
  using ptr = boost::shared_ptr<Connection>;
  Connection(asio::io_service& io_service);
  Connection();

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

  void read(sys::error_code const& error, size_t bytes_transferred);
  void write(sys::error_code const& error, size_t bytes_transferred);

 private:
  asio::io_service m_ioService;
  ip::tcp::socket m_socket{m_ioService};
  static const int maxLength = 1024;
  char m_data[maxLength]     = {};
  util::SocketRAII<ip::tcp::socket> m_raii{m_socket};
};
