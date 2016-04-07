#ifndef RECONSTRUCTION_BLOCKBUILDER_H
#define RECONSTRUCTION_BLOCKBUILDER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "PFEvent.h"
#include "Edge.h"
#include "PFBlock.h"

/** @class   rec::BlockBuilder Reconstruction/Reconstruction/BlockBuilder.h BlockBuilder.h
 *
 *  @brief An BlockBuilder created blocks of connected elements
 *
 *  Example usage: BlockBuilder b = BlockBuilder...;
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */

//TODO namespace
class PFEvent;

class BlockBuilder {
  typedef long longID;
public:
  BlockBuilder(const PFEvent& pfevent);
  
private:
  void sortIDs(std::vector<longID>& ids);
  //void sortEgdes
  bool compareEdges( long long key1, long long key2, longID uniqueid) const;
  //bool compare(longID id1, longID id2) const;
  const PFEvent& m_pfEvent; //allows access to the underlying objects
  std::unordered_map<long long, Edge> m_edges;
};

void BlockBuilder::sortIDs(std::vector<longID>& ids)
{
  std::sort( ids.begin(), ids.end(), [this] (longID a, longID b) { return this->m_pfEvent.compare(a,b);});
}

bool BlockBuilder::compareEdges(long long key1, long long key2, longID uniqueid) const//TODO check direction of sort
{
  //sort by the type eg ecal hcal
  // and then in order of decreasing energy
  Edge e1 = m_edges.find(key1)->second; // should part of this be a static function in Edges?
  Edge e2 = m_edges.find(key2)->second;
  if (e1.distance() < e2.distance())
    return true;
  else if (e1.distance() > e2.distance())
    return false;
  // the distance for edge1 and edge 2 is same
  // so return based on edgetype and end energy comparison for the items
  // at the other end from uniqueID
  double energy1 = m_pfEvent.getEnergy(e1.otherID(uniqueid));
  double energy2 = m_pfEvent.getEnergy(e2.otherID(uniqueid));
  
  return (energy1 > energy2) ;
}




#endif /* BlockBuilder_h */
