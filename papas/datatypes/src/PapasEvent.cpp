#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/datatypes/SimParticle.h"
#include "papas/datatypes/Track.h"
#include "papas/reconstruction/PFBlock.h"
#include <stdio.h>

namespace papas {
/// PapasEvent holds pointers to collections of particles, clusters etc and the address of the history associated with
/// an event

PapasEvent::PapasEvent()
    : m_ecalClustersCollection{},
      m_hcalClustersCollection{},
      m_tracksCollection{},
      m_particlesCollection{},
      m_blocksCollection{},
      m_historys{} {};

void PapasEvent::addCollection(const Clusters& clusters) {
  // decide if the clusters are from Ecal or Hcal and add to appropriate collection
  if (Identifier::isEcal(clusters.begin()->first))
    addCollectionInternal<Cluster>(clusters, m_ecalClustersCollection);
  else
    addCollectionInternal<Cluster>(clusters, m_hcalClustersCollection);
}

void PapasEvent::addHistory(const Nodes& history) { m_historys.push_back(&history); }

void PapasEvent::addCollection(const Tracks& tracks) { addCollectionInternal<Track>(tracks, m_tracksCollection); };

void PapasEvent::addCollection(const SimParticles& particles) {
  addCollectionInternal<SimParticle>(particles, m_particlesCollection);
};

void PapasEvent::addCollection(const Blocks& blocks) { addCollectionInternal<PFBlock>(blocks, m_blocksCollection); };

const Clusters& PapasEvent::clusters(Identifier::ItemType type, const Identifier::SubType subtype) const {
  // return the corresponding collection
  if (type == Identifier::ItemType::kEcalCluster)
    return *(m_ecalClustersCollection.at(subtype));
  else
    return *(m_hcalClustersCollection.at(subtype));
};

const Clusters& PapasEvent::clusters(IdType id) const {
  // return the corresponding collection with the same type and subtype as this id
  return clusters(Identifier::itemType(id), Identifier::subtype(id));
};

const Clusters& PapasEvent::clusters(const std::string& typeAndSubtype) const {
  // return the corresponding collection with this type and subtype
  return clusters(Identifier::itemType(typeAndSubtype[0]), typeAndSubtype[1]);
}
  
  
bool PapasEvent::hasCollection(Identifier::ItemType type, const Identifier::SubType subtype) const {
  // Check if this collection is present
  auto found = false;
  switch (type) {
  case Identifier::kEcalCluster:
    found = (m_ecalClustersCollection.find(subtype) != m_ecalClustersCollection.end());
    break;
  case Identifier::kHcalCluster:
    found = (m_hcalClustersCollection.find(subtype) != m_hcalClustersCollection.end());
    break;
  case Identifier::kTrack:
    found = (m_tracksCollection.find(subtype) != m_tracksCollection.end());
    break;
  case Identifier::kBlock:
    found = (m_blocksCollection.find(subtype) != m_blocksCollection.end());
    break;
  case Identifier::kParticle:
    found = (m_particlesCollection.find(subtype) != m_particlesCollection.end());
    break;
  default:
    break;
  }
  return found;
};

bool PapasEvent::hasCollection(IdType id) const {
  return hasCollection(Identifier::itemType(id), Identifier::subtype(id));
};

bool PapasEvent::hasObject(IdType id) const {
  // check if this object id is present
  auto found = false;
  auto type = Identifier::itemType(id);
  if (hasCollection(id)) {
    switch (type) {
    case Identifier::kEcalCluster:
    case Identifier::kHcalCluster:
      found = (clusters(id).find(id) != clusters(id).end());
      break;
    case Identifier::kTrack:
      found = (tracks(id).find(id) != tracks(id).end());
      break;
    case Identifier::kBlock:
      found = (blocks(id).find(id) != blocks(id).end());
      break;
    case Identifier::kParticle:
      found = (particles(id).find(id) != particles(id).end());
      break;
    default:
      break;
    }
  }
  return found;
};
  
  
 void PapasEvent::mergeHistories() {
    //A separate history is created at each stage.
    //the following merges these separate histories into
    //one single history that can be used for analysis
    for (auto history : m_historys)
    {
      for (const auto node : *history){
        for (const auto& c : node.second.children()) {
          makeHistoryLink(node.first, c->value(), m_history);
        }
      }
    }
    
  }
  
  
void PapasEvent::clear() {
  m_ecalClustersCollection.clear();
  m_hcalClustersCollection.clear();
  m_tracksCollection.clear();
  m_particlesCollection.clear();
  m_blocksCollection.clear();
  m_historys.clear();
  m_history.clear();
}
}
