#pragma once

#include "util.h"

#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;
namespace sys  = boost::system;

class Connection : public boost::enable_shared_from_this<Connection> {
 public:
  using ptr = boost::shared_ptr<Connection>;
  explicit Connection(asio::io_service& ioService);
  Connection()                  = default;
  Connection(Connection const&) = delete;
  Connection& operator=(Connection const&) = delete;
  Connection(Connection&&)                 = delete;
  Connection& operator=(Connection&&) = delete;
  ~Connection()                       = default;

  static ptr create(asio::io_service& ioService) {
    return boost::make_shared<Connection>(ioService);
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
  // TODO investigate new type std::byte?
  static const int m_maxLength = 1024;
  char m_data[m_maxLength]     = {};
  util::SocketRAII<ip::tcp::socket> m_raii{m_socket};
};
