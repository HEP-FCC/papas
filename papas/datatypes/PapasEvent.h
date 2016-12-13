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
/// PapasEvent holds collections of pointers to particles, clusters etc
///  and also the history associated with an event
/**

 @author  Alice Robson
 @date    2016-12-01
 */
class PapasEvent {
public:
  // History is owned elsewhere
  PapasEvent(Nodes& history);

  // store the address of the clusters object into PapasEvent
  //TODO think if there is anyway to help this with templating?
  void addCollection(const Clusters& clusters);
  void addCollection(const Tracks& tracks);
  void addCollection(const Blocks& blocks);
  void addCollection(const SimParticles& particles);

  const Nodes& getHistory() const { return m_history; };
  
  bool hasCollection(IdType id) const;
  bool hasCollection(const Identifier::ItemType type, const Identifier::SubType subtype) const;
  bool hasObject(IdType id) const;

  // get clusters collection id
  const Clusters& clusters(IdType id) const;
  // get clusters collection matching Type and subtype
  const Clusters& clusters(const Identifier::ItemType type, const Identifier::SubType ssubtype) const;
  // return cluster matching id
  const Cluster& cluster(IdType id) const {return clusters(id).at(id);};

  const Tracks& tracks(const Identifier::SubType subtype) const { return *m_tracksCollection.at(subtype); };
  const Tracks& tracks(IdType id) const { return tracks(Identifier::subtype(id)); };
  const Track& track(IdType id) const {return tracks(id).at(id);};

  const SimParticles& particles(const Identifier::SubType subtype) const { return *m_particlesCollection.at(subtype); };
  const SimParticles& particles(IdType id) const { return particles(Identifier::subtype(id)); };
  const SimParticle& particle(IdType id) const {return particles(id).at(id);};
  
  //const Blocks& blocks(const Identifier::SubType subtype) const { return *m_blocksCollection.at(subtype); };
  //const Blocks& blocks(IdType id) const { return blocks(Identifier::subtype(id)); };
  //const PFBlock& block(IdType id) const {return blocks(id).at(id);};;


 
  void clear();

private:
  // Unordered map of pointers to unordered map of (concrete) Clusters
  CollectionClusters m_ecalClustersCollection;
  CollectionClusters m_hcalClustersCollection;
  CollectionTracks m_tracksCollection;
  CollectionParticles m_particlesCollection;
  //CollectionBlocks m_blocksCollection;
  Nodes& m_history;
};
}

#endif /* PapasEvent_h */
//