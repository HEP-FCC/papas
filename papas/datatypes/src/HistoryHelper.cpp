
#include "papas/datatypes/HistoryHelper.h"
#include "papas/datatypes/Event.h"
#include "papas/graphtools/DirectedAcyclicGraph.h"

namespace papas {

HistoryHelper::HistoryHelper(const Event& event) : m_event(event) {}

Ids HistoryHelper::linkedIds(IdType id, DAG::enumVisitType direction) const {
  const auto history = m_event.history();
  auto& startnode = history->at(id);
  auto bfs = DAG::BFSRecurseVisitor<PFNode>();
  auto nodes = bfs.traverseNodes(startnode, direction);
  Ids ids;
  for (auto node : nodes) {
    ids.push_back(node->value());
  }
  return ids;
}

Ids HistoryHelper::linkedIds(IdType id, const std::string& typeAndSubtype, DAG::enumVisitType direction) const {
  auto ids = linkedIds(id, direction);
  auto fids = filteredIds(ids, typeAndSubtype);
  return fids;
}

Ids HistoryHelper::filteredIds(Ids ids, const Identifier::ItemType type, const Identifier::SubType subtype) const {
  Ids matchedIds;
  for (auto id : ids) {
    if (Identifier::itemType(id) == type && Identifier::subtype(id) == subtype) {
      matchedIds.push_back(id);
    }
  }
  return matchedIds;
}

Ids HistoryHelper::filteredIds(Ids ids, const std::string& typeAndSubtype) const {
  Ids matchedIds;
  for (auto id : ids) {
    if (Identifier::typeAndSubtype(id) == typeAndSubtype) {
      matchedIds.push_back(id);
    }
  }
  return matchedIds;
}

}  // end namespace papas