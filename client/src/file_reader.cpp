#include "file_reader.h"

#include <fstream>
#include <memory>

FileReader::FileReader(fs::path const& filePath)
    : m_data{std::istreambuf_iterator<char>(
            *std::make_unique<std::ifstream>(filePath)),
             std::istreambuf_iterator<char>()} {}
