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
/// PapasEvent holds pointers to collections of particles, clusters etc and the address of the history associated with an event
/**

 @author  Alice Robson
 @date    2016-04-05
 */

PapasEvent::PapasEvent(Nodes& history)
    : m_ecalClusterCollections{},
      m_hcalClusterCollections{},
      /*, m_trackCollections{}, m_particleCollections{}, m_blockCollections{}, */ m_history{history} {};

void PapasEvent::addCollection(const papas::Clusters& clusters) {
  
  
  
  
  // check that everything in clusters is of same type and subtype
  IdType firstId = 0;
  for (const auto& it : clusters) {
      if (!firstId) {
        firstId = it.first;
        if (hasClusters(firstId)) {
          std::cout << Identifier::pretty(firstId) << std::endl;
          throw "Clusters Collection already exists";
        }
    }
    if (Identifier::typeAndSubtype(it.first) != Identifier::typeAndSubtype(firstId)) throw "more than one typeandSubtype found in collection";
  }
  if (Identifier::itemType(firstId)==Identifier::kEcalCluster)
    m_ecalClusterCollections.emplace(Identifier::subtype(firstId), &clusters);
  else
    m_hcalClusterCollections.emplace(Identifier::subtype(firstId), &clusters);
};
// move into papasevent
// void PapasEvent::addCollection(CollectionType name, Tracks&& tracks);
// void PapasEvent::addCollection(CollectionType name, Blocks&& blocks);
// void PapasEvent::addCollection(CollectionType name, SimParticles&& particles);

  const Clusters& PapasEvent::clusters(Identifier::ItemType type, const Identifier::SubType name) const {
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

bool PapasEvent::hasClusters(Identifier::ItemType type, const Identifier::SubType name) const {
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

  void PapasEvent::clear() {
    m_ecalClusterCollections.clear(); //deletes pointers not object (which is const)
    m_hcalClusterCollections.clear();
  }

}

