#include "message_parser.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include <boost/algorithm/string.hpp>

MessageParser::MessageParser(std::string const& mess) : m_mess(mess) {}
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
      std::cerr << "Line " << m_countLine << ": is empty" << std::endl;
      continue;
    }
    boost::split(splitStr, line, boost::is_any_of(","));
    if (splitStr.empty()) {
      continue;
    }
    if (splitStr.size() != 3) {
      std::cerr << "Line " << m_countLine << ": has invalid format"
                << std::endl;
      continue;
    }
    // assert(!splitStr[0].empty());
    Row row;
    bool isValid = true;
    if (auto const& colDate = splitStr[0]; !colDate.empty())
      row.date = colDate;
    else {
      std::cerr << "Line " << line << ", column 1: the value is empty"
                << std::endl;
      isValid = false;
    }
    if (auto optValue = getValue(splitStr[1]))
      row.price1 = optValue.value();
    else {
      std::cerr << "Line " << line << ", column 1: the value " << splitStr[1]
                << " is incorrect" << std::endl;
      isValid = false;
    }
    if (auto optValue = getValue(splitStr[2]))
      row.price2 = optValue.value();
    else {
      std::cerr << "Line " << line << ", column 2: the value " << splitStr[2]
                << " is incorrect" << std::endl;
      isValid = false;
    }
    // we fill columns in only correct data
    if (isValid)
      m_rows.emplace_back(row);
  }
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
  assert(!rows.empty());
  std::sort(rows.begin(), rows.end(), [](auto const& col1, auto const& col2) {
    return col1.date < col2.date;
  });

  if (!rows.empty())
    return rows.back();
  return std::nullopt;
}
