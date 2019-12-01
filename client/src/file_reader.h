#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class FileReader {
 public:
  explicit FileReader(fs::path const& filePath);
  size_t hash() const;
  std::string const& data() const;

 private:
  std::string m_data;
};
