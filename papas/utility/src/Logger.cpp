// This file originated from ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "papas/utility/Logger.h"
#include <memory>

namespace papaslog {

std::shared_ptr<const Logger>
getDefaultLogger(const std::string& name, const Logging::Level& lvl, std::ostream* log_stream) {
  using namespace Logging;
  auto print = std::make_shared<DefaultFilterPolicy>(lvl);
  auto output = std::make_shared<LevelOutputDecorator>(std::make_shared<NamedOutputDecorator>(
      std::make_shared<TimedOutputDecorator>(std::make_shared<DefaultPrintPolicy>(log_stream)), name));
  return std::make_shared<const Logger>(std::move(output), std::move(print));
}

<<<<<<< HEAD
std::shared_ptr<const papaslog::Logger> papaslogger = nullptr;
=======
std::shared_ptr<const papaslog::Logger> papaslogger = papaslog::getDefaultLogger("Papas Logger", papaslog::Logging::WARNING);
>>>>>>> official/logtest
std::shared_ptr<const papaslog::Logger> logger() { return papaslogger; };

}  // end of namespace papaslog
