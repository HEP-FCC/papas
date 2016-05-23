//
//  Created by Alice Robson on 09/11/15.
//
//

#include "PFParticle.h"
#include <cmath>
#include <iostream>
#include <utility>

#include "Cluster.h"
#include "Track.h"

#include "Particle.h"
#include "ParticlePData.h"
#include "Path.h"
#include "Helix.h"
#include "Definitions.h"

namespace papas {
  
  PFParticle::PFParticle(Idtype uniqueid, const Track& track) : //TODO check what this is used for
    PFParticle(uniqueid,
            211 * track.charge(),
               TLorentzVector(track.p3(), track.energy()),
            track.path()->namedPoint(papas::Position::kVertex),
            track.path()->field()) {
}

  PFParticle::PFParticle(Idtype uniqueid, int pdgid, TLorentzVector tlv, TVector3 vertex, double field) :
  Particle(uniqueid, pdgid, ParticlePData::particleCharge(pdgid), tlv),
  m_vertex(vertex),
  m_isHelix(fabs(charge())>0.5)
{
  if (m_isHelix)
    m_path = std::make_shared<Helix>(tlv, vertex, field, charge());
  else
    m_path = std::make_shared<Path>(tlv, vertex, field);
  
  
}
/*
void PFParticle::setHelix(const Path& path) {
  m_helix = path; //copy??
  }

void PFParticle::setPath(const Path& path) {
  m_path = path;
}*/


TVector3 PFParticle::pathPosition(papas::Position layer) const
{
  return m_path->namedPoint(layer);
}




/*PFParticle::PFParticle( TLorentzVector& tlv, TVector3& vertex, double charge, int pdgid):
 Particle(pdgid, charge, tlv), m_vertex(vertex) {
 self.path = None
 self.clusters = dict()
 self.track = Track(self.p3(), self.q(), selfget.path)
 self.clusters_smeared = dict()
 self.track_smeared = None
 }*/

bool PFParticle::isElectroMagnetic() const
{
  unsigned int kind = abs(pdgId());
  if (kind == 11 || kind == 22) {
    return true;
  } else
    return false;
  
};
  
} // end namespace papas
