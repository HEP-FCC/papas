//
//  Log.cpp
//  papas
//
//  Created by Alice Robson on 31/05/16.
//
//
#include <string>
#include <iostream>
#include "Log.h"
namespace papas{

  bool Log::logInitialized =false;
  
  
void Log::init(){
  logInitialized=true;
  //auto console = spdlog::stdout_logger_mt("console", true );
  
  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
  sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_st>("logfile",true));
  auto combined_logger = std::make_shared<spdlog::logger>("papas_logger", begin(sinks), end(sinks));
  //register it if you need to access it globally
  spdlog::register_logger(combined_logger);
  combined_logger->set_pattern("%l: %v");
  combined_logger->set_level(spdlog::level::debug);
  
}

std::shared_ptr<spdlog::logger> Log::log() {
  if (!logInitialized) {
    Log::init();
  }
  return spdlog::get("papas_logger");
}

} //namesapce papas