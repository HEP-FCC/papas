#include "papas/datatypes/Particle.h"

#include <iomanip> //lxplus needs this
#include <iostream>

#include "papas/datatypes/IdCoder.h"
#include "spdlog/details/format.h"

namespace papas {

// Particle::Particle() : m_pdgId(0), m_charge(0), m_status(0) {}

Particle::Particle(int pdgid, double charge, const TLorentzVector& tlv, unsigned int index, char subtype,
                   const TVector3& startVertex, double status)
    : m_tlv(tlv),
      m_pdgId(pdgid),
      m_charge(charge),
      m_status(status),
      m_startVertex(startVertex),
      m_id(IdCoder::makeId(index, IdCoder::kParticle, subtype, tlv.E())) {}

bool Particle::isElectroMagnetic() const {
  unsigned int kind = abs(pdgId());
  if (kind == 11 || kind == 22) {
    return true;
  } else
    return false;
};

std::string Particle::info() const {
  fmt::MemoryWriter out;
  int pid = m_pdgId;
  out.write("pdgid = {:5}, status = {:3}, q = {:2}", pid, m_status, m_charge);
  out.write(", e = {:5.1f}, theta = {:5.2f}, phi = {:5.2f}, mass = {:5.2f}", e(), theta(), phi(), fabs(mass()));
  return out.str();
}

std::ostream& operator<<(std::ostream& os, const Particle& particle) {
  os << "Particle :" << std::setw(6) << std::left << IdCoder::pretty(particle.id()) << ":" << particle.id() << ": "
     << particle.info();
  return os;
}

}  // end namespace papas
