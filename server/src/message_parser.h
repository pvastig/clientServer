#pragma once

#include <optional>
#include <string>
#include <vector>

struct Row {
  std::string date;
  double price1;
  double price2;
};
using Rows = std::vector<Row>;

class MessageParser {
 public:
  MessageParser(std::string const& mess);
  void parse();

  size_t countLine() const {
    return m_countLine;
  }

  Rows const& rows() const {
    return m_rows;
  }

  std::string const& message() const {
    return m_mess;
  }

 private:
  std::string m_mess;
  size_t m_countLine = 0;
  Rows m_rows;

 private:
  std::optional<double> getValue(std::string const& str);
};

std::optional<Row> findMax(Rows rows);
