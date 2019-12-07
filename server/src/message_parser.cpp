#include "message_parser.h"

#include "log.h"

#include <cassert>
#include <string>

#include <boost/algorithm/string.hpp>

posix_time::ptime convertToTime(std::string const& s) {
  using namespace boost::posix_time;
  std::vector<std::string> dateTime;
  boost::split(dateTime, s, boost::is_any_of(" "));

  std::vector<std::string> splitDate;
  boost::split(splitDate, dateTime[0], boost::is_any_of("."));
  std::vector<std::string> splitTime;
  boost::split(splitTime, dateTime[1], boost::is_any_of(":"));

  if (splitDate.size() != 3)
    throw std::logic_error(s + " has incorrect format");
  if (splitTime.size() != 3)
    throw std::logic_error(s + " has time format");

  ptime pt;
  try {
    std::string const ts(splitDate[2] + splitDate[1] + splitDate[0] + "T" +
                         splitTime[0] + splitTime[1] + splitTime[2]);
    pt = from_iso_string(ts);
  } catch (std::exception&) {
    throw std::logic_error(s + " has incorrect format");
  }
  return pt;
}

MessageParser::MessageParser(std::string const& mess) : m_mess(mess) {}
// TODO: split the function on parts
void MessageParser::parse() {
  if (m_mess.empty())
    return;

  std::vector<std::string> lines;
  boost::split(lines, m_mess, boost::is_any_of("\n"));
  for (auto const& line : lines) {
    ++m_countLine;
    std::vector<std::string> splitStr;
    if (line.empty()) {
      WARN << "Server: line " << m_countLine << " : is empty";
      continue;
    }
    boost::split(splitStr, line, boost::is_any_of(","));
    assert(!splitStr.empty());
    if (splitStr.empty())
      continue;
    if (splitStr.size() != 3) {
      WARN << "Server: line " << m_countLine << ": has invalid format";
      continue;
    }

    Row row;
    bool isValid = true;
    size_t index = 0;
    if (auto const& colDate = splitStr[index++]; !colDate.empty()) {
      row.date = colDate;
    } else {
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
           << strValue << " is incorrect";
      isValid = false;
    }
    // we fill columns in only correct data
    if (isValid) {
      if (bool inserted = insert(row); !inserted)
        WARN << "Server: line " << line << ", date: " << row.date
             << " exists in the file";
    }
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

bool MessageParser::insert(Row row) {
  auto const& [date, price1, price2] = row;
  auto const pt                      = convertToTime(date);
  auto const& [item, inserted] = m_rows.emplace(pt, Row{date, price1, price2});
  return inserted;
}

std::optional<Row> findMax(Rows rows) {
  if (rows.empty())
    return std::nullopt;
  auto const& [secondFromEpoch, row] = *rows.rbegin();
  return row;
}
