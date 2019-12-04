#pragma once

#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>

namespace logging = boost::log;

#define INFO BOOST_LOG_SEV(logger::get(), logging::trivial::info)
#define WARN BOOST_LOG_SEV(logger::get(), logging::trivial::warning)
#define ERROR BOOST_LOG_SEV(logger::get(), logging::trivial::error)

// TODO: make possability to set path outside
#define SYS_LOGFILE "clientServer.log"

using loggerType =
    boost::log::sources::severity_logger_mt<logging::trivial::severity_level>;

BOOST_LOG_GLOBAL_LOGGER(logger, loggerType)
