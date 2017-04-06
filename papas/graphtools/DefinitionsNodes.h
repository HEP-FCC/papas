#ifndef DefinitionsNodes_h
#define DefinitionsNodes_h

#include "papas/datatypes/Definitions.h"
#include "papas/graphtools/DirectedAcyclicGraph.h"
#include "papas/datatypes/IdCoder.h"

#include <map>
#include <list>

namespace papas {

typedef DAG::Node<Identifier> PFNode;
typedef std::map<Identifier, PFNode> Nodes;
typedef std::list<const Nodes*> ListNodes;  ///< collection of Nodes

inline PFNode& findOrMakeNode(Identifier id, Nodes& history) {
  if (history.empty() || (history.find(id) == history.end())) {
    PFNode newnode(id);
    history.emplace(id, newnode);
  }
  return history.at(id);
}

inline void makeHistoryLink(Identifier parentid, Identifier childid, Nodes& history) {
  findOrMakeNode(parentid, history).addChild(findOrMakeNode(childid, history));
}

inline void makeHistoryLinks(const Ids& parentids, const Ids& childids, Nodes& history) {
  for (const auto pid : parentids) {
    for (const auto cid : childids) {
      makeHistoryLink(pid, cid, history);
    }
  }
}

inline void printHistory(const Nodes& history) {
  for (const auto& node : history)
    for (const auto& cnode : node.second.children())
      std::cout << IdCoder::pretty(node.first) << ":" << IdCoder::pretty(cnode->value()) << "   ";
}

}  // end namespace papas

#endif /* DefinitionsNodes_h */
