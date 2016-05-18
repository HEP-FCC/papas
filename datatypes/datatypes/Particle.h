//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PARTICLE_H
#define PARTICLE_H

#include "TLorentzVector.h"
#include "TVector3.h"

/// Particle
/**
 Contains 4-momentum vector, particle id and accessor functions
 Intended to be used as the base for derived Particle classes
 */

namespace papas {

class Particle {
public:
  Particle();
  Particle(int pdgid, double charge, TLorentzVector tlv, double status = 0);
  Particle(int pdgid, double charge);
  Particle(long id, int pdgid, double charge, TLorentzVector tlv, double status = 0);
  Particle(long id, int pdgid, double charge);
  std::string stringDescription() const;              ///< String to describe the particle
  const TLorentzVector p4() const { return m_tlv; }   ///< 4-momentum, px, py, pz, E
  const TVector3 p3() const { return m_tlv.Vect(); }  ///< 3-momentum px, py, pz
  double e() const { return m_tlv.E(); }              ///<Energy
  double getPt() const { return m_tlv.Pt(); }         ///<transverse momentum (magnitude of p3 in transverse plane)
  double getTheta() const { return M_PI / 2 - m_tlv.Theta(); }  ///< angle w/r to transverse plane
  double eta() const { return m_tlv.Eta(); }                    ///<pseudo-rapidity (-ln(tan self._tlv.Theta()/2)).
  double phi() const { return m_tlv.Phi(); }              ///<azymuthal angle (from x axis, in the transverse plane)
  double getM() const { return m_tlv.M(); }               ///< mass
  int pdgId() const { return m_particleId; }              ///< particle type (an integer value)
  double charge() const { return m_charge; }              ///< particle charge
  bool status() const { return m_status; }                ///<status code, e.g. from generator. 1:stable.
  TVector3 startVertex() const { return m_startVertex; }  ///<start vertex (3d point)
  TVector3 endVertex() const { return m_endVertex; }      ///<end vertex (3d point)
  long id() const { return m_uniqueId; }

protected:
  long m_uniqueId;  // to be used by virtual classes
private:
  TLorentzVector m_tlv;
  int m_particleId;
  double m_charge;
  double m_status;
  TVector3 m_startVertex;
  TVector3 m_endVertex;
};
  
} // end namespace papas

#endif
