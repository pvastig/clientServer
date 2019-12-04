#include "connection.h"
#include "message_parser.h"

#include "../common/log.h"

#include <iostream>

#include <boost/bind.hpp>

Connection::Connection(asio::io_service& io_service) : m_socket(io_service) {}

Connection::Connection(std::string const& ip,
                       unsigned short port)  // TODO: tempararly
{}

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
  if (error) {
    throw std::runtime_error(error.message());
  }
  MessageParser mp(m_data);
  mp.parse();
  std::cout << mp.countLine() << std::endl;
  std::string message = std::to_string(mp.countLine());
  std::cout << mp.rows().size() << std::endl;
  std::optional<Row> row = findMax(mp.rows());
  message                = row.value().date;
  INFO << message;
  m_socket.async_write_some(asio::buffer(message, maxLength),
                            boost::bind(&Connection::write, shared_from_this(),
                                        asio::placeholders::error,
                                        asio::placeholders::bytes_transferred));
}
void Connection::write(boost::system::error_code const& error, size_t) {
  if (error) {
    throw std::runtime_error(error.message());
  }
}
