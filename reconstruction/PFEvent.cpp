//
//  PFEvent.cpp
//  fastsim
//
//  Created by Alice Robson on 06/04/16.
//
//

#include "PFEvent.h"
#include "PFBlock.h"
#include "DataTypes.h"
#include "Identifier.h"


bool PFEvent::compare(longID id1, longID id2) const //TODO check direction of sort
{
  //sort by the type eg ecal hcal
  // and then in order of decreasing energy
  char c1 = Identifier::typeShortCode(id1);
  char c2 = Identifier::typeShortCode(id2);
  if ( c1 < c2)
    return true;
  else if (c2 > c1)
    return false;
  else if (energy(id1) > energy(id2)) // c1 == c2
    return true;
  return false;
  
}


double PFEvent::energy(longID id1) const //TODO check direction of sort
{
  return 12.5; //TODO
  
}

//check this be a move here
PFEvent::PFEvent(Clusters&& ecals, Clusters&& hcals, Tracks&& tracks, Nodes& historyNodes) :
m_ecals(std::move(ecals)), m_hcals(std::move(hcals)), m_tracks(std::move(tracks)), m_historyNodes(historyNodes)
{
  
}

const Track& PFEvent::track(longID id) const {
  if (m_tracks.find(id) != m_tracks.end()) {
    return m_tracks.at(id);
  }
  else {
    class Track t;
    return std::move(t); //TODO produce error
  };
}

const Cluster&  PFEvent::ECALCluster(longID id) const {
  if (m_ecals.find(id) != m_ecals.end()) {
    return m_ecals.at(id);
  }
}

IDs PFEvent::elementIDs() const {
  IDs ids;
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

const Cluster&  PFEvent::HCALCluster(longID id) const {
  if (m_hcals.find(id) != m_hcals.end()) {
    return m_hcals.at(id);
  }
  class Cluster c;
  return std::move(c); //TODO produce error
}

const Cluster&  PFEvent::cluster(longID id) const {
  if (m_hcals.find(id) != m_hcals.end()) {
    return m_hcals.at(id);
  }
  else if (m_ecals.find(id) != m_ecals.end()) {
    return m_ecals.at(id);
  }
  class Cluster c;
  return std::move(c); //TODO produce error
}


