#include "connection.h"

#include "log.h"

#include <cassert>

namespace sys = boost::system;

Connection::Connection(std::string const& ip, unsigned short port) {
  sys::error_code error;
  m_socket.connect(ip::tcp::endpoint(ip::address::from_string(ip), port),
                   error);
  if (error)
    throw std::runtime_error(error.message());
}

void Connection::sendMsg(std::string const& s) {
  assert(m_socket.is_open());
  asio::streambuf buf;
  std::ostream out(&buf);
  out << s;
  sys::error_code error;
  write(m_socket, buf, error);
  if (error)
    throw std::runtime_error(error.message());
}

std::string Connection::readMsg() {
  assert(m_socket.is_open());
  asio::streambuf buf;
  sys::error_code error;
  asio::read(m_socket, buf, asio::transfer_all(), error);
  if (error && error != asio::error::eof)
    throw std::runtime_error(error.message());
  std::ostringstream out;
  out << &buf;
  return out.str();
}
