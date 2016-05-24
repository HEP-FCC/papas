//
//  Cluster.cpp
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#include "Cluster.h"
#include "Id.h"

namespace papas {

double Cluster::s_maxEnergy = 0;

Cluster::Cluster(double energy, TVector3 position, double size_m, Idtype id) :
m_uniqueId(id),  m_position(position), m_subClusters()
{
  setSize(size_m);
  setEnergy(energy);
}

void Cluster::setSize(double value)
{
  m_size = value;
  //AJRTODO put in exception here
  //try:
  m_angularSize = atan(m_size / m_position.Mag()) ;
  //except:
  //   import pdb; pdb.set_trace()
}

void Cluster::setEnergy(double energy)
{
  m_energy = energy;
  if (energy > s_maxEnergy)
    s_maxEnergy = energy;
  m_pt = energy * m_position.Unit().Perp() ;
}

//TODO need to change this so that the merged cluster has its ownID and the cluster from which it is made is
//put into subClusters its not rght just yet
Cluster& Cluster::operator+=(const Cluster& rhs){
  
  if (Id::itemType(m_uniqueId) != Id::itemType(rhs.id())) {
    std::cout << "can only add a cluster from the same layer";
  }
  m_position = m_position * m_energy + rhs.position() * rhs.energy();
  m_energy = m_energy + rhs.energy();
  double denom  = 1./m_energy;
  m_position *= denom;
  if (rhs.subClusters().size() != 1)
    std::cout << "can only add in a cluster which is not already merged";
  if (m_subClusters.size()==0)
    m_subClusters.push_back(m_uniqueId);
  m_subClusters.insert(m_subClusters.end(), rhs.subClusters().begin(), rhs.subClusters().end());
  
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Cluster& cluster) { //TODO move to helper class
  os << "Cluster:" << Id::itemType(cluster.m_uniqueId)  << ": "<< std::setw(9) << std::fixed  << cluster.m_energy ;
  os << cluster.m_position.Phi() << ", " << M_PI/2. - cluster.m_position.Theta()<< std::endl;
  return os;
}



} // end namespace papas

/*
 Cluster::Cluster( Cluster && c) :
 m_size(c.m_size),
 m_angularsize(c.m_angularsize),
 m_pt(c.m_pt),
 m_uniqueid(c.m_uniqueid),
 m_energy(c.m_energy)
 {
 m_position=c.m_position;
 }
 
 
 Cluster& Cluster::operator=(Cluster&& c) {
 m_energy=c.m_energy;
 m_position=c.m_position;
 m_size=c.m_size;
 m_pt=c.m_pt;
 m_uniqueid=c.m_uniqueid;
 std::cout<< "move assign cluster" <<std::endl;
 return *this;
 };
 
 Cluster& Cluster::operator=(const Cluster& c) {
 m_energy=c.m_energy;
 m_position=c.m_position;
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





