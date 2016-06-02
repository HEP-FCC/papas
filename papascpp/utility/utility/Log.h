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
  };
  
}


#endif /* log_h */
