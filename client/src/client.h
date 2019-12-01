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

 private:
  fs::path m_filePath;
  std::unique_ptr<Connection> m_implConn;
};
