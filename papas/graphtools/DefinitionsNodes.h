//
//  DefinitionsNodes.h
//  papas
//
//  Created by Alice Robson on 12/05/16.
//
//

#ifndef DefinitionsNodes_h
#define DefinitionsNodes_h

#include "papas/datatypes/Identifier.h"
#include "papas/graphtools/DirectedAcyclicGraph.h"
#include <map>
namespace papas {

typedef DAG::Node<IdType> PFNode;
typedef std::map<IdType, PFNode> Nodes;
typedef std::list<const Nodes*> ListNodes;  ///< collection of Nodes

inline PFNode& findOrMakeNode(IdType id, Nodes& history) {
  if (history.empty() || (history.find(id) == history.end()) ) {
      PFNode newnode(id);
      history.emplace(id, newnode);
  }
  return history.at(id);
}

inline void makeHistoryLink(IdType parentid, IdType childid, Nodes& history) {
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
      std::cout << Identifier::pretty(node.first) << ":" << Identifier::pretty(cnode->value()) << "   ";
}

}  // end namespace papas

#endif /* DefinitionsNodes_h */
