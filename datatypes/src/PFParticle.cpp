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
#include "Id.h"
//#include "deltar.h"
//#include "distance.h"
#include "particle.h"
#include "particledata.h"


const double ParticleData::m_e = 0.000511;
const double ParticleData::m_mu = 0.105;
const double ParticleData::m_pi = 0.139;
const double ParticleData::m_K0 = 0.498;
const double ParticleData::m_n = 1.;
const double ParticleData::m_p = 1.;

std::unordered_map<int, std::pair<double, int>> ParticleData::m_datamap =  {
  {11,  {ParticleData::m_e,   1}},
  {-11, {ParticleData::m_e,  -1}},
  {13,  {ParticleData::m_mu,  1}},
  {-13, {ParticleData::m_mu, -1}},
  {22,  {0,                   0}},
  {130, {ParticleData::m_K0,  0}},
  {211, {ParticleData::m_pi,  1}},
  {-211,{ParticleData::m_pi, -1}}
}  ;






PFParticle::PFParticle(long uniqueid,const Track& track) :
PFParticle(uniqueid,
            211 * track.charge(),
            TLorentzVector(track.p3(), track.energy()),
            track.path()->namedPoint("vertex"),
            track.path()->field()) {
}
/*
void PFParticle::setHelix(const Path& path) {
  m_helix = path; //copy??
  }

void PFParticle::setPath(const Path& path) {
  m_path = path;
}*/


TVector3 PFParticle::pathPosition(std::string name) const
{
  return m_path->namedPoint(name);
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
