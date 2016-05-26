#include "track.h"
#include "Id.h"

namespace papas {
  
Track::Track(const TVector3 p3, double charge, Path::Ptr path, IdType id) :
m_uniqueId(id),
m_p3(p3),
m_charge(charge),
m_path(path)
{
}
  
  std::ostream& operator<<(std::ostream& os, const Track& track) { //TODO move to helper class
    os << "Track  :" << Id::typeShortCode(track.m_uniqueId)  <<Id::uniqueId(track.m_uniqueId) << ": "<< std::setw(9) << std::fixed  << track.m_charge << std::endl;
   
    return os;
  }


} // end namespace papas