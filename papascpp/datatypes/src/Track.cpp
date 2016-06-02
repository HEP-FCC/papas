
#include "pTrack.h"
#include "Id.h"
#include "Log.h"

namespace papas {
  
Track::Track(const TVector3 p3, double charge, Path::Ptr path, IdType id) :
m_uniqueId(id),
m_p3(p3),
m_charge(charge),
m_path(path)
{
    Log::debug() << "Made Track   "  << *this;
}
  
std::ostream& operator<<(std::ostream& os, const Track& track) { //TODO move to helper class
    os<<string_format("%10s:%1c%-8d: %7.2f %7.2f %5.2f %5.2f","Track", Id::typeShortCode(track.id()) ,
                    Id::uniqueId(track.id()),   track.energy(), track.pt(),
                    track.theta(), track.p3().Phi());
  //  os << "Track  :" << Id::typeShortCode(track.m_uniqueId)  <<Id::uniqueId(track.m_uniqueId) << ": "<< std::setw(9) << //std::fixed  << track.m_charge << std::endl;
   
    return os;
  }


} // end namespace papas