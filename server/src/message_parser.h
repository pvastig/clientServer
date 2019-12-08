#pragma once

#include <cassert>
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
  explicit MessageParser(std::string const& mess);
  MessageParser(MessageParser const&) = default;
  MessageParser& operator=(MessageParser const&) = default;
  MessageParser(MessageParser&&)                 = default;
  MessageParser& operator=(MessageParser&&) = default;
  ~MessageParser()                          = default;

  void parse();

  size_t countLine() const {
    return m_countLine;
  }

  Rows rows() const& {
    return m_rows;
  }
  Rows rows() && {
    return std::move(m_rows);
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

std::optional<Row> getMaxRow(Rows rows);
