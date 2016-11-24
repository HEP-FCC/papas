////
////  PapasEvent.h
////  papas
////
////  Created by Alice Robson on 18/11/16.
////
////
//
//#ifndef PapasEvent_h
//#define PapasEvent_h
//#include "papas/datatypes/DefinitionsCollections.h"
//#include "papas/datatypes/Identifier.h"
//#include "papas/graphtools/DefinitionsNodes.h"
//
//namespace papas {
//  /// PapasEvent holds collections of particles, clusters etc and the history associated with an event
//  /**
//   
//   @author  Alice Robson
//   @date    2016-04-05
//   */
//  class PapasEvent {
//  public:
//    
//    PapasEvent();
//    
//    void addCollection(Clusters&& clusters); //move into papasevent
//    void addCollection(Tracks&& tracks);
//    void addCollection(Blocks&& blocks);
//    void addCollection(SimParticles&& particles);
//    
//    Nodes& getHistory() { return m_history;};
//    const Clusters& getClusters(const CollectionType& name) const;
//    bool hasClusters(const CollectionType& name) const;
//    const Cluster& getCluster(IdType id) const;
//    bool hasCluster(IdType id) const;
//    //const Track& getTrack(IdType id) const;
//    
//  private:
//    CollectionClusters m_clusterCollections;
//    CollectionTracks m_trackCollections;
//    CollectionParticles m_particleCollections;
//    CollectionBlocks m_blockCollections;
//    Nodes m_history;
//  };
//}
//
//#endif /* PapasEvent_h */
//
