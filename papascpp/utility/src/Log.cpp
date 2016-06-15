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

  bool Log::logInitialized = false;
  bool PDebug::logInitialized = false;
  std::vector<spdlog::sink_ptr> Log::m_sinks;
  std::vector<spdlog::sink_ptr> PDebug::m_sinks;
  
void Log::init(){
  logInitialized=true;
  //auto console = spdlog::stdout_logger_mt("console", true );
  

  //m_sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
  m_sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_st>("logfile",true));
  auto combined_logger = std::make_shared<spdlog::logger>("papas_logger", begin(m_sinks), end(m_sinks));
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
  
  void PDebug::init(){
    logInitialized=true;
    std::remove("/Users/alice/work/Outputs/physicsoutput.log"); // delete file
    m_sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_st>("/Users/alice/work/Outputs/physicsoutput.log",true));
    auto plogger = std::make_shared<spdlog::logger>("pdebug", begin(m_sinks), end(m_sinks));
    //auto plogger = spdlog::sinks::simple_file_sink_st("pdebug","/Users/alice/work/Outputs/physicsoutput");
    plogger->set_level(spdlog::level::info);
    plogger->set_pattern("%v");
    spdlog::register_logger(plogger);
  
  }
  void PDebug::consoleinit(){
    logInitialized=true;
    auto console = spdlog::stdout_logger_mt("pdebug", false );
    console->set_level(spdlog::level::info);
    console->set_pattern("PB: %v");
    
    
  }
  
  std::shared_ptr<spdlog::logger> PDebug::log() {
    if (!logInitialized) {
      PDebug::consoleinit();
      //PDebug::init();
    }
    return spdlog::get("pdebug");
  }

} //namesapce papas