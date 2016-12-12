////
////  PapasEvent.h
////  papas
////
////  Created by Alice Robson on 18/11/16.
////
////
//
#ifndef PapasEvent_h
#define PapasEvent_h
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Identifier.h"
#include "papas/graphtools/DefinitionsNodes.h"

namespace papas {
/// PapasEvent holds collections of particles, clusters etc and the history associated with an event
/**

 @author  Alice Robson
 @date    2016-04-05
 */
class PapasEvent {
public:
  //History is owned elsewhere
  PapasEvent(Nodes& history);
  
  //store the address of the clusters object into PapasEvent
  void addCollection(const Clusters& clusters);
  //TODO void addCollection(const Tracks& tracks);
  //TODO void addCollection(const Blocks& blocks);
  //TODO void addCollection(const Particles& particles);
  
  const Nodes& getHistory() const {return m_history;};
  
   //get clusters collection matching type and subType of id
  const Clusters& clusters(IdType id) const;
  
  //get clusters collection matching Type and subtype
  const Clusters& clusters(const Identifier::ItemType type, const Identifier::SubType name) const;
  bool hasClusters(IdType id) const;
  bool hasClusters(const Identifier::ItemType type, const Identifier::SubType name) const;
  
  //return cluster matching id
  const Cluster& cluster(IdType id) const;
  //check if cluster matching id exists
  bool hasCluster(IdType id) const;
  void clear();
  
  //TODO const Track& getTrack(IdType id) const;

private:
  //Unordered map of pointers to unordered map of (concrete) Clusters
  CollectionClusters m_ecalClusterCollections;
  CollectionClusters m_hcalClusterCollections;
  //TODO CollectionTracks m_trackCollections
  //TODO CollectionParticles m_particleCollections;
  //TODO CollectionBlocks m_blockCollections;
  Nodes& m_history;
};
}

#endif /* PapasEvent_h */
//