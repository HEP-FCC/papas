//
//  Cluster.cpp
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/IdCoder.h"
#include "papas/utility/StringFormatter.h"
#include <iomanip>

namespace papas {

double Cluster::s_maxEnergy = 0;

Cluster::Cluster(double energy, const TVector3& position, double size_m, unsigned int index, IdCoder::ItemType type,
                 char subtype)
    : m_id(IdCoder::makeId(index, type, subtype, fmax(0, energy))), m_position(position), m_subClusters({this}) {
  setSize(size_m);
  setEnergy(energy);
}

Cluster::Cluster(const Cluster& c, unsigned int index, IdCoder::ItemType type, char subtype, float val)
    : m_id(IdCoder::makeId(index, type, subtype, val)),
      m_size(c.m_size),
      m_angularSize(c.m_angularSize),
      m_position(c.m_position),
      m_energy(c.m_energy),
      m_subClusters({&c}) {}

Cluster::Cluster(std::list<const Cluster*> overlappingClusters, unsigned int index, char subtype)
    : m_subClusters(overlappingClusters) {
  Identifier firstId = 0;
  char type;
  for (const auto* cluster : overlappingClusters) {
    if (cluster->subClusters().size() > 1) {
      throw "can only merge clusters which are not already merged";
    }
    if (firstId == 0) {
      firstId = cluster->id();
      m_position = cluster->position() * cluster->energy();  // will be scaled by total energy at end
      m_energy = cluster->energy();
      m_size = cluster->size();  // needed for the case where overlappingClusters has just one cluster
      m_angularSize = cluster->angularSize();
    } else {
      if (IdCoder::typeAndSubtype(firstId) != IdCoder::typeAndSubtype(cluster->id()))
        throw "Merged Clusters must be made of clusters of same type and subtype";
      m_position += cluster->position() * cluster->energy();  // energy weighted position
      m_energy += cluster->energy();
    }
  }
  if (m_energy > s_maxEnergy) s_maxEnergy = m_energy;  // used for graphics
  double denom = 1. / m_energy;
  m_position *= denom;
  m_id = IdCoder::makeId(index, IdCoder::type(firstId), subtype, m_energy);
}

Cluster::Cluster(Cluster&& c)
    : m_id(c.id()),
      m_size(c.m_size),
      m_angularSize(c.m_angularSize),
      m_position(c.m_position),
      m_energy(c.m_energy),
      m_subClusters(c.m_subClusters) {}

void Cluster::setSize(double value) {
  m_size = value;
  if (m_position.Mag() == 0) throw "Undefined angularsize";
  m_angularSize = atan(m_size / m_position.Mag());
}

double Cluster::angularSize() const {
  if (m_subClusters.size() < 2) {
    return m_angularSize;
  } else
    throw "angularSize is not a valid measurement for a merged cluster";
}
double Cluster::size() const {
  if (m_subClusters.size() < 2) {
    return m_size;
  } else
    throw "size is not a valid measurement for a merged cluster";
}
void Cluster::setEnergy(double energy) {
  m_energy = energy;
  if (energy > s_maxEnergy) s_maxEnergy = energy;
}

std::string Cluster::info() const { return string_format("%7.2f %5.2f %5.2f", energy(), theta(), position().Phi()); }

std::ostream& operator<<(std::ostream& os, const Cluster& cluster) {
  os << "Cluster: " << std::setw(6) << std::left << IdCoder::pretty(cluster.id()) << ":" << cluster.id() << ": "
     << cluster.info();
  os << " sub(";
  for (const auto& c : cluster.subClusters()) {
    os << IdCoder::pretty(c->id()) << ", ";
  }
  os << ")";
  return os;
}

}  // end namespace papas  