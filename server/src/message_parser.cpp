#include "message_parser.h"

#include "log.h"

#include <algorithm>
#include <cassert>

#include <boost/algorithm/string.hpp>

MessageParser::MessageParser(std::string const& mess) : m_mess(mess) {}
// TODO: split the function on parts
void MessageParser::parse() {
  if (m_mess.empty())
    return;

  std::vector<std::string> lines;
  boost::split(lines, m_mess, boost::is_any_of("\n"));
  m_rows.reserve(lines.size());
  for (auto const& line : lines) {
    ++m_countLine;
    std::vector<std::string> splitStr;
    if (line.empty()) {
      WARN << "Server: line " << m_countLine << " : is empty";
      continue;
    }
    boost::split(splitStr, line, boost::is_any_of(","));
    assert(!splitStr.empty());
    if (splitStr.empty()) {
      continue;
    }
    if (splitStr.size() != 3) {
      WARN << "Server: line " << m_countLine << ": has invalid format";
      continue;
    }

    Row row;
    bool isValid = true;
    // TODO: thinking about indexes
    size_t index = 0;
    if (auto const& colDate = splitStr[index++]; !colDate.empty())
      row.date = colDate;
    else {
      WARN << "Server: line " << line << ", column " << index
           << ": the value is empty" << std::endl;
      isValid = false;
    }  // TODO: remove dublicated code
    if (auto const& strValue = splitStr[index++];
        auto optValue        = getValue(strValue))
      row.price1 = optValue.value();
    else {
      WARN << "Server: line " << line << ", column " << index << ": the value "
           << strValue << " is incorrect" << std::endl;
      isValid = false;
    }
    if (auto const& strValue = splitStr[index++];
        auto optValue        = getValue(strValue))
      row.price2 = optValue.value();
    else {
      WARN << "Server: line " << line << ", column " << index << ": the value "
           << strValue << " is incorrect" << std::endl;
      isValid = false;
    }
    // we fill columns in only correct data
    if (isValid)
      m_rows.emplace_back(row);
  }
  assert(m_countLine > m_rows.size());
  INFO << "Server: total line: " << m_countLine << ", good: " << m_rows.size()
       << ", warn: " << (m_countLine - m_rows.size());
}

std::optional<double> MessageParser::getValue(std::string const& str) {
  double value = 0.;
  try {
    value = std::stod(str);
  } catch (std::exception&) {
    return std::nullopt;
  }
  return value;
}

std::optional<Row> findMax(Rows rows) {
  std::sort(rows.begin(), rows.end(), [](auto const& row1, auto const& row2) {
    return row1.date < row2.date;
  });

  if (!rows.empty())
    return rows.back();
  return std::nullopt;
}
