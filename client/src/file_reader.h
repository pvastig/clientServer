#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class FileReader {
 public:
  explicit FileReader(fs::path const& filePath);
  FileReader(FileReader const&) = default;
  FileReader& operator=(FileReader const&) = default;
  FileReader(FileReader&&)                 = default;
  FileReader& operator=(FileReader&&) = default;
  ~FileReader()                       = default;

  std::string data() const& {
    return m_data;
  }

  std::string data() && {
    return std::move(m_data);
  }

 private:
  std::string m_data;
};
