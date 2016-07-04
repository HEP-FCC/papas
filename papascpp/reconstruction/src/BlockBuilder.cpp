//
//  BlockBuilder.cpp
//  fastsim
//
//  Created by Alice Robson on 07/04/16.
//
//

#include "BlockBuilder.h"
//#include "PFEvent.h"
#include "PFBlock.h"
#include "directedacyclicgraph.h"
#include "FloodFill.h"
#include "Log.h"
#include "Definitions.h"

namespace papas {


BlockBuilder::BlockBuilder(Ids ids, Edges& edges, Nodes& historynodes) :
  GraphBuilder(ids, edges),
  m_historyNodes(historynodes),
  m_blocks()
{
  makeBlocks();
}
  


/*BlockBuilder& BlockBuilder::operator=(const BlockBuilder& b)
{
  m_elementIds=b.m_elementIds;
  m_historyNodes=b.m_historyNodes;
  m_edges = b.m_edges;
  m_blocks =b.m_blocks;
  return *this;
}*/


void BlockBuilder::makeBlocks()
{
  /** uses the base class  GraphBuilder to work out which elements are connected
   Each set of connected elements will be used to make a new PFBlock
   */

  for (auto& elementIds : m_subGraphs) {

    //make the block
    sortIds(elementIds);//TODO allow sorting by energy using a helper class
    PFBlock block {elementIds, m_edges};
    PDebug::write("Made {}", block);
    //put the block in the unordered map of blocks using move
    m_blocks.emplace(block.uniqueId(), std::move(block));

    //update the history nodes (if they exist)
    if (m_historyNodes.size() > 0) {
      //make a new history node for the block and add into the history Nodes
      PFNode blocknode{block.uniqueId()};
      m_historyNodes.emplace(block.uniqueId(), std::move(blocknode)); // move
      //add in the links between the block elements and the block
      for (auto elemid : block.elementIds()) {
        m_historyNodes[elemid].addChild(blocknode);
      }
    }
  }
}


std::ostream& operator<<(std::ostream& os, const BlockBuilder& builder)
{
  //TODO move to helper
  for (const auto& block : builder.m_blocks) {
    os  << block.second << std::endl;
  }
  return os;
}

/*void BlockBuilder::sortIds(std::vector<Id::Type>& ids) // sorts by type and energy
{//TODO move to helper
  std::sort( ids.begin(), ids.end(), [this] (Id::Type a, Id::Type b) { return this->m_pfEvent.compare(a,b);});
}*/



/*bool BlockBuilder::compareEdges(long long key1, long long key2, Id::Type uniqueid) const//TODO check direction of sort
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
 // at the other end from uniqueId
 double energy1 = m_pfEvent.energy(e1.otherid(uniqueid));
 double energy2 = m_pfEvent.energy(e2.otherid(uniqueid));

 return (energy1 > energy2) ;
 }*/



} // end namespace papas
/*
//TODO make into gtest and move
int test_BlockBuilder()
{
  using namespace papas;
  
  Id::Type id1 = Id::makeEcalId();
  Id::Type id2 = Id::makeHcalId();
  Id::Type id3 = Id::makeTrackId();
  
  Id::Type id4 = Id::makeEcalId();
  Id::Type id5 = Id::makeHcalId();
  Id::Type id6 = Id::makeTrackId();
  
  std::vector<Id::Type> ids {id1, id2, id3, id4, id5, id6};
  
  Edge edge = Edge(id1, id2, false, 0.00023);
  Edge edge1 = Edge(id1, id3, true, 10030.0);
  Edge edge2 = Edge(id2, id3, true, 0.00005);
  
  Edge edge4 = Edge(id4, id5, false, 3.1234);
  Edge edge5 = Edge(id4, id6, true, 0.1234);
  Edge edge6 = Edge(id5, id6, true, 123.0);
  
  Edges edges;
  
  edges.emplace(edge.key(),  std::move(edge));
  edges.emplace(edge1.key(), std::move(edge1));
  edges.emplace(edge2.key(), std::move(edge2));
  edges.emplace(edge4.key(), std::move(edge4));
  edges.emplace(edge5.key(), std::move(edge5));
  edges.emplace(edge6.key(), std::move(edge6));
  
  
  //create history nodes
  Nodes historyNodes;
  for (auto id : ids)
    historyNodes.emplace(id, std::move(PFNode(id)));
  
  
  auto blockbuilder = BlockBuilder(ids, edges, historyNodes);
  
  std::cout << blockbuilder;
  return 0;
}*/


