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

PFParticle::PFParticle(int pdgid, double charge, const TLorentzVector& tlv, unsigned int index, char subtype, const TVector3& vertex, double field)
    : Particle(pdgid, charge, tlv),
      m_uniqueId(Identifier::makeId(index, Identifier::kParticle,  subtype, tlv.E())),
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

PFParticle::PFParticle(int pdgid, double charge, const TLorentzVector& tlv, const Track& track, unsigned int index, char subtype)
    :  // for when particle is created via reconstruct track - it calls the above constructor
      PFParticle(pdgid, charge, tlv,index, subtype, track.path()->namedPoint(papas::Position::kVertex), track.path()->field()) {
  for (const auto& p : track.path()->points()) {  // not sure if this is a good idea but it helps with plotting??
    if (p.first != papas::Position::kVertex) m_path->addPoint(p.first, p.second);
  }
}

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
