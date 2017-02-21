#include "papas/reconstruction/PFBlockBuilder.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/graphtools/Distance.h"
#include "papas/graphtools/PapasEventRuler.h"
#include "papas/reconstruction/BlockBuilder.h"
#include "papas/reconstruction/PFBlock.h"
#include "papas/datatypes/Identifier.h"

namespace papas {

PFBlockBuilder::PFBlockBuilder(const PapasEvent& papasEvent, const std::string& ecalTypeAndSubtype,
                               const std::string& hcalTypeAndSubtype, char trackSubtype, Blocks& blocks, Nodes& history)
    : m_papasEvent(papasEvent), m_blocks(blocks), m_history(history), m_uniqueIds() {

  const auto& ecals = m_papasEvent.clusters(ecalTypeAndSubtype);
  const auto& hcals = m_papasEvent.clusters(hcalTypeAndSubtype);
  const auto& tracks = m_papasEvent.tracks(trackSubtype);
  auto ids = m_papasEvent.collectionIds<Clusters>(ecals);
  for (auto id : m_papasEvent.collectionIds<Clusters>(hcals))
    ids.push_back(id);
  for (auto id : m_papasEvent.collectionIds<Tracks>(tracks))
    ids.push_back(id);

  Edges edges;
  PapasEventRuler ruler(m_papasEvent);
  for (auto id1 : ids) {
    for (auto id2 : ids) {
      if (id1 < id2) {
        Distance dist = ruler.distance(id1, id2);
        Edge edge{id1, id2, dist.isLinked(), dist.distance()};
        // the edge object is added into the edges dictionary
        edges.emplace(edge.key(), std::move(edge));
      }
    }
  }
  BlockBuilder bb(ids, std::move(edges), m_history, blocks, 'r');
}

}  // end namespace papas
