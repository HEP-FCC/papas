

#include "GraphBuilder.h"
//
//  GraphBuilder.cpp
//  fastsim
//
//  Created by Alice Robson on 07/04/16.
//
//

#include "GraphBuilder.h"
#include "Identifier.h"
#include "Edge.h"
#include "directedacyclicgraph.h"
#include "FloodFill.h"

//Allow optional parameters where construction arguments are references

Edges emptyEdges;

GraphBuilder::GraphBuilder(IDs ids,
                           Edges& edges) :
m_elementIDs(ids),
m_edges(edges)
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
  
  DAG::FloodFill<longID> FFill;
  
  //traverse does the work and returns a vector of connected node groups
  for (auto & group : FFill.traverse(m_localNodes)) {
    
    //each of the nodevectors is about to become a separate block
    //we need the vector of ids and the map of edges in order to make the block
    IDs subgraph;
    for (auto& node : group) {
      subgraph.push_back(node->getValue());
    }
    m_subGraphs.push_back(subgraph);
  }
  
}


GraphBuilder::GraphBuilder() :
m_edges(emptyEdges),
m_localNodes(emptyNodes)
{
  
}


GraphBuilder& GraphBuilder::operator=(const GraphBuilder& b)
{
  m_elementIDs=b.m_elementIDs;
  m_edges = b.m_edges;
  m_subGraphs = b.m_subGraphs;
  return *this;
}

void GraphBuilder::sortIDs(std::vector<longID>& ids)
{
  std::sort(ids.begin(), ids.end(), [](longID a, longID b) -> bool
            { return Identifier::typeShortCode(a) < Identifier::typeShortCode(b); } );
}


/*std::ostream& operator<<(std::ostream& os, const GraphBuilder& builder) {
  //TODO move to helper
  for (auto graph : builder.m_subGraphs) {
    os  << graph << std::endl;
  }
  return os;
}*/

/*void GraphBuilder::sortIDs(std::vector<longID>& ids) // sorts by type and energy
 {//TODO move to helper
 std::sort( ids.begin(), ids.end(), [this] (longID a, longID b) { return this->m_pfEvent.compare(a,b);});
 }*/



/*bool GraphBuilder::compareEdges(long long key1, long long key2, longID uniqueid) const//TODO check direction of sort
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



int test_GraphBuilder() {
  
  
  longID id1 = Identifier::makeECALClusterID();
  longID id2 = Identifier::makeHCALClusterID();
  longID id3 = Identifier::makeTrackID();
  
  longID id4 = Identifier::makeECALClusterID();
  longID id5 = Identifier::makeHCALClusterID();
  longID id6 = Identifier::makeTrackID();
  
  std::vector<longID> ids {id1,id2,id3, id4,id5,id6};
  
  Edge edge =  Edge(id1, id2, false, 0.00023);
  Edge edge1 = Edge(id1, id3, true, 10030.0);
  Edge edge2 = Edge(id2, id3, true, 0.00005);
  
  Edge edge4 = Edge(id4, id5, false, 3.1234);
  Edge edge5 = Edge(id4, id6, true, 0.1234);
  Edge edge6 = Edge(id5, id6, true, 123.0);
  
  std::unordered_map<long long, class Edge> edges;
  
  edges.emplace(edge.key(),  edge);
  edges.emplace(edge1.key(), edge1);
  edges.emplace(edge2.key(), edge2);
  edges.emplace(edge4.key(), edge4);
  edges.emplace(edge5.key(), edge5);
  edges.emplace(edge6.key(), edge6);
  
  
  
  
  
  auto graphbuilder = GraphBuilder(ids, edges);
  
  //std::cout<<graphbuilder;
  return 0;
}


