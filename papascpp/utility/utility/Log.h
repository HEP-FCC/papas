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
// TODOO split into separate files
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

class PDebug {

public:
  PDebug() {
    s_fname = "";
    slevel = spdlog::level::err;
  };

  /// Tells PDebug where to write output
  static void On(std::string fname) {
    s_fname = fname;
    slevel = spdlog::level::info;
  };

  /// Write to output (this is either null or a file)
  template <typename T>
  static spdlog::details::line_logger write(const T& t) { return log()->info(t); };
  /// Write to output (this is either null or a file)
  template <typename... Args>
  static spdlog::details::line_logger write(const char* fmt, const Args&... args) {
    return log()->info(fmt, args...);
  };
  
  static void flush() {PDebug::log()->flush();}

private:
  static spdlog::details::line_logger info() { return log()->info(); }
  static spdlog::details::line_logger write() { return log()->info(); }
  static spdlog::details::line_logger warn() { return log()->warn(); }
  static spdlog::details::line_logger error() { return log()->error(); }
  static spdlog::details::line_logger debug() { return log()->debug(); }
  static void init();
  static void consoleinit();
  static std::shared_ptr<spdlog::logger> log();
  static bool logInitialized;
  

  static std::vector<spdlog::sink_ptr> m_sinks;
  static spdlog::level::level_enum slevel;  // err or info
  static std::string s_fname;
};
}

#endif /* log_h */