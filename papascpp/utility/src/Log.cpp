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
spdlog::level::level_enum  PDebug::slevel = spdlog::level::info;
std::vector<spdlog::sink_ptr> Log::m_sinks;
std::vector<spdlog::sink_ptr> PDebug::m_sinks;

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
  if (PDebug::slevel == spdlog::level::info) std::remove("cpppapasphysicsoutput.txt");  // delete file
  m_sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_st>("cpppapasphysicsoutput.txt", true));

  auto plogger = std::make_shared<spdlog::logger>("pdebug", begin(m_sinks), end(m_sinks));
  plogger->set_level(PDebug::slevel);  //   ::info);
  plogger->set_pattern("%v");
  spdlog::register_logger(plogger);
}
void PDebug::consoleinit() {
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
