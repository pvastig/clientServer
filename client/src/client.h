#pragma once

#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

class Client {
 public:
  Client(fs::path const& filePath);
  void run(std::string_view ip, unsigned short port);

 private:
  fs::path m_filePath;
};
