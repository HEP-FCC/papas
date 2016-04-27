//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef BASEPARTICLE_H
#define BASEPARTICLE_H

#include <cmath>
#include <string>
#include "TLorentzVector.h"
#include "TVector3.h"

/// BaseParticle
/**
   Contains 4-momentum vector, particle id and accessor functions
   Intended to be used as the base for derived Particle classes
*/
class BaseParticle {
public:
   BaseParticle();
   BaseParticle(int pdgid, double charge, TLorentzVector tlv, double status = 0);
   BaseParticle(int pdgid, double charge);
   BaseParticle(long id, int pdgid, double charge, TLorentzVector tlv,
                double status = 0);
   BaseParticle(long id, int pdgid, double charge);
   std::string StringDescription() const;                ///< String to describe the particle
   const TLorentzVector p4() const {return m_tlv;}    ///< 4-momentum, px, py, pz, E
   const TVector3 p3() const {return m_tlv.Vect();}   ///< 3-momentum px, py, pz
   double e() const {return m_tlv.E();}               ///<Energy
   double getpt() const {return m_tlv.Pt();}             ///<transverse momentum (magnitude of p3 in transverse plane)
   double getTheta() const {return M_PI / 2 - m_tlv.Theta();} ///< angle w/r to transverse plane
   double eta() const {return m_tlv.Eta();}           ///<pseudo-rapidity (-ln(tan self._tlv.Theta()/2)).
   double phi() const {return m_tlv.Phi();}           ///<azymuthal angle (from x axis, in the transverse plane)
   double getM() const {return m_tlv.M();}               ///< mass
   int    pdgId() const {return m_particleId;}        ///< particle type (an integer value)
   double charge() const {return m_charge;}                ///< particle charge
   bool   status() const {return m_status;}           ///<status code, e.g. from generator. 1:stable.
   TVector3 start_vertex() const {return m_start_vertex ;}///<start vertex (3d point)
   TVector3 end_vertex() const {return m_end_vertex;} ///<end vertex (3d point)
protected:
   long m_uniqueId; //to be used by virtual classes
private:
   TLorentzVector m_tlv;
   int m_particleId;
   double m_charge;
   double m_status;
   TVector3 m_start_vertex;
   TVector3 m_end_vertex;
};
#endif
