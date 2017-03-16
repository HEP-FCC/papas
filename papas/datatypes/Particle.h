//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PARTICLE_H
#define PARTICLE_H

#include "TLorentzVector.h"
#include "TVector3.h"
#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/Path.h"
#include "papas/utility/PDebug.h"
class Track;

namespace papas {
/// Main Particle class
///
/// Contains 4-momentum vector, particle id and accessor functions
/// Intended to be used as the base for derived Particle classes

class Particle {
public:
  
  
  /// Constructor
  Particle() = default;
  /** Constructor
   @param[in] pdgid particle type
   @param[in] charge charge
   @param[in] tlv 4-momentum, px, py, pz, E
   @param[in] index index to the collection to which thie particle will belown
   @param[in] subtype Identifier subtype to be used when creating Identifier eg 'r' for reconstructed
   @param[in] status Status (1 = stable)
   @param[in] startvertex start vertex (3d point)
   @param[in] endvertex end vertex (3d point)
   */
  Particle(int pdgid, double charge, const TLorentzVector& tlv, unsigned int index, char subtype,  const TVector3& startvertex = TVector3(0., 0., 0.), const TVector3& endvertex = TVector3(0., 0., 0.) ,double status = 1);
  const TLorentzVector& p4() const { return m_tlv; }   ///< 4-momentum, px, py, pz, E
  TVector3 p3() const { return m_tlv.Vect(); }  ///< 3-momentum px, py, pz,
  double e() const { return m_tlv.E(); }               ///<Energy
  double pt() const { return m_tlv.Pt(); }             ///<transverse momentum (magnitude of p3 in transverse plane)
  double theta() const { return M_PI / 2 - m_tlv.Theta(); }  ///< angle w/r to transverse plane
  
  /**pseudo-rapidity (-ln(tan self._tlv.Theta()/2)).
  theta = 0 -> eta = +inf
  theta = pi/2 -> 0
  theta = pi -> eta = -inf*/
  double eta() const { return m_tlv.Eta(); }                 
  double phi() const { return m_tlv.Phi(); }                 ///<azymuthal angle (from x axis, in the transverse plane)
  double mass() const { return m_tlv.M(); }                  ///< mass
  int pdgId() const { return m_pdgId; }                      ///< particle type (an integer value)
  double charge() const { return m_charge; }                 ///< particle charge
  bool status() const { return m_status; }                   ///<status code, e.g. from generator. 1:stable.
  const TVector3& startVertex() const { return m_startVertex; }  ///<start vertex (3d point)
  const TVector3& endVertex() const { return m_endVertex; }      ///<end vertex (3d point)
  std::string info() const; ///< text descriptor of the particle
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
  void setPath(Path::Ptr path) { m_path = path;}
  const Path::Ptr path() const { return m_path; } ///< Return pointer to path
  IdType id() const { return m_uniqueId; }  ///< unique Identifier for object
  bool isElectroMagnetic() const;  ///< Is it electroMagnetic
private:
  TLorentzVector m_tlv;    ///<4-momentum, px, py, pz, E
  int m_pdgId;             ///<particle type
  double m_charge;         ///<particle charge
  double m_status;         ///< status code, e.g. from generator. 1:stable
  TVector3 m_startVertex;  ///<start vertex (3d point)
  TVector3 m_endVertex;    ///<end vertex (3d point)
  IdType m_uniqueId; ///< unique Identifier
  TVector3 m_vertex;  ///< start vertex (3d)
  Path::Ptr m_path; ///< pointer to path object
  bool m_isHelix; ///< Boolean to say if path is Helix or straightline
};

std::ostream& operator<<(std::ostream& os, const Particle& particle);

}  // end namespace papas

#endif