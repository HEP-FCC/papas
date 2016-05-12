#ifndef RECONSTRUCTION_PFEVENT_H
#define RECONSTRUCTION_PFEVENT_H

#include <iostream>
#include <vector>
#include <unordered_set>

/** @class   rec::PFEvent Reconstruction/Reconstruction/PFEvent.h PFEvent.h
 *
 *  @brief An PFEvent created blocks of connected elements
 *
 *  Example usage: PFEvent b = PFEvent...;
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */
#include <unordered_map>
#include "Id.h"
#include "directedacyclicgraph.h"
#include "PFBlock.h"
class Track;
class Cluster;


//TODO home for typedefs
typedef std::vector<Id::Type> Ids;
typedef DAG::Node<Id::Type> PFNode;
typedef std::unordered_map<Id::Type, PFNode> Nodes;
typedef std::unordered_map<Id::Type, Track> Tracks;
typedef std::unordered_map<Id::Type, PFBlock> Blocks;
typedef std::unordered_map<Id::Type, Cluster> Clusters;
extern Nodes emptyNodes;

class PFEvent {
public:
  PFEvent(Clusters&& ecals, Clusters&& hcals, Tracks&& tracks, Nodes& historyNodes);
  bool compare(Id::Type id1, Id::Type id2) const;
  double energy(Id::Type id1) const;
  const Track& track(Id::Type id ) const;
  const Cluster& cluster(Id::Type id) const;
  const class Cluster& ECALCluster(Id::Type id) const;
  const class Cluster& HCALCluster(Id::Type id) const;
  Ids elementIds() const;
  Nodes& historyNodes() { return m_historyNodes;} //allow these to be changed
  const Clusters& ecalClusters() const { return m_ecals;}
  const Clusters& hcalClusters() const { return m_hcals;}
  const Tracks& tracks() const { return m_tracks;}
  const Blocks& blocks() const { return m_blocks;}
  void setBlocks(Blocks&& blocks) {m_blocks=blocks;}

private:
  Clusters m_ecals; //should these be references instead of owned?
  Clusters m_hcals;
  Tracks m_tracks;
  Nodes& m_historyNodes; //should this be owned?
  Blocks m_blocks;
};



#endif /* PFEvent_h */
