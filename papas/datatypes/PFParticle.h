//
//  Created by Alice Robson on 29/11/15.

#ifndef PFParticle_H
#define PFParticle_H

#include "papas/datatypes/IdCoder.h"
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
   @param[in] index index to the collection to which thie particle will belown
   @param[in] field Magnetic field
   @param[in] subtype IdCoder subtype to be used when creating unique IdCoder eg 'r' for reconstructed
   */
  PFParticle(int pdgid, double charge, const TLorentzVector& tlv, unsigned int index, char subtype = 's', const TVector3& vertex = TVector3(0., 0., 0.), double field = 0. );
  /** Constructor
   @param[in] pdgid particle type
   @param[in] charge charge
   @param[in] tlv 4-momentum, px, py, pz, E
   @param[in] track The track to use in creating a particle
   @param[in] index index to the collection to which thie particle will belown
   @param[in] subtype IdCoder subtype to be used when creating unique IdCoder eg 'r' for reconstructed
   */
  PFParticle(int pdgid, double charge, const TLorentzVector& tlv, const Track& track, unsigned int index , char subtype = 'r');
  /** check id this position exists in particle path 
   @param[in] layer position to search for the path location
   */
  bool hasNamedPoint(papas::Position layer) const {return m_path->hasNamedPoint(layer);}
  /** returns position of particle path at given layer
  @param[in] layer position to search for the path location
  */
  const TVector3& pathPosition(papas::Position layer) const {return m_path->namedPoint(layer);}
  //void setPath(Path::Ptr path) { m_path = path; }  ///< Set ponter to path of particle
  bool isHelix() const { return m_isHelix; } ///< Helix or straighline
  const Path::Ptr path() const { return m_path; } ///< Return pointer to path
  IdType id() const { return m_uniqueId; }  ///< unique IdCoder for object
  bool isElectroMagnetic() const;  ///< Is it electroMagnetic
  const TVector3& vertex() const { return m_vertex; }
private:
  IdType m_uniqueId; ///< unique IdCoder
  TVector3 m_vertex;  ///< start vertex (3d)
  Path::Ptr m_path; ///< pointer to path object
  bool m_isHelix; ///< Boolean to say if path is Helix or straightline
};

std::ostream& operator<<(std::ostream& os, const PFParticle& particle);

}  // end namespace papas
#endif  // PFParticle_H
