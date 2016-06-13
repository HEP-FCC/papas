//
//  Cluster.cpp
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#include "Cluster.h"
#include "Id.h"
#include "Log.h"
#include "StringFormatter.h"

namespace papas {

double Cluster::s_maxEnergy = 0;

Cluster::Cluster(double energy, TVector3 position, double size_m, Id::ItemType idtype)
    : m_uniqueId(Id::makeId(idtype)), m_p3(position), m_subClusters() {
  setSize(size_m);
  setEnergy(energy);
}
  

void Cluster::setSize(double value) {
  m_size = value;
  // AJRTODO put in exception here

  m_angularSize = atan(m_size / m_p3.Mag());
}

void Cluster::setEnergy(double energy) {
  m_energy = energy;
  if (energy > s_maxEnergy) s_maxEnergy = energy;
  m_pt = energy * m_p3.Unit().Perp();
}

// TODO need to change this so that the merged cluster has its ownID and the cluster from which it is made is
// put into subClusters its not rght just yet
Cluster& Cluster::operator+=(const Cluster& rhs) {
  if(Id::pretty(m_uniqueId)=="e299     ")
    std::cout<<*this;
  if (Id::itemType(m_uniqueId) != Id::itemType(rhs.id())) {
    std::cout << "can only add a cluster from the same layer";
  }
  m_p3 = m_p3 * m_energy + rhs.position() * rhs.energy();
  m_energy = m_energy + rhs.energy();
  double denom = 1. / m_energy;
  m_p3 *= denom;
  if (rhs.subClusters().size() > 1) {
     std::cout << "can only add in a cluster which is not already merged";
  }
  //if (m_subClusters.size() == 0) m_subClusters.push_back(m_uniqueId);
  m_subClusters.push_back(rhs.id());
  //m_subClusters.insert(m_subClusters.end(), rhs.subClusters().begin(), rhs.subClusters().end());

  return *this;
}

std::string Cluster::info() const { return string_format("%7.2f %5.2f %5.2f", energy(), theta(), position().Phi()); }

  Cluster::Cluster(const Cluster& c, Id::Type id) :
      m_uniqueId(id),
      m_size(c.m_size),
      m_angularSize(c.m_angularSize),
      m_pt(c.m_pt),
      m_energy(c.m_energy),
      m_subClusters() {
  m_p3 = c.m_p3;
  // std::cout << m_subClusters.size();
        std::cout << Id::pretty(c.id())<< ": "<< Id::pretty(m_uniqueId);
  m_subClusters.push_back(c.id());
}

std::ostream& operator<<(std::ostream& os, const Cluster& cluster) {

  os << "Cluster :" << Id::pretty(cluster.id()) << ": " << cluster.info();
  return os;
}

}  // end namespace papas

/* KEEP these as  they can provide tests to check if any unwanted copying of Clusters is occuring
 Cluster::Cluster( Cluster && c) :
 m_size(c.m_size),
 m_angularsize(c.m_angularsize),
 m_pt(c.m_pt),
 m_uniqueid(c.m_uniqueid),
 m_energy(c.m_energy)
 {
 m_p3=c.m_p3;
 }


 Cluster& Cluster::operator=(Cluster&& c) {
 m_energy=c.m_energy;
 m_p3=c.m_p3;
 m_size=c.m_size;
 m_pt=c.m_pt;
 m_uniqueid=c.m_uniqueid;
 std::cout<< "move assign cluster" <<std::endl;
 return *this;
 };

 Cluster& Cluster::operator=(const Cluster& c) {
 m_energy=c.m_energy;
 m_p3=c.m_p3;
 m_size=c.m_size;
 m_pt=c.m_pt;
 m_uniqueid=c.m_uniqueid;
 std::cout<< "copy cluster" <<std::endl;
 return *this;
 };

 Cluster::Cluster(const Cluster&) {

 std::cout<< "copy" ;
 } ;
*/