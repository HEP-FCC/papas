//
//  PFEvent.cpp
//  fastsim
//
//  Created by Alice Robson on 06/04/16.
//
//

#include "PFEvent.h"
#include "PFBlock.h"
#include "Cluster.h"
#include "Track.h"
#include "Id.h"
#include "Path.h"
#include "Simulator.h" //temp
#include "PFBlockBuilder.h"

namespace papas {

/*
bool PFEvent::compare(Id::Type id1, Id::Type id2) const //TODO check direction of sort
{
  //sort by the type eg ecal hcal
  // and then in order of decreasing energy
  char c1 = Id::TypeShortCode(id1);
  char c2 = Id::TypeShortCode(id2);
  if ( c1 < c2)
    return true;
  else if (c2 > c1)
    return false;
  else if (energy(id1) > energy(id2)) // c1 == c2
    return true;
  return false;
  
}


double PFEvent::energy(Id::Type id1) const //TODO check direction of sort
{
  return 12.5; //TODO
}*/
//check this be a move here
PFEvent::PFEvent(Clusters&& ecals, Clusters&& hcals, Tracks&& tracks, Nodes& historyNodes) :
m_ecals(std::move(ecals)), m_hcals(std::move(hcals)), m_tracks(std::move(tracks)), m_historyNodes(historyNodes)
{
  
}
  
  PFEvent::PFEvent(Simulator& sim) :
  m_ecals(sim.smearedEcalClusters()), m_hcals(sim.smearedHcalClusters()), m_tracks(sim.smearedTracks()), m_historyNodes(sim.historyNodes())
  {
   
  }

  void PFEvent::setBlocks(PFBlockBuilder& builder){
    m_blocks = builder.blocks();
  }
  
const Track& PFEvent::track(Id::Type id) const {
  if (m_tracks.find(id) != m_tracks.end()) {
    return m_tracks.at(id);
  }
  else {
    class Track t;
    return std::move(t); //TODO produce error
  };
}

const Cluster&  PFEvent::ECALCluster(Id::Type id) const {
  if (m_ecals.find(id) != m_ecals.end()) {
    return m_ecals.at(id);
  }
  //TODO throw error or return null
  return m_ecals.at(id); //supress warning for now
}

Ids PFEvent::elementIds() const {
  Ids ids;
  ids.reserve(m_ecals.size() + m_hcals.size() + m_tracks.size());
  for(auto it = m_ecals.begin(); it != m_ecals.end(); ++it) {
    ids.push_back(it->first);
  }
  for(auto it = m_hcals.begin(); it != m_hcals.end(); ++it) {
    ids.push_back(it->first);
  }
  for(auto it = m_tracks.begin(); it != m_tracks.end(); ++it) {
    ids.push_back(it->first);
  }
  return std::move(ids);
}
  
  
std::ostream& operator<<(std::ostream& os, const PFEvent& pfevent) { //TODO move to helper class
  os << "PFEvent:" ;
  for(auto it = pfevent.m_ecals.begin(); it != pfevent.m_ecals.end(); ++it) {
    os << "EC: " << it->second;
  }
  for(auto it = pfevent.m_hcals.begin(); it != pfevent.m_hcals.end(); ++it) {
     os << "HC: " << it->second;
  }
  for(auto it = pfevent.m_tracks.begin(); it != pfevent.m_tracks.end(); ++it) {
     os << "TR: " << it->second;
  }
    return os;
}


const Cluster&  PFEvent::HCALCluster(Id::Type id) const {
  if (m_hcals.find(id) != m_hcals.end()) {
    return m_hcals.at(id);
  }
  class Cluster c;
  return std::move(c); //TODO produce error
}

const Cluster&  PFEvent::cluster(Id::Type id) const {
  if (m_hcals.find(id) != m_hcals.end()) {
    return m_hcals.at(id);
  }
  else if (m_ecals.find(id) != m_ecals.end()) {
    return m_ecals.at(id);
  }
  class Cluster c;
  return std::move(c); //TODO produce error
}

} // end namespace papas
