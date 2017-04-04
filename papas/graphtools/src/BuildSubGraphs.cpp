#include "papas/graphtools/BuildSubGraphs.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/Edge.h"
#include "papas/graphtools/FloodFill.h"
#include "papas/utility/PDebug.h"

namespace papas {

std::list<Ids> buildSubGraphs(const Ids& ids, const Edges& edges) {
  std::list<Ids> subGraphs;
  Nodes localNodes;
  // create local nodes ready to use to make the blocks
  for (auto id : ids) {
    localNodes.emplace(id, PFNode(id));
  }
  // use the edge information to say what is linked and add this into the local blocks
  for (const auto& edge : edges) {
    const Edge& e = edge.second;
    if (e.isLinked()) {  // note this is an undirected link - OK for undirected searches
      localNodes[e.endIds()[0]].addChild(localNodes[e.endIds()[1]]);
    }
  }
  DAG::FloodFill<Identifier> FFill;
  // traverse does the work and returns a vector of connected node groups
  for (const auto& group : FFill.traverse(localNodes)) {
    // each of the nodevectors is about to become a separate block
    // we need the vector of ids and the map of edges in order to make the block
    Ids subgraph;
    for (const auto& node : group) {
      subgraph.push_back(node->value());
    }
#if WITHSORT
    subgraph.sort(std::greater<uint64_t>());  // sort in descending order
#endif
    subGraphs.push_back(subgraph);
  }
  return subGraphs;
}

}  // end namespace papas
