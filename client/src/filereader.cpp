#include "filereader.h"

#include <fstream>

FileReader::FileReader(fs::path const& filePath) {
  std::string data(
      (std::istreambuf_iterator<char>(
          *(std::unique_ptr<std::ifstream>(new std::ifstream(filePath)))
               .get())),
      std::istreambuf_iterator<char>());
  m_data = std::move(data);
}

size_t FileReader::hash() const {
  std::hash<std::string> hash_fn;
  return hash_fn(m_data);
}

std::string const& FileReader::data() const {
  return m_data;
}
