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
#include "Identifier.h"
#include "directedacyclicgraph.h"

class Track;
class Cluster;
class PFBlock;

//TODO home for typedefs
typedef long longID;
typedef std::vector<longID> IDs;
typedef DAG::Node<longID> PFNode;
typedef std::unordered_map<longID, PFNode> Nodes;
typedef std::unordered_map<longID, Track> Tracks;
typedef std::unordered_map<longID, PFBlock> Blocks;
typedef std::unordered_map<longID, Cluster> Clusters;
extern Nodes emptyNodes;

class PFEvent {
public:
  PFEvent(Clusters&& ecals, Clusters&& hcals, Tracks&& tracks, Nodes& historyNodes);
  bool compare(longID id1, longID id2) const;
  double energy(longID id1) const;
  const Track& track(longID id ) const;
  const Cluster& cluster(longID id) const;
  const class Cluster& ECALCluster(longID id) const;
  const class Cluster& HCALCluster(longID id) const;
  IDs elementIDs() const;
  Nodes& historyNodes() { return m_historyNodes;} //allow these to be changed
  const Clusters& ECALClusters() const { return m_ecals;}
  const Clusters& HCALClusters() const { return m_hcals;}
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
