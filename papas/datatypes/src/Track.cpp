#include "papas/datatypes/Track.h"

#include <iomanip> //lxplus needs this
#include <iostream>

#include "papas/datatypes/IdCoder.h"
#include "papas/utility/StringFormatter.h"

namespace papas {

Track::Track(const TVector3& p3, double charge, const std::shared_ptr<Path> path, unsigned int index, char subtype)
    : m_id(IdCoder::makeId(index, IdCoder::ItemType::kTrack, subtype, p3.Mag())),
      m_p3(p3),
      m_charge(charge),
      m_path(path) {}

std::string Track::info() const {
  return string_format("%7.2f %7.2f %5.2f %5.2f", energy(), p3().Perp(), M_PI / 2. - p3().Theta(), p3().Phi());
}

std::ostream& operator<<(std::ostream& os, const Track& track) {
  os << "Track: " << std::setw(6) << std::left << IdCoder::pretty(track.id()) << ":" << track.id() << ": "
     << track.info();
  return os;
}

}  // end namespace papas
