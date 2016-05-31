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

namespace papas {


  class Log {
  public:
    Log() {};
    static void init();
    static std::shared_ptr<spdlog::logger> log();
    static bool logInitialized;
  };
  
}


#endif /* log_h */
