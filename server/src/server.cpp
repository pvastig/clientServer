//#include "server.h"

#include <algorithm>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;

struct Row {
  std::string date;
  double price1;
  double price2;
};
using Rows = std::vector<Row>;

class MessageParser {
 public:
  MessageParser(std::string const& mess) : m_mess(mess) {}
  void parse() {
    if (m_mess.empty())
      return;
    std::vector<std::string> lines;
    boost::split(lines, m_mess, boost::is_any_of("\n"));
    m_rows.reserve(lines.size());
    for (auto const& line : lines) {
      ++m_countLine;
      std::vector<std::string> splitStr;
      if (line.empty()) {
        std::cerr << "Line " << m_countLine << ": is empty" << std::endl;
        continue;
      }
      boost::split(splitStr, line, boost::is_any_of(","));
      if (splitStr.empty()) {
        continue;
      }
      if (splitStr.size() != 3) {
        std::cerr << "Line " << m_countLine << ": has invalid format"
                  << std::endl;
        continue;
      }
      // assert(!splitStr[0].empty());
      Row row;
      bool isValid = true;
      if (auto const& colDate = splitStr[0]; !colDate.empty())
        row.date = colDate;
      else {
        std::cerr << "Line " << line << ", column 1: the value is empty"
                  << std::endl;
        isValid = false;
      }
      if (auto optValue = getValue(splitStr[1]))
        row.price1 = optValue.value();
      else {
        std::cerr << "Line " << line << ", column 1: the value " << splitStr[1]
                  << " is incorrect" << std::endl;
        isValid = false;
      }
      if (auto optValue = getValue(splitStr[2]))
        row.price2 = optValue.value();
      else {
        std::cerr << "Line " << line << ", column 2: the value " << splitStr[2]
                  << " is incorrect" << std::endl;
        isValid = false;
      }
      // we fill columns in only correct data
      if (isValid)
        m_rows.emplace_back(row);
    }
  }

  size_t countLine() const {
    return m_countLine;
  }

  Rows const& rows() const {
    return m_rows;
  }

 private:
  std::string m_mess;
  size_t m_countLine = 0;
  Rows m_rows;

 private:
  std::optional<double> getValue(std::string const& str) {
    double value = 0.;
    try {
      value = std::stod(str);
    } catch (std::exception&) {
      return std::nullopt;
    }
    return value;
  }
};

std::optional<Row> findMax(Rows rows) {
  assert(!rows.empty());
  std::sort(rows.begin(), rows.end(), [](auto const& col1, auto const& col2) {
    return col1.date < col2.date;
  });

  if (!rows.empty())
    return rows.back();
  return std::nullopt;
}

class Connection : public boost::enable_shared_from_this<Connection> {
 public:
  using ptr = boost::shared_ptr<Connection>;
  Connection(asio::io_service& io_service) : m_socket(io_service) {}

  static ptr create(asio::io_service& io_service) {
    return ptr(new Connection(io_service));
  }

  ip::tcp::socket& socket() {
    return m_socket;
  }

  void start() {
    m_socket.async_read_some(
        asio::buffer(m_data),
        boost::bind(&Connection::read, shared_from_this(),
                    asio::placeholders::error,
                    asio::placeholders::bytes_transferred));
  }

  void stop() {
    if (m_socket.is_open())
      m_socket.close();
  }

  void read(boost::system::error_code const& error, size_t bytes_transferred) {
    if (error) {
      throw std::runtime_error(error.message());
    }
    std::cout << m_data << std::endl;
    MessageParser mp(m_data);
    mp.parse();
    std::cout << mp.countLine() << std::endl;
    std::string message = std::to_string(mp.countLine());
    std::cout << mp.rows().size() << std::endl;
    std::optional<Row> row = findMax(mp.rows());
    message                = row.value().date;
    std::cout << message << std::endl;

    m_socket.async_write_some(
        asio::buffer(message, maxLength),
        boost::bind(&Connection::write, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }
  void write(boost::system::error_code const& error, size_t bytes_transferred) {
    if (error) {
      throw std::runtime_error(error.message());
    }
  }

 private:
  ip::tcp::socket m_socket;
  static const int maxLength = 1024;
  char m_data[maxLength];

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

class Server {
 public:
  // constructor for accepting connection from client
  Server(asio::io_service& io_service)
      : m_ioService(io_service),
        m_acceptor(
            io_service,
            ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 1271)) {
    startAccept();
  }
  void handleAccept(Connection::ptr connection,
                    const boost::system::error_code& err) {
    if (!err) {
      connection->start();
    }
    startAccept();
  }

 private:
  boost::asio::io_service& m_ioService;
  ip::tcp::acceptor m_acceptor;
  void startAccept() {
    Connection::ptr connection = Connection::create(m_ioService);

    // asynchronous accept operation and wait for a new connection.
    m_acceptor.async_accept(connection->socket(),
                            boost::bind(&Server::handleAccept, this, connection,
                                        asio::placeholders::error));
  }
};

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_service io_service;
    Server server(io_service);
    io_service.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
