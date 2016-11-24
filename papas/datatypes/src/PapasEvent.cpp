////
////  PapasEvent.cpp
////  papas
////
////  Created by Alice Robson on 18/11/16.
////
////
//
//#include <stdio.h>
////
////  PapasEvent.h
////  papas
////
////  Created by Alice Robson on 18/11/16.
////
////
//
//#include "papas/datatypes/DefinitionsCollections.h"
//#include "papas/datatypes/Identifier.h"
//#include "papas/datatypes/PapasEvent.h"
//#include "papas/graphtools/DefinitionsNodes.h"
//#include "papas/datatypes/Cluster.h"
//#include "papas/datatypes/SimParticle.h"
//#include "papas/datatypes/Track.h"
//#include "papas/reconstruction/PFBlock.h"
//namespace papas {
///// PapasEvent holds collections of particles, clusters etc and the history associated with an event
///**
//
// @author  Alice Robson
// @date    2016-04-05
// */
//PapasEvent::PapasEvent()
//    : m_clusterCollections{}, m_trackCollections{}, m_particleCollections{}, m_blockCollections{}, m_history{} {};
//
//void PapasEvent::addCollection(Clusters&& clusters) {
//  CollectionType name;
//  auto first = true;
//   //check that everything in clusters is of same type and subtype
//  for (const auto & it : clusters) {
//    auto nextname = Identifier::typeAndSubtype(it.first);
//    if (first)  {
//      name = nextname;
//      first = false;
//    }
//    if (nextname != name) throw "more than one typeandSubtype found in collection";
//  }
//  /*if (m_clusterCollections.find(name)) {
//         throw "this CollectionType already exists";
//  }*/
//  m_clusterCollections.emplace(name, clusters);
//};
//// move into papasevent
////void PapasEvent::addCollection(CollectionType name, Tracks&& tracks);
////void PapasEvent::addCollection(CollectionType name, Blocks&& blocks);
////void PapasEvent::addCollection(CollectionType name, SimParticles&& particles);
//
//  const Clusters& PapasEvent::getClusters(const CollectionType& name) const {
//    return m_clusterCollections.at(name);
//  };
//  
//  bool PapasEvent::hasClusters(const CollectionType& name) const {
//    return m_clusterCollections.find(name)!=m_clusterCollections.end();
//  };
//
//  //const Tracks& PapasEvent::getTracks(const CollectionType& name) const;
//
//  const Cluster& PapasEvent::getCluster(IdType id) const {
//    CollectionType name = Identifier::typeAndSubtype(id);
//    return getClusters(name).at(id);
//  }
//  
//  bool PapasEvent::hasCluster(IdType id) const {
//    CollectionType name = Identifier::typeAndSubtype(id);
//    if (hasClusters(name)){
//      const Clusters& clusters = getClusters(name);
//      return (clusters.find(id) != clusters.end());
//    }
//    return false;
//  }
//  //const Track& PapasEvent::getTrack(IdType id) const;
//
//}