//
//  BlockBuilder.cpp
//  fastsim
//
//  Created by Alice Robson on 07/04/16.
//
//

#include "BlockBuilder.h"
#include "directedacyclicgraph.h"



BlockBuilder::BlockBuilder(IDs ids,
             Edges& edges,
             Nodes& historynodes ,
             const PFEvent& pfevent ) :
    m_elementIDs(ids),
    m_edges(edges),
    m_historyNodes(historynodes),
    m_pfEvent(pfevent)
{
  
  //create local nodes
  for (auto id :ids)
    m_localNodes.emplace(id, PFNode(id));
  
  for (const auto& edge : m_edges){
    Edge e =edge.second;
    //add linkage info into the nodes dictionary
    if  (e.isLinked())  //this is actually an undirected link - OK for undirected searches
      m_localNodes[e.id1()].addChild(m_localNodes[e.id2()]);
    std::cout << e;
 
    
  }
  //build the blocks of connected nodes
  makeBlocks();

  
}


void BlockBuilder::makeBlocks() {
  /** uses the DAGfloodfill algorithm in connection with the BlockBuilder nodes
   to work out which elements are connected
   Each set of connected elements will be used to make a new PFBlock
   */
  DAG::FloodFill<longID> FF{};
  for (auto & nodevector : FF.traverse(m_localNodes)) {
    IDs elementIDs;
    // NB the nodes that are found by FloodFill are the Nodes describing links between items
    // we want the ids of these nodes
    for (auto& node : nodevector) {
      elementIDs.push_back(node->getValue());
    }
    std::sort(elementIDs.begin(), elementIDs.end(), [](longID a, longID b) -> bool
              { return Identifier::typeShortCode(a) < Identifier::typeShortCode(b); } );
    //make the block
    PFBlock block {elementIDs,  m_edges};
    
    //put the block in the dict of blocks
    m_blocks[block.uniqueID()] = std::move(block);
 
    //make a node for the block and add into the history Nodes
    if (m_historyNodes.size()>0) {
      DAG::Node<longID> blocknode{block.uniqueID()};
      m_historyNodes.emplace(block.uniqueID(), blocknode);
      //now add in the links between the block elements and the block into the m_historyNodes
      for ( auto elemid : block.elementIDs()) {
        m_historyNodes[elemid].addChild(blocknode);
        std::cout <<block;
      }
    }
  
  }
  
}


//= std::unordered_map<longID, DAG::Node<longID>>()


std::ostream& operator<<(std::ostream& os, const BlockBuilder& builder) {
  
  //std::sorted(self.blocks, key = lambda k: (len(self.blocks[k].element_uniqueids), self.blocks[k].short_name()),reverse =True)
  for (auto block : builder.m_blocks) {
    os  << block.second << std::endl;
  }
  return os;
}

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



int testBlockBuilder() {
  

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
  
  //create local nodes
  Nodes m_localNodes;
  for (auto id :ids)
    m_localNodes.emplace(id, std::move(PFNode(id)));
  
  auto blockbuilder = BlockBuilder(ids, edges);
  
  std::cout<<blockbuilder;
  /*
  for (const auto& edge : edges){
    Edge e =edge.second;
    //add linkage info into the nodes dictionary
    if  (e.isLinked())  //this is actually an undirected link - OK for undirected searches
      m_localNodes[e.id1()].addChild(m_localNodes[e.id2()]);
    std::cout << e;
    
    
  }
  
  for (auto & n : m_localNodes)
  {
    std::cout << "Node" << n.first<< " address " << &n.second<< std::endl;
    for (auto c : n.second.getChildren())
      std::cout << " C" << c->getValue() << " address " << &c<< std::endl;
    for (auto c : n.second.getParents())
      std::cout << " P" << c->getValue()<< " address " << &c<< std::endl;
  }
  
  DAG::BFSVisitor<PFNode> bfs;
  for (auto i : ids) {
    std::cout <<"Node "<<Identifier::getUniqueID(i)-1<<std::endl;
  for (auto n : bfs.traverseUndirected(m_localNodes[i])) {
    long id = Identifier::getUniqueID(n->getValue());
    if (Identifier::isEcal(n->getValue())|| Identifier::isHcal(n->getValue()))
      std::cout << id - 1  << " :CLUSTER"  <<
      std::endl; //subtract 1 to match the node number
    else if (Identifier::isTrack(n->getValue()))
      std::cout <<  id - 1 << " :TRACK"  <<
      std::endl; //subtract 1 to match the node number
    else
      std::cout  << id-1<<" :other"<<std::endl;
  }
  }*/
  //PFBlock block(ids, edges);
  //PFBlock block2(ids2, edges);
  

  
  return 0;
}


