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
  ids.sort(std::greater<int>()); //sort in descending order
#endif
}

}  // end namespace papas
