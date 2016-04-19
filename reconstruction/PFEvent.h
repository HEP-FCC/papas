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
#include "Identifier.h"
#include "directedacyclicgraph.h"
#include <unordered_map>
class Track;
class Cluster;
class PFBlock;


//TODO
typedef long longID;
typedef std::vector<longID> IDs;
typedef std::unordered_map<longID, Cluster> Clusters;
typedef std::unordered_map<longID, Track> Tracks;
typedef DAG::Node<longID> PFNode;
typedef std::unordered_map<longID,PFNode> Nodes;
typedef std::unordered_map<longID, PFBlock> Blocks;

class PFEvent {
public:
  typedef long longID;
  //PFEvent();
  bool compare(longID id1, longID id2) const;
  double energy(longID id1) const;
  PFEvent(Clusters&& ecals, Clusters&& hcals, Tracks&& tracks,Nodes& historyNodes);
  const Track& track(longID id ) const;
  const Cluster& cluster(longID id) const;
  const class Cluster& ECALCluster(longID id) const;
  const class Cluster& HCALCluster(longID id) const;
  IDs elementIDs() const;
  Nodes& historyNodes() { return m_historyNodes;};

private:
  Clusters m_ecals; //should these be references instead of owned?
  Clusters m_hcals;
  Tracks m_tracks;
  Nodes& m_historyNodes;
};



#endif /* PFEvent_h */
