
#include "PParticle.h"

//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @brief  Particle from which more complex particles are derived
 */

#include "Definitions.h"
#include "Id.h"
#include "spdlog/details/format.h"
#include <iostream>
#include <sstream>  //AJRTODO temp

namespace papas {

Particle::Particle(unsigned int pdgid, double charge)
    : m_uniqueId(Id::makeParticleId()), m_particleId(pdgid), m_charge(charge), m_status(0) {
  m_tlv = TLorentzVector{0., 0., 0., 0.};
}

Particle::Particle() : m_uniqueId(0), m_particleId(0), m_charge(0), m_status(0) {}

Particle::Particle(IdType id, unsigned int pdgid, double charge)
    : m_uniqueId(id), m_particleId(pdgid), m_charge(charge), m_status(0) {
  m_tlv = TLorentzVector{0., 0., 0., 0.};
}

Particle::Particle(IdType id, unsigned int pdgid, double charge, TLorentzVector tlv, double status)
    : m_uniqueId(id), m_tlv(tlv), m_particleId(pdgid), m_charge(charge), m_status(status) {}

std::string Particle::info() const {
  fmt::MemoryWriter out;
  int pid = m_particleId;
  if (m_charge < 0) pid = -pid;
  out.write("pdgid = {:5}, status = {:3}, q = {:2}", pid, m_status, m_charge);
  out.write(", pt = {:5.1f}, e = {:5.1f}, eta = {:5.2f}, theta = {:5.2f}, phi = {:5.2f}, mass = {:5.2f}", pt(), e(),
            eta(), theta(), phi(), mass());
  return out.str();
}

std::ostream& operator<<(std::ostream& os, const Particle& particle) {
  os << "Particle :" << Id::pretty(particle.id()) << ": " << particle.info();
  return os;
}

}  // end namespace papas