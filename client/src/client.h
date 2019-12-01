#pragma once

#include <filesystem>
#include <memory>
#include <string_view>

namespace fs = std::filesystem;

class Connection;

class Client {
 public:
  Client(fs::path const& filePath, std::string_view ip, unsigned short port);
  void run();
  ~Client();

  Client(Client const&) = delete;
  Client& operator=(Client) = delete;
  Client(Client&&)          = delete;
  Client& operator=(Client&&) = delete;

 private:
  fs::path m_filePath;
  std::unique_ptr<Connection> m_implConn;
};
