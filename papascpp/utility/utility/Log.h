//
//  log.h
//  papas
//
//  Created by Alice Robson on 31/05/16.
//
//

#ifndef log_h
#define log_h

#include "StringFormatter.h"
#include "spdlog/sinks/null_sink.h"
#include "spdlog/spdlog.h"

#include <iostream>

namespace papas {

class Log {
public:
  Log(){};
  static spdlog::details::line_logger info() { return log()->info(); }
  static spdlog::details::line_logger warn() { return log()->warn(); }
  static spdlog::details::line_logger error() { return log()->error(); }
  static spdlog::details::line_logger debug() { return log()->debug(); }
  static void init();
  static std::shared_ptr<spdlog::logger> log();
  static bool logInitialized;
  static std::vector<spdlog::sink_ptr> m_sinks;
};

}

#endif /* log_h */