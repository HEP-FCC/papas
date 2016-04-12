//
//  BlockBuilder.cpp
//  fastsim
//
//  Created by Alice Robson on 07/04/16.
//
//

#include "BlockBuilder.h"
#include "PFEvent.h"
#include "Edge.h"
#include "PFBlock.h"
#include "directedacyclicgraph.h"
#include "FloodFill.h"

//Allow optional parameters where construction arguments are references
Nodes emptyNodes;
const Nodes emptyconstNodes;
const PFEvent emptyconstPFEvent;

BlockBuilder::BlockBuilder(
                           IDs ids,
                           Edges& edges,
                           Nodes& historynodes,
                           const PFEvent& pfevent ) :
    m_elementIDs(ids),
    m_edges(edges),
    m_historyNodes(historynodes),
    m_pfEvent(pfevent)
{
  
  //create local nodes ready to use to make the blocks
  for (auto id : ids)
    m_localNodes.emplace(id, PFNode(id));
  
  //use the edge information to say what is linked and add this
  //into the local blocks
  for (const auto& edge : m_edges){
    Edge e = edge.second;
    if  (e.isLinked())  //this is an undirected link - OK for undirected searches
      m_localNodes[e.id1()].addChild(m_localNodes[e.id2()]);
  }
  //build the blocks of connected nodes using FloodFill
  makeBlocks();
  
  
}


void BlockBuilder::makeBlocks() {
  /** uses the DAG::FloodFill algorithm in connection with the BlockBuilder nodes
   to work out which elements are connected
   Each set of connected elements will be used to make a new PFBlock
   */
  
  DAG::FloodFill<longID> FFill;
  
  //traverse does the work and returns a vector of connected node groups
  for (auto & nodevector : FFill.traverse(m_localNodes)) {
    
    //each of the nodevectors is about to become a separate block
    //we need the vector of ids and the map of edges in order to make the block
    IDs elementIDs;
    for (auto& node : nodevector) {
      elementIDs.push_back(node->getValue());
    }
  
    
    //make the block
    sortIDs(elementIDs);//TODO allow sorting my energy using a helper class
    PFBlock block {elementIDs,  m_edges};
    
    //put the block in the unordered map of blocks using move
    m_blocks.emplace(block.uniqueID(),block);
    
    //update the history nodes (if they exist)
    
    if (m_historyNodes.size()>0) {
      //make a new history node for the block and add into the history Nodes
      PFNode blocknode{block.uniqueID()};
      m_historyNodes.emplace(block.uniqueID(), blocknode); // move
      //add in the links between the block elements and the block
      for ( auto elemid : block.elementIDs()) {
        m_historyNodes[elemid].addChild(blocknode);
      }
    }
  }
}


std::ostream& operator<<(std::ostream& os, const BlockBuilder& builder) {
  //TODO move to helper
  for (auto block : builder.m_blocks) {
    os  << block.second << std::endl;
  }
  return os;
}

/*void BlockBuilder::sortIDs(std::vector<longID>& ids) // sorts by type and energy
{//TODO move to helper
  std::sort( ids.begin(), ids.end(), [this] (longID a, longID b) { return this->m_pfEvent.compare(a,b);});
}*/

void BlockBuilder::sortIDs(std::vector<longID>& ids)
{
  std::sort(ids.begin(), ids.end(), [](longID a, longID b) -> bool
                     { return Identifier::typeShortCode(a) < Identifier::typeShortCode(b); } );
}

/*bool BlockBuilder::compareEdges(long long key1, long long key2, longID uniqueid) const//TODO check direction of sort
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
 }*/



int test_BlockBuilder() {
  
  
  PFBlock::longID id1 = Identifier::makeECALClusterID();
  PFBlock::longID id2 = Identifier::makeHCALClusterID();
  PFBlock::longID id3 = Identifier::makeTrackID();
  
  PFBlock::longID id4 = Identifier::makeECALClusterID();
  PFBlock::longID id5 = Identifier::makeHCALClusterID();
  PFBlock::longID id6 = Identifier::makeTrackID();
  
  std::vector<PFBlock::longID> ids {id1,id2,id3, id4,id5,id6};
  
  Edge edge = Edge(id1, id2, false, 0.00023);
  Edge edge1 = Edge(id1, id3, true, 10030.0);
  Edge edge2 = Edge(id2, id3, true, 0.00005);
  
  Edge edge4 = Edge(id4, id5, false, 3.1234);
  Edge edge5 = Edge(id4, id6, true, 0.1234);
  Edge edge6 = Edge(id5, id6, true, 123.0);
  
  std::unordered_map<long long, const Edge> edges;
  
  edges.emplace(edge.key(),  edge);
  edges.emplace(edge1.key(), edge1);
  edges.emplace(edge2.key(), edge2);
  edges.emplace(edge4.key(), edge4);
  edges.emplace(edge5.key(), edge5);
  edges.emplace(edge6.key(), edge6);
  

  //create history nodes
  Nodes historyNodes;
  for (auto id : ids)
    historyNodes.emplace(id, std::move(PFNode(id)));

  
  auto blockbuilder = BlockBuilder(ids, edges, historyNodes);
  
  std::cout<<blockbuilder;
  return 0;
}


