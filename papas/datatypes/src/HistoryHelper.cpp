
#include "papas/datatypes/HistoryHelper.h"
#include "papas/datatypes/PapasEvent.h"

namespace papas {
 
  HistoryHelper::HistoryHelper(const PapasEvent& papasEvent): m_papasEvent(papasEvent) {
    
  }
  
   Ids HistoryHelper::linkedIds(IdType id) const {
    const Nodes& history = m_papasEvent.history();
    auto startnode = history.at(id);
    auto bfs = DAG::BFSVisitor<PFNode>();
    auto nodes = bfs.traverseUndirected(startnode);
    Ids ids;
    for (auto node : nodes){
      ids.push_back(node->value());
    }
    return ids;
  }
  
  
  Ids HistoryHelper::filteredIds(Ids ids, const Identifier::ItemType type, const Identifier::SubType subtype ) const{
    Ids matchedIds;
    for (auto id : ids) {
      if ( Identifier::itemType(id) == type &&Identifier::subtype(id) == subtype) {
        matchedIds.push_back(id);
      }
    }
    return matchedIds;
  }


} //end namespace papas
