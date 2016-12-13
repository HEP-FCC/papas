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
/// PapasEvent holds pointers to collections of particles, clusters etc and the address of the history associated with
/// an event
/**

 @author  Alice Robson
 @date    2016-04-05
 */

PapasEvent::PapasEvent(Nodes& history)
    : m_ecalClustersCollection{},
      m_hcalClustersCollection{},
      m_tracksCollection{},
      m_particlesCollection{},
      // m_blocksCollection{},
      m_history{history} {};

void PapasEvent::addCollection(const Clusters& clusters) {
  // check that everything in clusters is of same type and subtype
  IdType firstId = 0;
  for (const auto& it : clusters) {
    if (!firstId) {
      firstId = it.first;
      if (hasCollection(firstId)) {
        throw "Clusters Collection already exists";
      }
    }
    if (Identifier::typeAndSubtype(it.first) != Identifier::typeAndSubtype(firstId))
      throw "more than one typeandSubtype found in collection";
  }
  if (Identifier::isEcal(firstId))
    m_ecalClustersCollection.emplace(Identifier::subtype(firstId), &clusters);
  else
    m_hcalClustersCollection.emplace(Identifier::subtype(firstId), &clusters);
};
  
void PapasEvent::addCollection(const Tracks& tracks)
  {
    // check that everything is of same type and subtype
    IdType firstId = 0;
    for (const auto& it : tracks) {
      if (!firstId) {
        firstId = it.first;
        if (hasCollection(firstId)) {
          throw "Tracks Collection already exists";
        }
      }
      if (Identifier::typeAndSubtype(it.first) != Identifier::typeAndSubtype(firstId))
        throw "more than one typeandSubtype found in collection";
    }
    m_tracksCollection.emplace(Identifier::subtype(firstId), &tracks);
  };
  
  void PapasEvent::addCollection(const SimParticles& particles)
  {
    // check that everything is of same type and subtype
    IdType firstId = 0;
    for (const auto& it : particles) {
      if (!firstId) {
        firstId = it.first;
        if (hasCollection(firstId)) {
          throw "Particless Collection already exists";
        }
      }
      if (Identifier::typeAndSubtype(it.first) != Identifier::typeAndSubtype(firstId))
        throw "more than one typeandSubtype found in collection";
    }
    m_particlesCollection.emplace(Identifier::subtype(firstId), &particles);
  };


// void PapasEvent::addCollection(CollectionType name, Blocks&& blocks);

const Clusters& PapasEvent::clusters(Identifier::ItemType type, const Identifier::SubType name) const {
  if (type == Identifier::ItemType::kEcalCluster)
    return *m_ecalClustersCollection.at(name);
  else
    return *m_hcalClustersCollection.at(name);
};

const Clusters& PapasEvent::clusters(IdType id) const {
  if (Identifier::isEcal(id))
    return *m_ecalClustersCollection.at(Identifier::subtype(id));
  else
    return *m_hcalClustersCollection.at(Identifier::subtype(id));
};

bool PapasEvent::hasCollection(Identifier::ItemType type, const Identifier::SubType subtype) const {
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
    // found= (m_BlocksCollection.find(subtype) != m_blocksCollection.end());
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
  auto found = false;
  auto type = Identifier::itemType(id);
  if (hasCollection(id)) {
    switch (type) {
    case Identifier::kEcalCluster:
      found = (clusters(id).find(id) != clusters(id).end());
      break;
    case Identifier::kHcalCluster:
      found = (clusters(id).find(id) != clusters(id).end());
      break;
    case Identifier::kTrack:
      found = (tracks(id).find(id) != tracks(id).end());
      break;
    case Identifier::kBlock:
      // found= (m_BlocksCollection.find(subtype) != m_blocksCollection.end());
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


void PapasEvent::clear() {
  m_ecalClustersCollection.clear();  // deletes pointers not object (which is const)
  m_hcalClustersCollection.clear();
  m_tracksCollection.clear();
  m_particlesCollection.clear();
  // m_blocksCollection.clear();
}
}