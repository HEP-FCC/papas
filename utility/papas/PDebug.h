//
//  log.h
//  papas
//
//  Created by Alice Robson on 31/05/16.
//
//

#ifndef pdebug_h
#define pdebug_h

#include "StringFormatter.h"
#include "spdlog/sinks/null_sink.h"
#include "spdlog/spdlog.h"

#include <iostream>

namespace papas {
/** Produces Physics debug output and writes to file
 *
 * Usage:
 * @code
 *  PDebug::On("papas.log");  //If not turned on nothing will be produced
 *  PDebug::write("problem with track not found :{}", id);
 * @endcode
*/
class PDebug {
  // produce physics debug output
public:
  /// Constructor  defaults to debugger being turned off/ logging only errors
  PDebug() {
    s_fname ="";
    slevel=spdlog::level::err;
    s_On=false;
  };

  /// Tells PDebug where to write output and sets output level to info
  /// @param[in] fname filename
  static void On(const std::string& fname) {
    s_On=true;
    s_fname = fname;
    slevel = spdlog::level::info;
  };

  /// Write to output (this is either null or a file)
  template <typename T>
  static spdlog::details::line_logger write(const T& t) {
    return log()->info(t);
  };
    
  /// Write to output (this is either null or a file)
  template <typename... Args>
  static void write(const char* fmt, const Args&... args) {
    if (s_On)
      log()->info(fmt, args...);
  };

  static void flush() { PDebug::log()->flush(); }
  static std::shared_ptr<spdlog::logger> log();

private:
  static spdlog::details::line_logger info() { return log()->info(); }
  static spdlog::details::line_logger write() { return log()->info(); }
  static spdlog::details::line_logger warn() { return log()->warn(); }
  static spdlog::details::line_logger error() { return log()->error(); }
  static spdlog::details::line_logger debug() { return log()->debug(); }
  static void init();          ///< called when a log message is first encountered
  static void consoleinit();   ///< not used at present
  static bool logInitialized;  ///< whether the pdebug has been initialised yet (happens just once)
  static std::vector<spdlog::sink_ptr> m_sinks;
  static spdlog::level::level_enum slevel;  ///< either err or info
  static std::string s_fname;
  static bool s_On;
};
}

#endif /* pdebug_h */
