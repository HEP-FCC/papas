#include "papas/datatypes/Event.h"
#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Track.h"
#include "papas/reconstruction/PFBlock.h"
#include <stdio.h>

namespace papas {
/// Event holds pointers to collections of particles, clusters etc and the address of the history associated with
/// an event

Event::Event(std::shared_ptr<Nodes> hist)
    : m_ecalClustersCollection(),
      m_hcalClustersCollection(),
      m_tracksCollection(),
      m_particlesCollection(),
      m_blocksCollection(),
      m_history(hist){};

void Event::addCollection(const Clusters& clusters) {
  // decide if the clusters are from Ecal or Hcal and add to appropriate collection
  if (clusters.size() == 0) return;
  if (IdCoder::isEcal(clusters.begin()->first))
    addCollectionInternal<Cluster>(clusters, m_ecalClustersCollection);
  else
    addCollectionInternal<Cluster>(clusters, m_hcalClustersCollection);
}

void Event::addCollection(const Tracks& tracks) { addCollectionInternal<Track>(tracks, m_tracksCollection); };

void Event::addCollection(const Particles& particles) {
  addCollectionInternal<Particle>(particles, m_particlesCollection);
};

void Event::addCollection(const Blocks& blocks) { addCollectionInternal<PFBlock>(blocks, m_blocksCollection); };

const Clusters& Event::clusters(IdCoder::ItemType type, const IdCoder::SubType subtype) const {
  // return the corresponding collection
  if (!hasCollection(type, subtype)) return m_emptyClusters;
  if (type == IdCoder::ItemType::kEcalCluster)
    return *(m_ecalClustersCollection.at(subtype));
  else
    return *(m_hcalClustersCollection.at(subtype));
};

const Clusters& Event::clusters(Identifier id) const {
  // return the corresponding collection with the same type and subtype as this id
  return clusters(IdCoder::type(id), IdCoder::subtype(id));
};

const Clusters& Event::clusters(const std::string& typeAndSubtype) const {
  // return the corresponding collection with this type and subtype
  return clusters(IdCoder::type(typeAndSubtype[0]), typeAndSubtype[1]);
}

const Tracks& Event::tracks(const IdCoder::SubType subtype) const {
  if (!hasCollection(IdCoder::ItemType::kTrack, subtype)) return m_emptyTracks;
  return *m_tracksCollection.at(subtype);
}

const Particles& Event::particles(const IdCoder::SubType subtype) const {
  if (!hasCollection(IdCoder::ItemType::kParticle, subtype)) return m_emptyParticles;
  return *m_particlesCollection.at(subtype);
}
const Blocks& Event::blocks(const IdCoder::SubType subtype) const {
  if (!hasCollection(IdCoder::ItemType::kBlock, subtype)) return m_emptyBlocks;
  return *m_blocksCollection.at(subtype);
}
bool Event::hasCollection(IdCoder::ItemType type, const IdCoder::SubType subtype) const {
  // Check if this collection is present
  bool found = false;
  switch (type) {
  case IdCoder::kEcalCluster:
    found = (m_ecalClustersCollection.find(subtype) != m_ecalClustersCollection.end());
    break;
  case IdCoder::kHcalCluster:
    found = (m_hcalClustersCollection.find(subtype) != m_hcalClustersCollection.end());
    break;
  case IdCoder::kTrack:
    found = (m_tracksCollection.find(subtype) != m_tracksCollection.end());
    break;
  case IdCoder::kBlock:
    found = (m_blocksCollection.find(subtype) != m_blocksCollection.end());
    break;
  case IdCoder::kParticle:
    found = (m_particlesCollection.find(subtype) != m_particlesCollection.end());
    break;
  default:
    break;
  }
  return found;
};

bool Event::hasCollection(Identifier id) const { return hasCollection(IdCoder::type(id), IdCoder::subtype(id)); };

bool Event::hasObject(Identifier id) const {
  // check if this object id is present
  auto found = false;
  auto type = IdCoder::type(id);
  if (hasCollection(id)) {
    switch (type) {
    case IdCoder::kEcalCluster:
    case IdCoder::kHcalCluster:
      found = (clusters(id).find(id) != clusters(id).end());
      break;
    case IdCoder::kTrack:
      found = (tracks(id).find(id) != tracks(id).end());
      break;
    case IdCoder::kBlock:
      found = (blocks(id).find(id) != blocks(id).end());
      break;
    case IdCoder::kParticle:
      found = (particles(id).find(id) != particles(id).end());
      break;
    default:
      break;
    }
  }
  return found;
};

void Event::extendHistory(const Nodes& history) {
  // A separate history is created at each stage.
  // the following adds this history into the papasevent history
  for (const auto& node : history) {
    for (const auto& c : node.second.children()) {
      makeHistoryLink(node.first, c->value(), *m_history);
    }
  }
}

void Event::clear() {
  m_ecalClustersCollection.clear();
  m_hcalClustersCollection.clear();
  m_tracksCollection.clear();
  m_particlesCollection.clear();
  m_blocksCollection.clear();
  m_history = 0;
}
}
