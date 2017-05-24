#include "papas/reconstruction/SimplifyPFBlocks.h"

#include "papas/datatypes/Event.h"
#include "papas/reconstruction/BuildPFBlocks.h"
#include "papas/utility/PDebug.h"

namespace papas {

void simplifyPFBlocks(const Event& event, char blockSubtype, Blocks& simplifiedblocks, Nodes& history) {

  auto blockids = event.collectionIds(IdCoder::ItemType::kBlock, blockSubtype);
  // go through each block and see if it can be simplified
  // in some cases it will end up being split into smaller blocks
  // Note that the old block will be marked as disactivated
  for (auto bid : blockids) {
    const auto& block = event.block(bid);
    PDebug::write("Splitting {}", block);
    auto unlink = edgesToUnlink(block);
    simplifyPFBlock(unlink, block, simplifiedblocks, history);
  }
}

void simplifyPFBlock(const Edges& toUnlink, const PFBlock& block, Blocks& simplifiedBlocks, Nodes& history) {
  // take a block, unlink some of the edges and
  // create smaller blocks or a simplified blocks
  // or if nothing has changed take a copy of the original block
  if (toUnlink.size() == 0) {
    // no change needed, just make a copy of block
    PFBlock newblock(block.elementIds(), block.edges(), simplifiedBlocks.size(), 's'); //will copy edges and ids
    PDebug::write("Made {}", newblock);
    auto id  = newblock.id();
    simplifiedBlocks.emplace(id, std::move(newblock));
    // update history
    makeHistoryLinks(block.elementIds(), {id}, history);
  } else {
    Edges modifiedEdges;
    for (auto edge : block.edges()) {  // copying edges
      Edge e = edge.second;
      if (toUnlink.find(edge.first) != toUnlink.end()) {
        e.setLinked(false);
      }
      modifiedEdges.emplace(e.key(), e);
    }
    // create new blocks and add into simplifiedBlocks
    buildPFBlocks(block.elementIds(), modifiedEdges, 's', simplifiedBlocks, history);
  }
}

Edges edgesToUnlink(const PFBlock& block) {
  Edges toUnlink;
  Ids ids = block.elementIds();
  if (ids.size() > 1) {
    Ids linkedIds;
    bool firstHCAL;
    double minDist = -1;
    for (auto id : ids) {
      if (IdCoder::isTrack(id)) {
        linkedIds = block.linkedIds(id, Edge::EdgeType::kHcalTrack);
        if (linkedIds.size() > 0) {
          firstHCAL = true;
          for (auto elem : linkedIds) {  // find minimum distance between track and Hcals
            if (firstHCAL) {
              minDist = block.edge(id, elem).distance();
              firstHCAL = false;
            } else {
              minDist = fmin(minDist, block.edge(id, elem).distance());
            }
          }
          // unlink anything that is greater than minimum distance
          for (auto elem : linkedIds) {
            auto key = Edge::makeKey(id, elem);
            auto& edge = block.edge(id, elem);
            if (edge.distance() > minDist) {  // (could be more than one at zero distance)
              toUnlink[key] = edge;           // should toUnlink be list of keys rather than edges
            }
          }
        }
      }
    }
  }
  return toUnlink;  // move
}

}  // end namespace papas
