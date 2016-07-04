//
//  log.h
//  papas
//
//  Created by Alice Robson on 31/05/16.
//
//

#ifndef log_h
#define log_h

#include <iostream>
#include "spdlog/spdlog.h"
#include "StringFormatter.h"
//TODOO split into separate files
namespace papas {


  class Log {
  public:
    Log() {};
    static spdlog::details::line_logger info()  {return log()->info();}
    static spdlog::details::line_logger warn()  {return log()->warn();}
    static spdlog::details::line_logger error() {return log()->error();}
    static spdlog::details::line_logger debug() {return log()->debug();}
    static void init();
    static std::shared_ptr<spdlog::logger> log();
    static bool logInitialized;
    static std::vector<spdlog::sink_ptr> m_sinks;
  };
  
  
  class PDebug {
    //TODO allow output file to be set from here
  public:
    PDebug()  {};
    static void On() {level=spdlog::level::info;};
    static spdlog::details::line_logger info()  {return log()->info();}
    static spdlog::details::line_logger write()  {return log()->info();}
    static spdlog::details::line_logger warn()  {return log()->warn();}
    static spdlog::details::line_logger error() {return log()->error();}
    static spdlog::details::line_logger debug() {return log()->debug();}
    static void init();
    static void consoleinit();
    static std::shared_ptr<spdlog::logger> log();
    static bool logInitialized;
    template <typename T> static spdlog::details::line_logger write(const T& t) {return log()->info(t);};
    template <typename... Args> static spdlog::details::line_logger write(const char* fmt, const Args&... args) { return log()->info(fmt,args...);};
    static std::vector<spdlog::sink_ptr> m_sinks;
    static spdlog::level::level_enum level; //err or info
  };
  
}


#endif /* log_h */
