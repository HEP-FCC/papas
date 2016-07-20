//
//  Created by Alice Robson on 09/11/15.
//
//

#include "SimParticle.h"
#include <cmath>
#include <iostream>
#include <utility>

#include "Cluster.h"
#include "pTrack.h"

#include "Definitions.h"
#include "Helix.h"
#include "PParticle.h"
#include "ParticlePData.h"
#include "Path.h"

namespace papas {

/*SimParticle::SimParticle(IdType uniqueid, int pdgid, TLorentzVector tlv, TVector3 vertex, double field) :
Particle(uniqueid, pdgid, ParticlePData::particleCharge(pdgid), tlv),
m_vertex(vertex),
m_isHelix(fabs(charge())>0.5)
{
if (m_isHelix)
  m_path = std::make_shared<Helix>(tlv, vertex, field, charge());
else
  m_path = std::make_shared<Path>(tlv, vertex, field);


}*/

SimParticle::SimParticle(IdType uniqueid, int pdgid, double charge, TLorentzVector tlv, TVector3 vertex, double field)
    : Particle(uniqueid, pdgid, charge, tlv), m_vertex(vertex), m_isHelix(fabs(charge) > 0.5) {
      
  if (m_isHelix )
    if (field > 0)
      m_path = std::make_shared<Helix>(tlv, vertex, field, charge);
    else
      throw "Non zero field required for Charged particle";
  else
    m_path = std::make_shared<Path>(tlv, vertex, field);
}

SimParticle::SimParticle(IdType uniqueid, int pdgid, double charge, TLorentzVector tlv, const Track& track)
    :  // for when particle is created via reconstruct track - it calls the above constructor
      SimParticle(uniqueid, pdgid, charge, tlv, track.path()->namedPoint(papas::Position::kVertex),
                  track.path()->field()) {
  
  for (const auto& p : track.path()->points()) {  // not sure if this is a good idea but it helps with plotting??
    if (p.first != papas::Position::kVertex) m_path->addPoint(p.first, p.second);
  }
}
/*
void SimParticle::setHelix(const Path& path) {
  m_helix = path; //copy??
  }

void SimParticle::setPath(const Path& path) {
  m_path = path;
}*/

TVector3 SimParticle::pathPosition(papas::Position layer) const { return m_path->namedPoint(layer); }

/*SimParticle::SimParticle( TLorentzVector& tlv, TVector3& vertex, double charge, int pdgid):
 Particle(pdgid, charge, tlv), m_vertex(vertex) {
 self.path = None
 self.clusters = dict()
 self.track = Track(self.p3(), self.q(), selfget.path)
 self.clusters_smeared = dict()
 self.track_smeared = None
 }*/

bool SimParticle::isElectroMagnetic() const {
  unsigned int kind = abs(pdgId());
  if (kind == 11 || kind == 22) {
    return true;
  } else
    return false;
};

}  // end namespace papas