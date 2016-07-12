//
//  PFEvent.cpp
//  fastsim
//
//  Created by Alice Robson on 06/04/16.
//
//

#include "PFEvent.h"
#include "Cluster.h"
#include "Id.h"
#include "PDebug.h"
#include "MergedClusterBuilder.h"
#include "PFBlock.h"
#include "PFBlockBuilder.h"
#include "Path.h"
#include "PrettyPrinter.h"
#include "Simulator.h"  //temp
#include "pTrack.h"
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
// check this be a move here

PFEvent::PFEvent(const Clusters& ecals, const Clusters& hcals, const Tracks& tracks, Nodes& historyNodes)
    : m_ecals(ecals), m_hcals(hcals), m_tracks(tracks), m_historyNodes(historyNodes), m_reconstructedParticles() {}

PFEvent::PFEvent(Simulator& sim)
    : m_ecals(sim.smearedEcalClusters()),
      m_hcals(sim.smearedHcalClusters()),
      m_tracks(sim.smearedTracks()),
      m_historyNodes(sim.historyNodes()),
      m_reconstructedParticles() {}

  /*void PFEvent::setBlocks(PFBlockBuilder& builder) { m_blocks = std::move(builder.blocks()); }*/

void PFEvent::mergeClusters() {
  Ruler ruler{*this};
  MergedClusterBuilder ecalmerger{m_ecals, ruler, m_historyNodes};
  MergedClusterBuilder hcalmerger{m_hcals, ruler, m_historyNodes};
  m_mergedEcals = std::move(ecalmerger.mergedClusters());
  m_mergedHcals = std::move(hcalmerger.mergedClusters());
}

const Track& PFEvent::track(Id::Type id) const {
  if (m_tracks.find(id) != m_tracks.end()) {
    return m_tracks.at(id);
  } else {
    class Track t;

    PDebug::write("problem with track not found :{}", id);
    return std::move(t);  // TODO produce error
  };
}

const Cluster& PFEvent::ECALCluster(Id::Type id) const {
  if (m_mergedEcals.find(id) != m_mergedEcals.end()) {  // not quite as I want it yet...
    return m_mergedEcals.at(id);
  }
  if (m_ecals.find(id) != m_ecals.end()) {
    return m_ecals.at(id);
  }
  // TODO throw error or return null
  return m_ecals.at(id);  // supress warning for now
}

Ids PFEvent::elementIds() const {
  Ids ids;
  ids.reserve(m_ecals.size() + m_hcals.size() + m_tracks.size());
  for (auto it = m_ecals.begin(); it != m_ecals.end(); ++it) {
    ids.push_back(it->first);
  }
  for (auto it = m_hcals.begin(); it != m_hcals.end(); ++it) {
    ids.push_back(it->first);
  }
  for (auto it = m_tracks.begin(); it != m_tracks.end(); ++it) {
    ids.push_back(it->first);
  }
  return std::move(ids);
}

Ids PFEvent::mergedElementIds() const {
  Ids ids;

  ids.reserve(m_mergedEcals.size() + m_mergedHcals.size() + m_tracks.size());
  for (auto it = m_mergedEcals.begin(); it != m_mergedEcals.end(); ++it) {
    ids.push_back(it->first);
  }
  for (auto it = m_mergedHcals.begin(); it != m_mergedHcals.end(); ++it) {
    ids.push_back(it->first);
  }
  for (auto it = m_tracks.begin(); it != m_tracks.end(); ++it) {
    ids.push_back(it->first);
  }
  return std::move(ids);
}

std::ostream& operator<<(std::ostream& os, const PFEvent& pfevent) {  // TODO move to helper class??
  os << "PFEvent:" << std::endl;
  os << " EC: " << pfevent.m_ecals << std::endl;
  os << " HC: " << pfevent.m_hcals << std::endl;
  os << " TR: " << pfevent.m_tracks << std::endl;
  return os;
}

const Cluster& PFEvent::HCALCluster(Id::Type id) const {
  if (m_mergedHcals.find(id) != m_mergedHcals.end()) {  // not quite as I want it yet...
    return m_mergedHcals.at(id);
  } else if (m_hcals.find(id) != m_hcals.end()) {
    return m_hcals.at(id);
  }
  class Cluster c;
  PDebug::write("problem with cluster not found :{}", id);
  PDebug::flush();
  return std::move(c);  // TODO produce error
}

const Cluster& PFEvent::cluster(Id::Type id) const {
   if (m_hcals.find(id) != m_hcals.end()) {
     if (m_mergedHcals.find(id) != m_mergedHcals.end()) {
       std::cout <<"humph MH";
     }
     if (m_mergedEcals.find(id) != m_mergedHcals.end()) {
       std::cout <<"humph ME";
     }
     if (m_ecals.find(id) != m_ecals.end()) {
       std::cout <<"humph e";}
    return m_hcals.at(id);
  } else if (m_ecals.find(id) != m_ecals.end()) {
    return m_ecals.at(id);
  } else if (m_mergedHcals.find(id) != m_mergedHcals.end()) {
    return m_mergedHcals.at(id);
  } else if (m_mergedEcals.find(id) != m_mergedEcals.end()) {
    return m_mergedEcals.at(id);
  } 
  std::cout << "Oopps" << Id::pretty(id);
  // TODO throw error
  class Cluster c;
  PDebug::write("problem with cluster not found :{}", id);
  PDebug::flush();
  return std::move(c);  // TODO produce error
}

void PFEvent::clear() {
  //m_blocks.clear();
  m_mergedEcals.clear();
  m_mergedHcals.clear();
  m_reconstructedParticles.clear();
}

}  // end namespace papas