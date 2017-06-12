#include "papas/datatypes/Event.h"
#include "papas/datatypes/Cluster.h"

#include <iomanip>  //lxplus needs this
#include <iostream>

#include "spdlog/details/format.h"

namespace papas {
/// Event holds pointers to collections of particles, clusters etc and the address of the history associated with
/// an event

Event::Event(Nodes& hist)
    : m_ecalClustersFolder(),
      m_hcalClustersFolder(),
      m_tracksFolder(),
      m_particlesFolder(),
      m_blocksFolder(),
      m_history(hist){};

void Event::addCollectionToFolder(const Clusters& clusters) {
  // decide if the clusters are from Ecal or Hcal and add to appropriate collection
  if (clusters.size() == 0) return;
  if (IdCoder::isEcal(clusters.begin()->first))
    addCollectionToFolderInternal<Cluster>(clusters, m_ecalClustersFolder);
  else
    addCollectionToFolderInternal<Cluster>(clusters, m_hcalClustersFolder);
}

void Event::addCollectionToFolder(const Tracks& tracks) {
  addCollectionToFolderInternal<Track>(tracks, m_tracksFolder);
};

void Event::addCollectionToFolder(const Particles& particles) {
  addCollectionToFolderInternal<Particle>(particles, m_particlesFolder);
};

void Event::addCollectionToFolder(const Blocks& blocks) {
  addCollectionToFolderInternal<PFBlock>(blocks, m_blocksFolder);
};

const Clusters& Event::clusters(IdCoder::ItemType type, const IdCoder::SubType subtype) const {
  // return the corresponding collection
  if (!hasCollection(type, subtype)) return m_emptyClusters;
  if (type == IdCoder::ItemType::kEcalCluster)
    return *(m_ecalClustersFolder.at(subtype));
  else
    return *(m_hcalClustersFolder.at(subtype));
};

const Clusters& Event::clusters(Identifier id) const {
  // return the corresponding collection with the same type and subtype as this id
  return clusters(IdCoder::type(id), IdCoder::subtype(id));
};

const Clusters& Event::clusters(const std::string& typeAndSubtype) const {
  // return the corresponding collection with this type and subtype
  return clusters(IdCoder::type(typeAndSubtype[0]), typeAndSubtype[1]);
}

Ids Event::collectionIds(IdCoder::ItemType type, IdCoder::SubType subtype) const {
  // return the collection ids
  Ids ids;
  if (hasCollection(type, subtype)) switch (type) {
    case IdCoder::kEcalCluster:
    case IdCoder::kHcalCluster:
      return collectionIds<Clusters>(clusters(type, subtype));
      break;
    case IdCoder::kTrack:
      return collectionIds<Tracks>(tracks(subtype));
      break;
    case IdCoder::kBlock:
      return collectionIds<Blocks>(blocks(subtype));
      break;
    case IdCoder::kParticle:
      return collectionIds<Particles>(particles(subtype));
      break;
    default:
      break;
    }
  return ids;  // empty
}

Ids Event::collectionIds(const std::string& typeAndSubtype) const {
  // return the collection ids
  return collectionIds(IdCoder::type(typeAndSubtype[0]), typeAndSubtype[1]);
}

const Tracks& Event::tracks(const IdCoder::SubType subtype) const {
  if (!hasCollection(IdCoder::ItemType::kTrack, subtype)) return m_emptyTracks;
  return *m_tracksFolder.at(subtype);
}

const Particles& Event::particles(const IdCoder::SubType subtype) const {
  if (!hasCollection(IdCoder::ItemType::kParticle, subtype)) return m_emptyParticles;
  return *m_particlesFolder.at(subtype);
}

const Blocks& Event::blocks(const IdCoder::SubType subtype) const {
  if (!hasCollection(IdCoder::ItemType::kBlock, subtype)) return m_emptyBlocks;
  return *m_blocksFolder.at(subtype);
}

bool Event::hasCollection(IdCoder::ItemType type, const IdCoder::SubType subtype) const {
  // Check if this collection is present
  bool found = false;
  switch (type) {
  case IdCoder::kEcalCluster:
    found = (m_ecalClustersFolder.find(subtype) != m_ecalClustersFolder.end());
    break;
  case IdCoder::kHcalCluster:
    found = (m_hcalClustersFolder.find(subtype) != m_hcalClustersFolder.end());
    break;
  case IdCoder::kTrack:
    found = (m_tracksFolder.find(subtype) != m_tracksFolder.end());
    break;
  case IdCoder::kBlock:
    found = (m_blocksFolder.find(subtype) != m_blocksFolder.end());
    break;
  case IdCoder::kParticle:
    found = (m_particlesFolder.find(subtype) != m_particlesFolder.end());
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
      makeHistoryLink(node.first, c->value(), m_history);
    }
  }
}

void Event::clear() {
  m_ecalClustersFolder.clear();
  m_hcalClustersFolder.clear();
  m_tracksFolder.clear();
  m_particlesFolder.clear();
  m_blocksFolder.clear();
  m_history.clear();
}

std::string Event::info() const {
  fmt::MemoryWriter out;
  out.write("Papas::Event: {}\n", m_eventNo);
  out.write("\thistory = {}", m_history.size());
  out.write("\n\tecals =");
  for (auto c : m_ecalClustersFolder) {
    out.write(" {}({}) +", c.first, c.second->size());
  }
  out.write("\n\thcals =");
  for (auto c : m_hcalClustersFolder) {
    out.write(" {}({}) +", c.first, c.second->size());
  }
  out.write("\n\ttracks =");
  for (auto c : m_tracksFolder) {
    out.write(" {}({}) +", c.first, c.second->size());
  }
  out.write("\n\tblocks =");
  for (auto c : m_blocksFolder) {
    out.write(" {}({}) +", c.first, c.second->size());
  }
  out.write("\n\tparticles =");
  for (auto c : m_particlesFolder) {
    out.write(" {}({}) +", c.first, c.second->size());
  }
  out.write("\n");
  return out.str();
}
}
