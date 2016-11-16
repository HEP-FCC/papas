//
//  Created by Alice Robson on 29/11/15.

#ifndef SimParticle_H
#define SimParticle_H

#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Path.h"

namespace papas {

class Track;
/// Simulated particle
/// Inherits from Particle and contains additional information to decribe the path of the particle
class SimParticle : public Particle {
public:
  bool isElectroMagnetic() const;
  SimParticle() = default;
  SimParticle(int pdgid, double charge, const TLorentzVector& tlv,
              const TVector3& vertex = TVector3(0., 0., 0.), double field = 0., char subtype ='s');
  SimParticle(int pdgid, double charge, const TLorentzVector& tlv, const Track& track, char subtype ='r');

  const TVector3& pathPosition(papas::Position layer) const;  ///< returns position of Simparticle at given layer
  void setPath(Path::Ptr path) { m_path = path; }
  bool isHelix() const { return m_isHelix; }
  Path::Ptr path() const { return m_path; }

private:
  TVector3 m_vertex;
  Path::Ptr m_path;
  bool m_isHelix;
};

}  // end namespace papas
#endif  // SimParticle_H
