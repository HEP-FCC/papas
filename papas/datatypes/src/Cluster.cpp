//
//  Cluster.cpp
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Identifier.h"
#include "papas/utility/PDebug.h"

namespace papas {

double Cluster::s_maxEnergy = 0;

Cluster::Cluster(double energy, const TVector3& position, double size_m, Identifier::ItemType idtype, char subtype)
    : m_uniqueId(Identifier::makeId(idtype, subtype, energy)), m_p3(position), m_subClusters() {
  setSize(size_m);
  setEnergy(energy);
  m_subClusters.push_back(this);
}

  Cluster::Cluster(const Cluster& c, Identifier::ItemType type, char subtype, float val)
    : m_uniqueId(Identifier::makeId(type, subtype, val)),
      m_size(c.m_size),
      m_angularSize(c.m_angularSize),
      m_pt(c.m_pt),
      m_energy(c.m_energy),
      m_subClusters() {
  m_p3 = c.m_p3;
  m_subClusters.push_back(&c);
}

Cluster::Cluster(Cluster&& c)
    : m_uniqueId(c.id()),
      m_size(c.m_size),
      m_angularSize(c.m_angularSize),
      m_pt(c.m_pt),
      m_energy(c.m_energy),
      m_subClusters() {
  m_p3 = c.m_p3;

  // Moving a Cluster is a little tricky because must make sure that
  // the subclusters are pointing to something that has already been moved
  // This is a disadvantage of using Cluster class to deal with both
  // "cluster" and "mergedcluster" and it may infact be better to have the
  // subclusters empty for a non-merged cluster
  // For a non merged cluster the subcluster points to itself.
  if (c.subClusters().size() == 1 && c.id() == c.subClusters()[0]->id())
    m_subClusters.push_back(this);  // non merged cluster point to itself
  else
    for (auto s : c.subClusters())  // merged clusters
      m_subClusters.push_back(s);
}

void Cluster::setSize(double value) {
  m_size = value;
  if (m_p3.Mag() == 0) throw "Undefined angularsize for zero momentum";
  m_angularSize = atan(m_size / m_p3.Mag());
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
  m_pt = energy * m_p3.Unit().Perp();
}

Cluster& Cluster::operator+=(const Cluster& rhs) {
  // if(Identifier::pretty(m_uniqueId)=="e299     ")
  //  std::cout<<*this;
  if (Identifier::itemType(m_uniqueId) != Identifier::itemType(rhs.id())) {
    std::cout << "can only add a cluster from the same layer";
  }
  m_p3 = m_p3 * m_energy + rhs.position() * rhs.energy();
  m_energy = m_energy + rhs.energy();
  if (m_energy > s_maxEnergy) s_maxEnergy = m_energy;  // ajr not sure if this is needed at all?
  double denom = 1. / m_energy;
  m_p3 *= denom;
  if (rhs.subClusters().size() > 1) {
    std::cout << "can only add in a cluster which is not already merged";
  }
  m_pt = m_energy * m_p3.Unit().Perp();
  m_subClusters.push_back(&rhs);
  return *this;
}

std::string Cluster::info() const { return string_format("%7.2f %5.2f %5.2f", energy(), theta(), position().Phi()); }

std::ostream& operator<<(std::ostream& os, const Cluster& cluster) {
  os << "Cluster :" << Identifier::pretty(cluster.id()) << ":" << cluster.id() << ": " << cluster.info();
  os << " sub(";
  if (cluster.subClusters().size() == 0) std::cout << "hmmm";
  for (auto c : cluster.subClusters()) {
    os << Identifier::pretty(c->id()) << ", ";
  }
  os << ")";
  return os;
}

/*//KEEP these as  they can provide tests to check if any unwanted copying of Clusters is occuring
Cluster::Cluster( Cluster && c) :
m_size(c.m_size),
m_angularSize(c.m_angularSize),
m_pt(c.m_pt),
m_uniqueId(c.m_uniqueId),
m_energy(c.m_energy),
m_subClusters(std::move(c.m_subClusters))

{
m_p3=c.m_p3;
    PDebug::write("Move cluster {}" , *this);
 std::cout<< "Move Cluster" <<std::endl;
}*/

/* Cluster& Cluster::operator=(Cluster&& c) {
 m_energy=c.m_energy;
 m_p3=c.m_p3;
 m_size=c.m_size;
 m_pt=c.m_pt;
 m_uniqueId=c.m_uniqueId;
 std::cout<< "move assign cluster" <<std::endl;
 return *this;
 };*/

/*Cluster& Cluster::operator=(const Cluster& c) {
m_energy=c.m_energy;
m_p3=c.m_p3;
m_size=c.m_size;
m_pt=c.m_pt;
m_uniqueId=c.m_uniqueId;
std::cout<< "copy cluster=" <<std::endl;
return *this;
};

Cluster::Cluster(const Cluster&) {
  PDebug::write("copy cluster {}" , Identifier::pretty(m_uniqueId));
std::cout<< "copy cluster" ;
} ;*/

/*Cluster::~Cluster() {
  PDebug::write("delete cluster {}" , Identifier::pretty(m_uniqueId));
  std::cout<< "delete cluster" ;
} ;*/

}  // end namespace papas
