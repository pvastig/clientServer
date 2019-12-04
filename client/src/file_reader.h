#pragma once

#include <cassert>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class FileReader {
 public:
  explicit FileReader(fs::path const& filePath);
  std::string data() const& {
    return m_data;
  }
  std::string data() const&& {
    return std::move(m_data);
  }

 private:
  std::string m_data;
};
