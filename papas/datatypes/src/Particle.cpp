
#include "papas/datatypes/Particle.h"

//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @brief  Particle from which more complex particles are derived
 */

#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/Identifier.h"
#include "spdlog/details/format.h"
#include <iostream>
#include <sstream>  //AJRTODO temp

namespace papas {

Particle::Particle() : m_pdgId(0), m_charge(0), m_status(0) {}

Particle::Particle(int pdgid, double charge, const TLorentzVector& tlv, double status, const TVector3& startVertex,
                   const TVector3& endVertex)
    : m_tlv(tlv),
      m_pdgId(pdgid),
      m_charge(charge),
      m_status(status),
      m_startVertex(startVertex),
      m_endVertex(endVertex) {}

std::string Particle::info() const {
  fmt::MemoryWriter out;
  int pid = m_pdgId;
  //if (m_charge < 0) pid = -abs(pid);
  //if (m_charge == 0 && pid < 0) pid = -pid;
  out.write("pdgid = {:5}, status = {:3}, q = {:2}", pid, m_status, m_charge);
  out.write(", e = {:5.1f}, theta = {:5.2f}, phi = {:5.2f}, mass = {:5.2f}", e(), theta(), phi(), fabs(mass()));
  return out.str();
}

std::ostream& operator<<(std::ostream& os, const Particle& particle) {
  os << "Particle :" << ":" << particle.info();
  return os;
}

}  // end namespace papas
