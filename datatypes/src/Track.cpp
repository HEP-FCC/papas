#include "track.h"
#include "path.h"


Track::Track(const TVector3 p3, double charge, Path::Ptr path, long id) :
m_uniqueId(id),
m_p3(p3),
m_charge(charge),
m_path(path)
{
}


