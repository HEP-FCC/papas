//
//  Created by Alice Robson on 09/11/15.
//
//

#include "papas/datatypes/PFParticle.h"
#include <cmath>
#include <iostream>
#include <utility>

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Track.h"

#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/Helix.h"
#include "papas/datatypes/Particle.h"
#include "papas/datatypes/ParticlePData.h"
#include "papas/datatypes/Path.h"

namespace papas {

/*PFParticle::PFParticle(IdType uniqueid, int pdgid, TLorentzVector tlv, TVector3 vertex, double field) :
Particle(uniqueid, pdgid, ParticlePData::particleCharge(pdgid), tlv),
m_vertex(vertex),
m_isHelix(fabs(charge())>0.5)
{
if (m_isHelix)
  m_path = std::make_shared<Helix>(tlv, vertex, field, charge());
else
  m_path = std::make_shared<Path>(tlv, vertex, field);


}*/

PFParticle::PFParticle(int pdgid, double charge, const TLorentzVector& tlv, const TVector3& vertex, double field,
                       char subtype)
    : Particle(pdgid, charge, tlv),
      m_uniqueId(Identifier::makeId(Identifier::kParticle, subtype, tlv.E())),
      m_vertex(vertex),
      m_isHelix(fabs(charge) > 0.5) {

  if (m_isHelix)
    if (field > 0)
      m_path = std::make_shared<Helix>(tlv, vertex, field, charge);
    else
      throw "Non zero field required for Charged particle";
  else
    m_path = std::make_shared<Path>(tlv, vertex, field);
}

PFParticle::PFParticle(int pdgid, double charge, const TLorentzVector& tlv, const Track& track, char subtype)
    :  // for when particle is created via reconstruct track - it calls the above constructor
      PFParticle(pdgid, charge, tlv, track.path()->namedPoint(papas::Position::kVertex), track.path()->field(),
                 subtype) {

  for (const auto& p : track.path()->points()) {  // not sure if this is a good idea but it helps with plotting??
    if (p.first != papas::Position::kVertex) m_path->addPoint(p.first, p.second);
  }
}
/*
void PFParticle::setHelix(const Path& path) {
  m_helix = path; //copy??
  }

void PFParticle::setPath(const Path& path) {
  m_path = path;
}*/

const TVector3& PFParticle::pathPosition(papas::Position layer) const { return m_path->namedPoint(layer); }

/*PFParticle::PFParticle( TLorentzVector& tlv, TVector3& vertex, double charge, int pdgid):
 Particle(pdgid, charge, tlv), m_vertex(vertex) {
 self.path = None
 self.clusters = dict()
 self.track = Track(self.p3(), self.q(), selfget.path)
 self.clusters_smeared = dict()
 self.track_smeared = None
 }*/

bool PFParticle::isElectroMagnetic() const {
  unsigned int kind = abs(pdgId());
  if (kind == 11 || kind == 22) {
    return true;
  } else
    return false;
};

std::ostream& operator<<(std::ostream& os, const PFParticle& particle) {
  os << "Particle :" << std::setw(6) << std::left << Identifier::pretty(particle.id()) << ":" << particle.id() << ": "
     << particle.info();
  return os;
}

}  // end namespace papas
