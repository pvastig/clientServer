#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include <boost/date_time.hpp>

using StringArray    = std::vector<std::string>;
namespace posix_time = boost::posix_time;

struct Row {
  std::string date;
  double price1;
  double price2;
};

using Rows = std::map<posix_time::ptime, Row>;

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
  bool insert(Row row);
};

std::optional<Row> findMax(Rows rows);
