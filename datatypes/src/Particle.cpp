
#include "Particle.h"

//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @brief  Particle from which more complex particles are derived
 */


#include <iostream>
#include <sstream> //AJRTODO temp
#include "enums.h"
#include "Id.h"



std::string Particle::stringDescription() const
{
  std::ostringstream oss;
  
  oss << (std::string)typeid(this).name();
  oss << " : pdgid = " << m_particleId; //AJRTODO  make length 5
  oss << " status = " << m_status; //AJRTODO  make length 3
  oss << " q = " << m_charge; //AJRTODO  make length 3
  oss << " status = " << m_status; //AJRTODO  make length 3
                                   //todo continue oss<<" status = " << status; //AJRTODO  make length 3
  return oss.str();
}



Particle::Particle(int pdgid, double charge, TLorentzVector tlv,
                           double status) :
m_uniqueId(Id::makeParticleId(fastsim::enumSource::NONE)),
m_tlv(tlv), m_particleId(pdgid), m_charge(charge), m_status(status)

{
  
  /*std::cout << "BASE " << tlv.X() << " " << tlv.Y() << " " << tlv.Z() << " " <<
   tlv.Et() << " ";
   std::cout << m_tlv.X() << " " << m_tlv.Y() << " " << m_tlv.Z() << " " <<
   m_tlv.Et() << " ";*/
}


Particle::Particle(int pdgid, double charge) :
  m_uniqueId(Id::makeParticleId(fastsim::enumSource::NONE)),
  m_particleId(pdgid),
  m_charge(charge),
  m_status(0)
{
  m_tlv = TLorentzVector{0., 0., 0., 0.};
  ;
}


Particle::Particle() :
  m_uniqueId(Id::makeParticleId(fastsim::enumSource::NONE)),
  m_particleId(0), m_charge(0), m_status(0)
{

}

Particle::Particle(long id, int pdgid, double charge) :
  m_uniqueId(id), m_particleId(pdgid), m_charge(charge), m_status(0)
{
  m_tlv = TLorentzVector{0., 0., 0., 0.};

}


Particle::Particle(long id, int pdgid, double charge, TLorentzVector tlv, double status) :
  m_uniqueId(id),
  m_tlv(tlv),
  m_particleId(pdgid),
  m_charge(charge),
  m_status(status)
{

}

