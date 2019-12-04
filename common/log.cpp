#include "log.h"

#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>

namespace expr     = boost::log::expressions;
namespace keywords = boost::log::keywords;

// Defines a global logger initialization routine
BOOST_LOG_GLOBAL_LOGGER_INIT(logger, loggerType) {
  loggerType lg;
  logging::add_common_attributes();
  logging::add_file_log(
      keywords::file_name     = SYS_LOGFILE,
      keywords::rotation_size = 1024 * 1024 * 20,  // megabytes
      keywords::time_based_rotation =
          boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
      keywords::auto_flush = true,
      // TODO: make possability to set outside
      // keywords::open_mode  = (std::ios::out | std::ios::app),
      keywords::format =
          (expr::stream << expr::format_date_time<boost::posix_time::ptime>(
                               "TimeStamp", "%d.%m.%y %H:%M:%S.%f")
                        << " ["
                        << expr::attr<boost::log::trivial::severity_level>(
                               "Severity")
                        << "]: " << expr::smessage));

  // TODO: make possability to set outside
  logging::add_console_log(
      std::clog,
      keywords::format =
          (expr::stream << expr::format_date_time<boost::posix_time::ptime>(
                               "TimeStamp", "%d.%m.%y %H:%M:%S.%f")
                        << " ["
                        << expr::attr<boost::log::trivial::severity_level>(
                               "Severity")
                        << "]: " << expr::smessage));
  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::info);
  return lg;
}
