//
//  Created by Alice Robson on 29/11/15.

#ifndef PFParticle_H
#define PFParticle_H

#include "papas/datatypes/Identifier.h"
#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Path.h"

namespace papas {

class Track;
  
/// Simulated particle
/// Inherits from Particle and contains additional information to decribe the path of the particle
class PFParticle : public Particle {
public:
  /// Constructor
  PFParticle() = default;
  /** Constructor
   @param[in] pdgid particle type
   @param[in] charge charge
   @param[in] tlv 4-momentum, px, py, pz, E
   @param[in] vertex start vertex (3d point)
   @param[in] field Magnetic field
   @param[in] subtype Identifier subtype to be used when creating unique Identifier eg 'r' for reconstructed
   */
  PFParticle(int pdgid, double charge, const TLorentzVector& tlv, const TVector3& vertex = TVector3(0., 0., 0.),
             double field = 0., char subtype = 's');
  /** Constructor
   @param[in] pdgid particle type
   @param[in] charge charge
   @param[in] tlv 4-momentum, px, py, pz, E
   @param[in] track The track to use in creating a particle
   @param[in] subtype Identifier subtype to be used when creating unique Identifier eg 'r' for reconstructed
   */
  PFParticle(int pdgid, double charge, const TLorentzVector& tlv, const Track& track, char subtype = 'r');

  const TVector3& pathPosition(papas::Position layer) const;  ///< returns position of particle path at given layer
  void setPath(Path::Ptr path) { m_path = path; }  ///< Set ponter to path of particle
  bool isHelix() const { return m_isHelix; } ///< Helix or straighline
  Path::Ptr path() const { return m_path; } ///< Return pointer to path
  IdType id() const { return m_uniqueId; }  ///< unique Identifier for object
  bool isElectroMagnetic() const;  ///< Is it electroMagnetic
private:
  IdType m_uniqueId; ///< unique Identifier
  TVector3 m_vertex;  ///< start vertex (3d)
  Path::Ptr m_path; ///< pointer to path object
  bool m_isHelix; ///< Boolean to say if path is Helix or straightline
};

std::ostream& operator<<(std::ostream& os, const PFParticle& particle);

}  // end namespace papas
#endif  // PFParticle_H
