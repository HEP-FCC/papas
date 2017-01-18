
#include "papas/datatypes/Identifier.h"
#include "papas/utility/PDebug.h"
#include "papas/datatypes/Track.h"

namespace papas {

Track::Track(const TVector3& p3, double charge, Path::Ptr path, char subtype)
  : m_uniqueId(Identifier::makeId(Identifier::ItemType::kTrack, subtype, p3.Mag())),
               m_p3(p3),
               m_charge(charge),
               m_path(path)
  {}

std::string Track::info() const {
  return string_format("%7.2f %7.2f %5.2f %5.2f", energy(), pt(), theta(), p3().Phi());
}

std::ostream& operator<<(std::ostream& os, const Track& track) {
  os << "Track: " << std::setw(6)<< std::left << Identifier::pretty(track.id()) << ":" << track.id() << ": " << track.info();
  return os;
}

}  // end namespace papas
