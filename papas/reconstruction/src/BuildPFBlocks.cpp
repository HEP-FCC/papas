#include "papas/reconstruction/BuildPFBlocks.h"
#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Event.h"
#include "papas/datatypes/IdCoder.h"
#include "papas/graphtools/BuildSubGraphs.h"
#include "papas/graphtools/Distance.h"
#include "papas/graphtools/EventRuler.h"
#include "papas/graphtools/FloodFill.h"
#include "papas/reconstruction/PFBlock.h"
#include "papas/utility/PDebug.h"

namespace papas {

void buildPFBlocks(const Event& event, IdCoder::SubType  ecalSubtype,  IdCoder::SubType hcalSubtype,
                   char trackSubtype, Blocks& blocks, Nodes& history) {

  const auto& ecals = event.clusters(IdCoder::ItemType::kEcalCluster, ecalSubtype);
  const auto& hcals = event.clusters(IdCoder::ItemType::kHcalCluster, hcalSubtype);
  const auto& tracks = event.tracks( trackSubtype);
  auto ecalids = event.getCollectionIds(IdCoder::ItemType::kEcalCluster, ecalSubtype);
  auto hcalids = event.getCollectionIds(IdCoder::ItemType::kHcalCluster, hcalSubtype);
  
  auto ids = event.getCollectionIds(IdCoder::ItemType::kTrack, trackSubtype);
  //the ids should all be in the right order, so I wonder what the most efficient way to merge them would be?
  ids.insert(hcalids.begin(), hcalids.end());
  ids.insert(ecalids.begin(), ecalids.end());
  
  Edges edges;
  EventRuler ruler(event);
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
  buildPFBlocks(ids, std::move(edges), 'r', blocks, history);
}

void buildPFBlocks(const Ids& ids, Edges&& edges, char subtype, Blocks& blocks, Nodes& history) {
  std::list<Ids> subGraphs = buildSubGraphs(ids, edges);
  for (const auto& elementIds : subGraphs) {
    PFBlock block(elementIds, edges, blocks.size(), subtype);  // make the block
    PDebug::write("Made {}", block);
    // put the block in the unordered map of blocks using move
    Identifier id = block.id();
    makeHistoryLinks(block.elementIds(), {id}, history);
    blocks.emplace(id, std::move(block));
  }
}

}  // end namespace papas
