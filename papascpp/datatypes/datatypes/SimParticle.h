//
//  Created by Alice Robson on 29/11/15.

#ifndef SimParticle_H
#define SimParticle_H

#include "PParticle.h"
#include "Path.h"


namespace papas {

class Track;

class SimParticle : public Particle {
public:
  bool isElectroMagnetic() const;
  SimParticle() = default;
  //SimParticle(IdType uniqueid, int pdgid, TLorentzVector tlv, TVector3 vertex = TVector3(0., 0., 0.), double field = 0.);
  SimParticle(IdType uniqueid, unsigned int pdgid, double charge, TLorentzVector tlv, TVector3 vertex = TVector3(0., 0., 0.), double field = 0.);
  SimParticle(IdType uniqueid, unsigned int pdgid, double charge, TLorentzVector tlv, const Track& track);
  Path::Ptr path() const { return m_path; }
  TVector3 pathPosition(papas::Position layer) const;
  void setPath(Path::Ptr path) { m_path = path; }
  bool isHelix() const { return m_isHelix; }

private:
  TVector3 m_vertex;
  Path::Ptr m_path;
  bool m_isHelix;
};
  
  

} // end namespace papas
#endif  // SimParticle_H

