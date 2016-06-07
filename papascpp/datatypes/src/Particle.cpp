
#include "BaseParticle.h"

//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @brief  Particle from which more complex particles are derived
 */


#include <iostream>
#include <sstream> //AJRTODO temp
#include "Definitions.h"
#include "Id.h"
#include "spdlog/details/format.h"
//#include "easylogging.h"

namespace papas {

std::string Particle::info() const
{
  
  fmt::MemoryWriter out;
  out.write("pdgid = {:5}, status = {:3}, q = {:2}",m_particleId, m_status,m_charge);
  out.write(", pt = {:5.1f}, e = {:5.1f}, eta = {:5.2f}, theta = {:5.2f}, phi = {:5.2f}, mass = {:5.2f}",
            pt(), e(), eta(), theta(), phi(), mass());
  return out.str();
  
}
  
std::ostream& operator<<(std::ostream& os, const Particle& particle) {
    os<< "Particle :"<< Id::pretty(particle.id())<<": "<<particle.info();
    return os;
}




Particle::Particle(int pdgid, double charge, TLorentzVector tlv,
                           double status) :
m_uniqueId(Id::makeParticleId(papas::enumSource::NONE)),
m_tlv(tlv), m_particleId(pdgid), m_charge(charge), m_status(status)

{
  
}


Particle::Particle(int pdgid, double charge) :
  m_uniqueId(Id::makeParticleId(papas::enumSource::NONE)),
  m_particleId(pdgid),
  m_charge(charge),
  m_status(0)
{
  m_tlv = TLorentzVector{0., 0., 0., 0.};
  ;
  //LOG(INFO)<< "Made a particle";

}


Particle::Particle() :
  m_uniqueId(Id::makeParticleId(papas::enumSource::NONE)),
  m_particleId(0), m_charge(0), m_status(0)
{
  //LOG(INFO)<< "Made a particle";

}

Particle::Particle(IdType id, int pdgid, double charge) :
  m_uniqueId(id), m_particleId(pdgid), m_charge(charge), m_status(0)
{
  m_tlv = TLorentzVector{0., 0., 0., 0.};
  //LOG(INFO)<< "Made a particle";

}


Particle::Particle(IdType id, int pdgid, double charge, TLorentzVector tlv, double status) :
  m_uniqueId(id),
  m_tlv(tlv),
  m_particleId(pdgid),
  m_charge(charge),
  m_status(status)
{
  //LOG(INFO)<< "Made a particle";

}
  
  } // end namespace papas

