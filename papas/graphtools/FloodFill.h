#ifndef TOOLS_FLOODFILL_H
#define TOOLS_FLOODFILL_H

#include "papas/graphtools/DirectedAcyclicGraph.h"

#include <map>
#include <unordered_set>
#include <vector>


namespace DAG {
///
///  @author  Alice Robson
///  @date    2016-04-12

/// FloodFill creates blocks of connected elements
///  @tparam T is the content of a Node
///
///   FLOODFILL uses the DAG BFSVisitor to find connected groups of nodes
///
/// Example usage:
/**
@code
  typedef DAG::Node<long> PFNode;
  typedef std::unordered_map<long,PFNode> Nodes;
  Nodes myNodes;
  long id1, id2;
  id1=1;
  id2=2;

  DAG::FloodFill<long> FFill;
  myNodes.emplace(id1, PFNode(id1));
  myNodes.emplace(id2, PFNode(id2));
  myNodes[id1].addChild(myNodes[id2]);


  for (auto & nodevector : FFill.traverse(myNodes)) {
    std::cout << "Block: ";
    for (auto &node : nodevector) {
      std::cout<< node->value()<< ", " ;
    }
  }
  std::cout<< std::endl;
@endcode
*/
template <typename T>
class FloodFill {

  typedef Node<T> TNode;                             ///< templated Node
  typedef std::map<T, TNode> Nodemap;                ///< Map of concrete TNodes, indexed by the the template item T
  typedef std::unordered_set<const TNode*> Nodeset;  ///< Set of pointers to TNodes
  typedef std::vector<const TNode*> Nodevector;      ///< Vector of pointers to TNodes

public:
  FloodFill();  ///< constructor
  std::vector<Nodevector>
  traverse(Nodemap&);  ///< runs floodfull algorithm; each element of return vector is a group of connected nodes

private:
  Nodeset m_visited;  ///< Nodes that have been visited (reset each time a traversal is made)
};

template <typename T>
FloodFill<T>::FloodFill() {}

template <typename T>
std::vector<typename FloodFill<T>::Nodevector> FloodFill<T>::traverse(FloodFill<T>::Nodemap& nodes) {
  std::vector<Nodevector> resultsVector;

  m_visited.clear();
  BFSVisitor<TNode> bfs;

  for (const auto& elem : nodes) {

    if (m_visited.find(&elem.second) != m_visited.end()) continue;  // already done this node so skip the rest

    // do a BFS search on any node that has not yet been visited
    Nodevector result = bfs.traverseUndirected(elem.second);
    for (const TNode* n : result)
      m_visited.insert(n);  // mark these as visited

    resultsVector.push_back(result);
  }
  return resultsVector;  // Move
}
}

#endif /* FloodFill_h */
