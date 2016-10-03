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
#include "MergedClusterBuilder.h"
#include "PDebug.h"
#include "PFBlock.h"
#include "PFBlockBuilder.h"
#include "Path.h"
#include "PrettyPrinter.h"
#include "Simulator.h"  //temp
#include "pTrack.h"
namespace papas {

PFEvent::PFEvent(const Clusters& ecals, const Clusters& hcals, const Tracks& tracks, Nodes& historyNodes)
    : m_ecals(ecals), m_hcals(hcals), m_tracks(tracks), m_historyNodes(historyNodes), m_reconstructedParticles() {}

PFEvent::PFEvent(Simulator& sim)
    : m_ecals(sim.smearedEcalClusters()),
      m_hcals(sim.smearedHcalClusters()),
      m_tracks(sim.smearedTracks()),
      m_historyNodes(sim.historyNodes()),
      m_reconstructedParticles() {}

const Track& PFEvent::track(IdType id) const {
  if (m_tracks.find(id) != m_tracks.end()) {
    return m_tracks.at(id);
  }
  throw "problem with track not found ";
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

const Cluster& PFEvent::ECALCluster(IdType id) const {
  if (m_mergedEcals.find(id) != m_mergedEcals.end()) {
    return m_mergedEcals.at(id);
  }
  if (m_ecals.find(id) != m_ecals.end()) {
    return m_ecals.at(id);
  }
  PDebug::write("problem with ECAL cluster not found :{}", id);
  PDebug::flush();
  throw std::range_error("Cluster not found in ECAL Clusters Collections");
}

const Cluster& PFEvent::HCALCluster(IdType id) const {
  if (m_mergedHcals.find(id) != m_mergedHcals.end()) {
    return m_mergedHcals.at(id);
  } else if (m_hcals.find(id) != m_hcals.end()) {
    return m_hcals.at(id);
  }
  PDebug::write("problem with HCAL cluster not found :{}", id);

  PDebug::flush();
  throw std::range_error("Cluster not found in HCAL Clusters Collections");
}

const Cluster& PFEvent::cluster(IdType id) const {
  if (m_hcals.find(id) != m_hcals.end()) {
    if (m_mergedHcals.find(id) != m_mergedHcals.end()) throw "id is erroneously found in more than one collection";
    if (m_mergedEcals.find(id) != m_mergedEcals.end()) throw "id is erroneously found in more than one collection";
    if (m_ecals.find(id) != m_ecals.end()) throw "id is erroneously found in more than one collection";

    return m_hcals.at(id);
  } else if (m_ecals.find(id) != m_ecals.end()) {
    return m_ecals.at(id);
  } else if (m_mergedHcals.find(id) != m_mergedHcals.end()) {
    return m_mergedHcals.at(id);
  } else if (m_mergedEcals.find(id) != m_mergedEcals.end()) {
    return m_mergedEcals.at(id);
  }
  PDebug::write("problem with cluster not found :{}", id);
  PDebug::flush();
  throw "error, cluster not found in PFEvent collections";
}

void PFEvent::clear() {
  m_mergedEcals.clear();
  m_mergedHcals.clear();
  m_reconstructedParticles.clear();
}

}  // end namespace papas

/*
 bool PFEvent::compare(IdType id1, IdType id2) const //TODO check direction of sort
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

 */