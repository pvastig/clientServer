#include "file_reader.h"

#include <fstream>
#include <memory>

FileReader::FileReader(fs::path const& filePath)
    : m_data(std::string(
          (std::istreambuf_iterator<char>(
              *(std::unique_ptr<std::ifstream>(new std::ifstream(filePath))))),
          std::istreambuf_iterator<char>())) {}
