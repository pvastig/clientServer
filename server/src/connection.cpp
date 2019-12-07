#include "connection.h"
#include "message_parser.h"

#include "log.h"
#include "util.h"

#include <iostream>

#include <boost/bind.hpp>

// namespace system = boost::system;

Connection::Connection(asio::io_service& ioService) : m_socket(ioService) {}

void Connection::start() {
  // TODO thinking about a big message
  m_socket.async_read_some(asio::buffer(m_data, m_maxLength),
                           boost::bind(&Connection::read, shared_from_this(),
                                       asio::placeholders::error,
                                       asio::placeholders::bytes_transferred));
}

void Connection::stop() {
  if (m_socket.is_open())
    m_socket.close();
}

void Connection::read(sys::error_code const& error, size_t bytes_transferred) {
  if (error)
    throw std::runtime_error(error.message());

  INFO << "Server: read bytes: " << bytes_transferred;

  MessageParser mp(m_data);
  mp.parse();

  // first we write hach, secod - count of line
  std::string const message = std::to_string(util::hashStr(mp.message())) +
                              "," + std::to_string(mp.countLine());

  if (auto const row = findMax(mp.rows())) {
    auto const& value = row.value();
    if (value.price2 == 0.0)
      throw std::logic_error("dividing by zero, check columns");
    INFO << "Server: max: " << value.date << " " << std::setprecision(2)
         << std::fixed << (value.price1 / value.price2);
  }
  m_socket.async_write_some(asio::buffer(message, message.size()),
                            boost::bind(&Connection::write, shared_from_this(),
                                        asio::placeholders::error,
                                        asio::placeholders::bytes_transferred));
}
void Connection::write(sys::error_code const& error, size_t bytes_transferred) {
  if (error) {
    throw std::runtime_error(error.message());
  }
  INFO << "Server: wrote bytes: " << bytes_transferred;
}
