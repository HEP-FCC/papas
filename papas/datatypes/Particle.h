//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PARTICLE_H
#define PARTICLE_H

#include "papas/datatypes/Definitions.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "papas/utility/PDebug.h"

namespace papas {
/// Main Particle class
///
/// Contains 4-momentum vector, particle id and accessor functions
/// Intended to be used as the base for derived Particle classes

class Particle {
public:
  Particle();
  // Particle& operator=(const Particle& P) = default;
  Particle(int pdgid, double charge, const TLorentzVector& tlv,   double status = 1,const TVector3& startVector =TVector3(0,0,0), const TVector3& endVertex=TVector3(0,0,0));
  Particle(int pdgid, double charge);
  //Particle(IdType id, int pdgid, double charge, const TLorentzVector& tlv, double status = 1);
  //Particle(IdType id, int pdgid, double charge);

  std::string stringDescription() const;              ///< String to describe the particle
  const TLorentzVector& p4() const { return m_tlv; }  ///< 4-momentum, px, py, pz, E
  TVector3 p3() const { return m_tlv.Vect(); }  ///< 3-momentum px, py, pz, returning a local object so is not const &
  double e() const { return m_tlv.E(); }        ///<Energy
  double pt() const { return m_tlv.Pt(); }      ///<transverse momentum (magnitude of p3 in transverse plane)
  double theta() const { return M_PI / 2 - m_tlv.Theta(); }  ///< angle w/r to transverse plane
  double eta() const { return m_tlv.Eta(); }                 ///<pseudo-rapidity (-ln(tan self._tlv.Theta()/2)).
  double phi() const { return m_tlv.Phi(); }                 ///<azymuthal angle (from x axis, in the transverse plane)
  double mass() const { return m_tlv.M(); }                  ///< mass
  int pdgId() const { return m_particleId; }                 ///< particle type (an integer value)
  double charge() const { return m_charge; }                 ///< particle charge
  bool status() const { return m_status; }                   ///<status code, e.g. from generator. 1:stable.
  const TVector3& startVertex() const { return m_startVertex; }  ///<start vertex (3d point)
  const TVector3& endVertex() const { return m_endVertex; }      ///<end vertex (3d point)
    std::string info() const;  ///< text descriptor of the particle

private:
  TLorentzVector m_tlv;
  int m_particleId;
  double m_charge;
  double m_status;
  TVector3 m_startVertex;
  TVector3 m_endVertex;
};


}  // end namespace papas

#endif
