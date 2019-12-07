#include "message_parser.h"

#include "log.h"

#include <cassert>
#include <functional>
#include <string>
#include <string_view>

#include <boost/algorithm/string.hpp>

const std::string msgServerLine = "Server: line: ";

namespace {
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
}  // namespace

MessageParser::MessageParser(std::string const& mess) : m_mess(mess) {}
void MessageParser::parse() {
  if (m_mess.empty()) {
    ERROR << "Server: input message is empty";
    return;
  }

  auto splitMessageLines = [this]() {
    StringArray lines;
    boost::split(lines, m_mess, boost::is_any_of("\n"));
    return lines;
  };

  auto splitAndCheckLine = [this](auto const& line) {
    StringArray data;
    boost::split(data, line, boost::is_any_of(","));
    assert(!data.empty());
    if (data.empty())
      return StringArray{};
    if (data.size() != 3) {
      WARN << msgServerLine << m_countLine << ", " << line
           << " has invalid format";
      StringArray{};
    }
    INFO << data[0] << " " << data[1] << " " << data[2];
    return data;
  };

  for (auto&& line : splitMessageLines()) {
    ++m_countLine;
    if (line.empty()) {
      WARN << msgServerLine << m_countLine << " is empty";
      continue;
    }

    auto const dataLines = splitAndCheckLine(line);
    if (dataLines.empty())
      continue;

    Row row;
    size_t colIndex     = 0;
    auto processColDate = [line, colIndex](auto const& colDate,
                                           std::string& date) {
      if (!colDate.empty()) {
        date = colDate;
      } else {
        WARN << msgServerLine << line << ", column: " << colIndex
             << ": the value is empty";
        return false;
      }
      return true;
    };

    auto processColPrice = [this, line, colIndex](const auto& colPrice,
                                                  double& value) {
      if (auto const& strValue = colPrice; auto optValue = getValue(strValue))
        value = optValue.value();
      else {
        WARN << msgServerLine << line << ", column: " << colIndex
             << ": the value " << strValue << " is incorrect" << std::endl;
        return false;
      }
      return true;
    };

    bool isValid = processColDate(dataLines[colIndex++], row.date) &&
                   processColPrice(dataLines[colIndex++], row.price1) &&
                   processColPrice(dataLines[colIndex++], row.price2);

    // we fill in array in the case all data is correct
    if (isValid) {
      if (bool inserted = insert(row); !inserted)
        WARN << msgServerLine << line << ", date: " << row.date
             << " has existed";
    }
  }
  assert(m_countLine > m_rows.size());
  INFO << "Server: total line: " << m_countLine << ", good: " << m_rows.size()
       << ", warn: " << m_countLine - m_rows.size();
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
  for (auto& [secondFromEpoch, row] : rows) {
    auto [date, price1, price2] = row;
    INFO << secondFromEpoch << " " << date << " " << price1 << " " << price2;
  }
  auto const& [secondFromEpoch, row] = *rows.rbegin();
  return row;
}
