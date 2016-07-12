//
//  Log.cpp
//  papas
//
//  Created by Alice Robson on 31/05/16.
//
//
#include "Log.h"
#include <iostream>
#include <string>
namespace papas {

bool Log::logInitialized = false;
bool PDebug::logInitialized = false;
spdlog::level::level_enum PDebug::slevel = spdlog::level::info;
std::vector<spdlog::sink_ptr> Log::m_sinks;
std::vector<spdlog::sink_ptr> PDebug::m_sinks;
std::string PDebug::s_fname = "";

void Log::init() {
  logInitialized = true;
  m_sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_st>("logfile", true));
  auto combined_logger = std::make_shared<spdlog::logger>("papas_logger", begin(m_sinks), end(m_sinks));
  // register it if you need to access it globally
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

void PDebug::init() {
  logInitialized = true;
  // we either create a null sink
  // or we sink to a file. Note the file will be deleted before it is initialised
  if (PDebug::s_fname == "") {  // no output
    m_sinks.push_back(std::make_shared<spdlog::sinks::null_sink_st>());
  } else {  // output to named file //TODO error checking
    if (PDebug::slevel == spdlog::level::info) std::remove(PDebug::s_fname.c_str());  // delete file
    m_sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_st>(PDebug::s_fname.c_str(), true));
  }
  auto plogger = std::make_shared<spdlog::logger>("pdebug", begin(m_sinks), end(m_sinks));
  plogger->set_level(PDebug::slevel);
  plogger->set_pattern("%v");
  spdlog::register_logger(plogger);
}

void PDebug::consoleinit() {  // for debugging
  logInitialized = true;
  auto console = spdlog::stdout_logger_mt("pdebug", false);
  console->set_level(spdlog::level::info);
  console->set_pattern("PB: %v");
}

std::shared_ptr<spdlog::logger> PDebug::log() {
  if (!logInitialized) {
    // PDebug::consoleinit();
    PDebug::init();
  }
  return spdlog::get("pdebug");
}
}  // namespace papas