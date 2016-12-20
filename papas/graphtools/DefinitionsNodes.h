//
//  DefinitionsNodes.h
//  papas
//
//  Created by Alice Robson on 12/05/16.
//
//

#ifndef DefinitionsNodes_h
#define DefinitionsNodes_h

#include "papas/graphtools/DirectedAcyclicGraph.h"
#include "papas/datatypes/Identifier.h"
#include <map>
#include <vector>
namespace papas {

typedef DAG::Node<IdType> PFNode;
typedef std::map<IdType, PFNode> Nodes;
typedef std::list< const Nodes*> ListNodes;  ///< collection of Nodes

}  // end namespace papas

#endif /* DefinitionsNodes_h */
