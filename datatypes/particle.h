//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include "baseparticle.h"

//TOD porbably get rid of this class layer
class Particle: public BaseParticle {
public:
   using BaseParticle::BaseParticle;
   long getID() const
   {
      return m_uniqueid;
   }
   //Particle(int pdgid, double charge, TLorentzVector&& tlv, double status = 1.0);
   //Particle(int pdgid, double charge, TLorentzVector& tlv, double status = 1.0);

   //Particle(string,double charge, TLorentzVector tlv);
private:
};

#endif