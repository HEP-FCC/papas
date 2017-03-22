#include "papas/graphtools/GraphBuilder.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/Edge.h"
#include "papas/graphtools/FloodFill.h"
#include "papas/utility/PDebug.h"

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
      m_localNodes[e.endIds()[0]].addChild(m_localNodes[e.endIds()[1]]);
      }
  }
  DAG::FloodFill<Identifier> FFill;
  // traverse does the work and returns a vector of connected node groups
  for (const auto& group : FFill.traverse(m_localNodes)) {
    // each of the nodevectors is about to become a separate block
    // we need the vector of ids and the map of edges in order to make the block
    Ids subgraph;
    for (const auto& node : group) {
      subgraph.push_back(node->value());
    }
    #if WITHSORT
    sortIds(subgraph); //sort in descending order
    #endif
    m_subGraphs.push_back(subgraph);
  }
}

void GraphBuilder::sortIds(Ids& ids) {
  ids.sort(std::greater<uint64_t>()); //sort in descending order
}

}  // end namespace papas
