//
//  PapasEvent.cpp
//  papas
//
//  Created by Alice Robson on 18/11/16.
//
//

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Identifier.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/datatypes/SimParticle.h"
#include "papas/datatypes/Track.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/reconstruction/PFBlock.h"
#include <stdio.h>

namespace papas {
/// PapasEvent holds collections of particles, clusters etc and the history associated with an event
/**

 @author  Alice Robson
 @date    2016-04-05
 */

PapasEvent::PapasEvent()
    : m_ecalClusterCollections{},
      m_hcalClusterCollections{},
      /*, m_trackCollections{}, m_particleCollections{}, m_blockCollections{}, */ m_history{} {};

void PapasEvent::addCollection(const papas::Clusters& clusters) {

  // check that everything in clusters is of same type and subtype
  std::string collectionname = "uu";  // unset
  std::string nexttype;
  bool isEcal;
  auto first = true;
  for (const auto& it : clusters) {
    nexttype = Identifier::typeAndSubtype(it.first);
    if (first) {
      first = false;
      collectionname = nexttype;
      if (hasClusters(it.first)) throw "Clusters Collection already exists";
      isEcal = Identifier::isEcal(it.first);
    }
    if (nexttype != collectionname) throw "more than one typeandSubtype found in collection";
  }
  /*if (m_clusterCollections.find(name)) {
         throw "this CollectionType already exists";
  }*/
  if (collectionname[0] == 'e')
    m_ecalClusterCollections.emplace(collectionname[2], &clusters);
  else
    m_hcalClusterCollections.emplace(collectionname[2], &clusters);
};
// move into papasevent
// void PapasEvent::addCollection(CollectionType name, Tracks&& tracks);
// void PapasEvent::addCollection(CollectionType name, Blocks&& blocks);
// void PapasEvent::addCollection(CollectionType name, SimParticles&& particles);

const Clusters& PapasEvent::clusters(Identifier::ItemType type, const CollectionType name) const {
  if (type == Identifier::ItemType::kEcalCluster)
    return *m_ecalClusterCollections.at(name);
  else
    return *m_hcalClusterCollections.at(name);
};

const Clusters& PapasEvent::clusters(IdType id) const {
  if (Identifier::isEcal(id))
    return *m_ecalClusterCollections.at(Identifier::subtype(id));
  else
    return *m_hcalClusterCollections.at(Identifier::subtype(id));
};

bool PapasEvent::hasClusters(IdType id) const {
  if (Identifier::isEcal(id))
    return m_ecalClusterCollections.find(Identifier::subtype(id)) != m_ecalClusterCollections.end();
  else
    return m_hcalClusterCollections.find(Identifier::subtype(id)) != m_hcalClusterCollections.end();
};

bool PapasEvent::hasClusters(Identifier::ItemType type, const CollectionType name) const {
  if (type == Identifier::ItemType::kEcalCluster)
    return m_ecalClusterCollections.find(name) != m_ecalClusterCollections.end();
  else
    return m_hcalClusterCollections.find(name) != m_hcalClusterCollections.end();
};

// const Tracks& PapasEvent::getTracks(const CollectionType& name) const;

const Cluster& PapasEvent::cluster(IdType id) const { return clusters(id).at(id); }

bool PapasEvent::hasCluster(IdType id) const {
  if (hasClusters(id)) {
    auto cclusters = clusters(id);
    return (cclusters.find(id) != cclusters.end());
  }
  else
    return false;};
// const Track& PapasEvent::getTrack(IdType id) const;
}