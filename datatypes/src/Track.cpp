#include "track.h"

#include "TVector3.h"
#include "PFParticle.h"
#include "particle.h"
#include "particledata.h"
#include "path.h"


class TLorentzVector;
typedef std::shared_ptr<Path> sptrPath; ///shared pointer to allow for striaghtline or helix


/*Track::Track(const TVector3 p3, double charge,const Path& path, long id) :
 m_uniqueId(id),
 m_p3(p3),
 m_charge(charge),
 m_path(path)
 {
 }*/


Track::Track(const TVector3 p3, double charge, sptrPath path, long id) :
m_uniqueId(id),
m_p3(p3),
m_charge(charge),
m_path(path)
{
}

PFParticle::PFParticle(long uniqueid,int pdgid, TLorentzVector tlv, TVector3 vertex, double field) :
Particle(uniqueid, pdgid, ParticleData::particleCharge(pdgid), tlv),
m_vertex(vertex),
m_isHelix(fabs(charge())>0.5)
{
  if (m_isHelix)
    m_path = std::make_shared<Helix>(tlv, vertex, field, charge());
  else
    m_path = std::make_shared<Path>(tlv, vertex, field);
  
  
}
