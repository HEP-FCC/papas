
#include "papas/datatypes/Particle.h"

//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @brief  Particle from which more complex particles are derived
 */

#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/IdCoder.h"
#include "papas/datatypes/Helix.h"
#include "spdlog/details/format.h"
#include <iostream>
//#include <sstream>  //AJRTODO temp

namespace papas {

// Particle::Particle() : m_pdgId(0), m_charge(0), m_status(0) {}

Particle::Particle(int pdgid, double charge, const TLorentzVector& tlv, unsigned int index, char subtype,
                    const TVector3& startVertex,const TVector3& endVertex, double status)
    : m_tlv(tlv),
      m_pdgId(pdgid),
      m_charge(charge),
      m_status(status),
      m_startVertex(startVertex),
      m_endVertex(endVertex),
      m_id(IdCoder::makeId(index, IdCoder::kParticle, subtype, tlv.E()))
      {
      
  /*if (charge!=0)
      m_path = std::make_shared<Helix>(tlv, m_startVertex, m_charge);
  else
      m_path = std::make_shared<Path>(tlv, m_startVertex);
      }*/
      }

/*Particle::Particle(int pdgid, double charge, const TLorentzVector& tlv, const Track& track, unsigned int index,
                       char subtype)
    :  // for when particle is created via reconstruct track - it calls the above constructor
      Particle(pdgid, charge, tlv, index, subtype, track.path()->namedPoint(papas::Position::kVertex),
                 track.path()->field()) {
  for (const auto& p : track.path()->points()) {  // not sure if this is a good idea but it helps with plotting??
    if (p.first != papas::Position::kVertex) m_path->addPoint(p.first, p.second);
  }
}*/

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