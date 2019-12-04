#include "connection.h"
#include "message_parser.h"

#include "log.h"
#include "util.h"

#include <iostream>

#include <boost/bind.hpp>

Connection::Connection(asio::io_service& io_service) : m_socket(io_service) {}

Connection::Connection() {}

void Connection::start() {
  m_socket.async_read_some(asio::buffer(m_data),
                           boost::bind(&Connection::read, shared_from_this(),
                                       asio::placeholders::error,
                                       asio::placeholders::bytes_transferred));
}

void Connection::stop() {
  if (m_socket.is_open())
    m_socket.close();
}

void Connection::read(boost::system::error_code const& error, size_t) {
  if (error)
    throw std::runtime_error(error.message());

  MessageParser mp(m_data);
  mp.parse();

  std::string const message = std::to_string(util::hashStr(mp.message())) +
                              "," + std::to_string(mp.countLine());

  if (auto const row = findMax(mp.rows())) {
    auto const& value = row.value();
    if (value.price2 == 0.0)
      throw std::logic_error("Divided by zero, check columns");
    INFO << "Server: " << value.date << " " << (value.price1 / value.price2);
  }
  m_socket.async_write_some(asio::buffer(message, message.size()),
                            boost::bind(&Connection::write, shared_from_this(),
                                        asio::placeholders::error,
                                        asio::placeholders::bytes_transferred));
}
void Connection::write(boost::system::error_code const& error, size_t) {
  if (error) {
    throw std::runtime_error(error.message());
  }
}
