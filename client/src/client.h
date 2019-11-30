#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class Client {
 public:
  Client(fs::path filePath);
  void run();

 private:
  fs::path m_filePath;
};
