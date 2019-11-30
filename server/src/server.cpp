#include "server.h"

#include <functional>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

class Connection : public boost::enable_shared_from_this<Connection> {
 public:
  using error = boost::system::error_code;
  Connection(asio::io_service& ioService);
  void start(ip::tcp::endpoint ep);
  void stop();
  bool started() const;

 private:
  void on_connect(const error& err);
  void on_read(const error& err, size_t bytes);
  void on_write(const error& err, size_t bytes);
  void do_read();
  void do_write(const std::string& msg);
  void process_data(const std::string& msg);

 private:
  asio::io_service& m_ioService;
  ip::tcp::socket m_sock;
  bool m_started;
  static const int maxMsg    = 1024;
  char m_readBuffer[maxMsg]  = {};
  char m_writeBuffer[maxMsg] = {};
};

using ptr = std::shared_ptr<Connection>;

Connection::Connection(asio::io_service& ioService)
    : m_ioService(ioService), m_sock(m_ioService), m_started(true) {}

void Connection::start(ip::tcp::endpoint ep) {
  m_sock.async_connect(ep,
                       std::bind(&Connection::on_connect, shared_from_this(),
                                 std::placeholders::_1));
}
void Connection::stop() {
  if (!m_started)
    return;
  m_started = false;
  m_sock.close();
}
bool Connection::started() const {
  return m_started;
}

void Connection::on_connect(const error& err) {
  // here you decide what to do with the connection: read or write
  if (!err)
    do_read();
  else
    stop();
}
void Connection::on_read(const error& err, size_t bytes) {
  if (!started())
    return;
  std::string msg(m_readBuffer, bytes);
  if (msg == "can_login")
    do_write("access_data");
  else if (msg.find("data ") == 0)
    process_data(msg);
  else if (msg == "login_fail")
    stop();
}
void Connection::on_write(const error& err, size_t) {
  do_read();
}
void Connection::do_read() {
  m_sock.async_read_some(
      asio::buffer(m_readBuffer),
      std::bind(&Connection::on_read, shared_from_this(), std::placeholders::_1,
                std::placeholders::_2));
}
void Connection::do_write(const std::string& msg) {
  if (!started())
    return;
  // note: in case you want to send several messages before
  // doing another async_read, you'll need several write buffers!
  std::copy(msg.begin(), msg.end(), m_writeBuffer);
  m_sock.async_write_some(
      asio::buffer(m_writeBuffer, msg.size()),
      std::bind(&Connection::on_write, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2));
}
void Connection::process_data(const std::string& msg) {
  // process what comes from server, and then perform another write
}

Server::Server(std::string_view ip, unsigned short port)
    : m_endPoint{ip::address::from_string(ip.data()), port} {}

void Server::run() {
  // ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);
  // connection::ptr(new connection)->start(ep);
  ptr(new Connection(m_ioService))->start(m_endPoint);
}

int main() {
  Server server("127.0.0.1", 1234);
  server.run();
  return 0;
}
