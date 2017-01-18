
#include "papas/graphtools/GraphBuilder.h"
#include "papas/graphtools/Edge.h"
#include "papas/graphtools/FloodFill.h"
#include "papas/utility/PDebug.h"
#include "papas/datatypes/DefinitionsCollections.h"
namespace papas {

GraphBuilder::GraphBuilder(const Ids& ids, Edges&& edges) : m_edges(edges), m_elementIds(ids) {

  // create local nodes ready to use to make the blocks
  for (auto id : ids) {
    m_localNodes.emplace(id, PFNode(id));
  }

  // use the edge information to say what is linked and add this into the local blocks
  for (const auto& edge : m_edges) {
    const Edge& e = edge.second;
    if (e.isLinked()) {  // note this is an undirected link - OK for undirected searches
      m_localNodes[e.id1()].addChild(m_localNodes[e.id2()]);
      // PDebug::write("      Add Child {:9} to  Node {:9}",Id::pretty(e.id2()),Id::pretty(e.id1()));
    }
  }

  DAG::FloodFill<IdType> FFill;

  // traverse does the work and returns a vector of connected node groups
  for (auto& group : FFill.traverse(m_localNodes)) {
    // each of the nodevectors is about to become a separate block
    // we need the vector of ids and the map of edges in order to make the block
    Ids subgraph;
    for (auto& node : group) {
      subgraph.push_back(node->value());
    }
    m_subGraphs.push_back(subgraph);
  }
}

void GraphBuilder::sortIds(Ids& ids) {

#if WITHSORT
  ids.sort();
  ids.reverse();
#endif
}

}  // end namespace papas

/*void GraphBuilder::sortIds(std::vector<IdType>& ids) // sorts by type and energy
 {//TODO move to helper
 std::sort( ids.begin(), ids.end(), [this] (IdType a, IdType b) { return this->m_pfEvent.compare(a,b);});
 }*/

/*bool GraphBuilder::compareEdges(long long key1, long long key2, IdType uniqueid) const//TODO check direction of sort
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
