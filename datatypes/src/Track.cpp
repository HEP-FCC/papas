
#include "Id.h"
#include "PDebug.h"
#include "pTrack.h"

namespace papas {

Track::Track(const TVector3& p3, double charge, Path::Ptr path)
    : m_uniqueId(Id::makeTrackId()), m_p3(p3), m_charge(charge), m_path(path) {}

std::string Track::info() const {
  return string_format("%7.2f %7.2f %5.2f %5.2f", energy(), pt(), theta(), p3().Phi());
}

std::ostream& operator<<(std::ostream& os, const Track& track) {
  os << "Track :" << Id::pretty(track.id()) << ":" << track.id() << ": " << track.info();
  return os;
}

}  // end namespace papas