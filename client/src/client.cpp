#include "client.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;

// TODO: may be create classes Connection and FileReader in different files? and
// separate implementation and interface
class Connection {
 public:
  Connection(std::string const& ip, unsigned short port) {
    boost::system::error_code error;
    m_socket.connect(ip::tcp::endpoint(ip::address::from_string(ip), port),
                     error);
    if (error)
      throw std::runtime_error(error.message());
  }

  ~Connection() {
    if (m_socket.is_open())
      m_socket.close();
  }

  void sendMsg(std::string const& s) {
    assert(m_socket.is_open());
    asio::streambuf buf;
    std::ostream out(&buf);
    out << s;
    boost::system::error_code error;
    write(m_socket, buf, error);
    if (error) {
      throw std::runtime_error(error.message());
    }
  }

  std::string readMsg() {
    assert(m_socket.is_open());
    asio::streambuf buf;
    boost::system::error_code error;
    asio::read(m_socket, buf, boost::asio::transfer_all(), error);
    if (error && error != asio::error::eof)
      throw std::runtime_error(error.message());
    std::ostringstream out;
    out << &buf;
    return out.str();
  }

 private:
  asio::io_service m_ioService;
  ip::tcp::socket m_socket{m_ioService};

 private:
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

class FileReader {
 public:
  explicit FileReader(fs::path const& filePath) {
    std::string data(
        (std::istreambuf_iterator<char>(
            *(std::unique_ptr<std::ifstream>(new std::ifstream(filePath)))
                 .get())),
        std::istreambuf_iterator<char>());
    m_data = std::move(data);
  }

  size_t hash() {
    std::hash<std::string> hash_fn;
    return hash_fn(m_data);
  }

  std::string const& data() const {
    return m_data;
  }

 private:
  std::string m_data;
};

Client::Client(fs::path const& filePath, std::string_view ip,
               unsigned short port)
    : m_impConn{std::make_unique<Connection>(ip.data(), port)} {
  if (!fs::exists(filePath) || fs::is_directory(filePath) ||
      !filePath.has_filename())
    throw std::runtime_error("File path " + filePath.string() + "is incorrect");
  m_filePath = filePath;
};

void Client::run() {
  FileReader fileReader(m_filePath);
  m_impConn->sendMsg(fileReader.data());
  auto const serverMsgHash = m_impConn->readMsg();
  std::cout << serverMsgHash << std::endl;
  /*auto const clientHash    = std::to_string(fileReader.hash());
  assert(clientHash == serverMsgHash);*/
}

Client::~Client() = default;
