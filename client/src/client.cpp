#include "client.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>

class Message {
 public:
  Message() {}
  std::string readFile(fs::path filePath) {
    std::string v(
        (std::istreambuf_iterator<char>(
            *(std::unique_ptr<std::ifstream>(new std::ifstream(filePath)))
                 .get())),
        std::istreambuf_iterator<char>());
    return v;
  }
};

Client::Client(fs::path filePath) {
  if (!fs::exists(filePath) || fs::is_directory(filePath) ||
      !filePath.has_filename())
    throw std::runtime_error("File path " + filePath.string() + "is incorrect");
  m_filePath = filePath;
};

std::optional<double> getValue(std::string const& str, size_t line) {
  double value = 0.;
  try {
    value = std::stod(str);
  } catch (std::exception&) {
    std::cerr << "Line " << line << ": the value " << str << " is incorrect"
              << std::endl;
    return std::nullopt;
  }
  return value;
}

class MessageParser {
 public:
  MessageParser(std::string const& mess) : m_mess(mess) {}
  void parse() {
    if (m_mess.empty())
      return;
    std::vector<std::string> lines;
    boost::split(lines, m_mess, boost::is_any_of("\n"));
    m_col1.reserve(lines.size());
    m_col2.reserve(lines.size());
    m_col3.reserve(lines.size());

    size_t countLine = 0;
    for (auto const& line : lines) {
      ++countLine;
      std::vector<std::string> splitStr;
      if (line.empty())
        std::cerr << "Line " << countLine << ": is empty" << std::endl;
      boost::split(splitStr, line, boost::is_any_of(","));
      if (splitStr.empty()) {
        continue;
      }
      if (splitStr.size() != 3) {
        std::cerr << "Line " << countLine << ": has invalid format"
                  << std::endl;
        continue;
      }
      assert(!splitStr[0].empty());
      m_col1.emplace_back(splitStr[0]);
      if (auto optValue = getValue(splitStr[1], countLine))
        m_col2.emplace_back(optValue.value());
      if (auto optValue = getValue(splitStr[2], countLine))
        m_col3.emplace_back(optValue.value());
    }
  }

  auto col1() const {
    return m_col1;
  }
  auto col2() const {
    return m_col2;
  }
  auto col3() const {
    return m_col3;
  }

 private:
  using column = std::vector<std::string>;
  std::string m_mess;
  column m_col1;
  std::vector<double> m_col2;
  std::vector<double> m_col3;
};

void Client::run() {
  Message mess;
  auto m = mess.readFile(m_filePath);
  std::cout << m.size() << std::endl;

  MessageParser p(m);
  p.parse();
  if (p.col1().size() != p.col2().size() || p.col2().size() != p.col3().size())
    return;  // TODO: write some message here

  for (auto item : p.col1()) {
    std::cout << item << " ";
  }
  std::cout << "\n";
  for (auto item : p.col2()) {
    std::cout << item << " ";
  }
  std::cout << "\n";
  for (auto item : p.col3()) {
    std::cout << item << " ";
  }
}
