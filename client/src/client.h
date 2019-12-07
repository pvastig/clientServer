#pragma once

#include <filesystem>
#include <memory>
#include <string_view>

namespace fs = std::filesystem;

class Connection;

class Client {
 public:
  Client(fs::path const& filePath, std::string_view ip, unsigned short port);
  Client(Client const&) = delete;
  Client& operator=(Client const&) = delete;
  Client(Client&&)          = delete;
  Client& operator=(Client&&) = delete;
  ~Client();

  void run() const;

 private:
  fs::path m_filePath;
  std::unique_ptr<Connection> m_implConn;
};
