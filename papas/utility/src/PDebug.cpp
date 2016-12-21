//
//  Log.cpp
//  papas
//
//  Created by Alice Robson on 31/05/16.
//
//
#include "papas/utility/PDebug.h"
#include <iostream>
#include <string>

namespace papas {

bool PDebug::logInitialized = false;
spdlog::level::level_enum PDebug::slevel = spdlog::level::info;
std::vector<spdlog::sink_ptr> PDebug::m_sinks;
std::string PDebug::s_fname = "";
bool PDebug::s_On = false;

void PDebug::init() {  // we either create a null sink or we sink to a file
  logInitialized = true;
  if (PDebug::s_fname == "") {  // no output
    m_sinks.push_back(std::make_shared<spdlog::sinks::null_sink_mt>());
  } else {  // output to named file //TODO error checking
    if (PDebug::slevel == spdlog::level::info) std::remove(PDebug::s_fname.c_str());  // delete file
    m_sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>(PDebug::s_fname.c_str(), true));
  }
  auto plogger = std::make_shared<spdlog::logger>("pdebug", begin(m_sinks), end(m_sinks));
  plogger->set_level(PDebug::slevel);  // what level output will be sent to log
  plogger->set_pattern("%v");
  spdlog::register_logger(plogger);
}

void PDebug::consoleinit() {  // for debugging goes to screen instead
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
