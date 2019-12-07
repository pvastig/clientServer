#include "client.h"
#include "connection.h"
#include "file_reader.h"

#include "log.h"
#include "util.h"

#include <cassert>

#include <boost/algorithm/string.hpp>

namespace asio = boost::asio;
namespace ip   = asio::ip;

Client::Client(fs::path const& filePath, std::string_view ip,
               unsigned short port)
    : m_implConn{std::make_unique<Connection>(ip.data(), port)} {
  if (!fs::exists(filePath) || fs::is_directory(filePath) ||
      !filePath.has_filename())
    throw std::runtime_error("file path " + filePath.string() + "is incorrect");
  m_filePath = filePath;
}

Client::~Client() = default;

void Client::run() const {
  auto const msg = FileReader(m_filePath).data();
  m_implConn->sendMsg(msg);
  INFO << "Client: send file: " << m_filePath;
  auto const serverMsg = m_implConn->readMsg();

  std::vector<std::string> mess;
  boost::split(mess, serverMsg, boost::is_any_of(","));
  if (mess.size() != 2)
    throw std::runtime_error("wrong received message from server");

  // first message must be hash, second the main message
  if (util::hashStr(msg) != std::stoull(mess[0]))
    throw std::runtime_error("received message is not equal to sent message");
  INFO << "Client: total lines: " << mess[1];
}
