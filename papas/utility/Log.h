//
//  log.h
//  papas
//
//  Created by Alice Robson on 31/05/16.
//
//

#ifndef log_h
#define log_h

#include "papas/utility/StringFormatter.h"
#include "spdlog/sinks/null_sink.h"
#include "spdlog/spdlog.h"

#include <iostream>

namespace papas {
/** Logging class based on external speedy logging library spdlog
*  https://github.com/gabime/spdlog
*/
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
  
  /// Write to output (this is either null or a file)
  template <typename T>
  static spdlog::details::line_logger error(const T& t) {
    return log()->error(t);
  }
  
  /// Write to output (this is either null or a file)
  template <typename... Args>
  static void error(const char* fmt, const Args&... args) {
    if (logInitialized) log()->error(fmt, args...);
  }
  
  /// Write to output (this is either null or a file)
  template <typename T>
  static spdlog::details::line_logger warn(const T& t) {
    return log()->warn(t);
  }
  
  /// Write to output (this is either null or a file)
  template <typename... Args>
  static void warn(const char* fmt, const Args&... args) {
    if (logInitialized) log()->warn(fmt, args...);
  }
  
  /// Write to output (this is either null or a file)
  template <typename T>
  static spdlog::details::line_logger info(const T& t) {
    return log()->info(t);
  }
  
  /// Write to output (this is either null or a file)
  template <typename... Args>
  static void info(const char* fmt, const Args&... args) {
    if (logInitialized) log()->info(fmt, args...);
  }

};
}

#endif /* log_h */
