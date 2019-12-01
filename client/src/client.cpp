#include "client.h"
#include "connection.h"
#include "filereader.h"

#include <cassert>
#include <iostream>

namespace asio = boost::asio;
namespace ip   = asio::ip;

Client::Client(fs::path const& filePath, std::string_view ip,
               unsigned short port)
    : m_implConn{std::make_unique<Connection>(ip.data(), port)} {
  if (!fs::exists(filePath) || fs::is_directory(filePath) ||
      !filePath.has_filename())
    throw std::runtime_error("File path " + filePath.string() + "is incorrect");
  m_filePath = filePath;
};

void Client::run() const {
  FileReader fileReader(m_filePath);
  m_implConn->sendMsg(fileReader.data());
  auto const serverMsg = m_implConn->readMsg();
  std::cout << serverMsg << std::endl;
}

Client::~Client() = default;
