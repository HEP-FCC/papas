#include "papas/reconstruction/SimplifyPFBlocks.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Event.h"
#include "papas/datatypes/IdCoder.h"
#include "papas/reconstruction/BuildPFBlocks.h"

namespace papas {

void simplifyPFBlocks(const Event& event, char blockSubtype, Blocks& simplifiedblocks, Nodes& history) {
  
  auto blockids = event.getCollectionIds(IdCoder::ItemType::kBlock, blockSubtype);
  // go through each block and see if it can be simplified
  // in some cases it will end up being split into smaller blocks
  // Note that the old block will be marked as disactivated
  for (auto bid : blockids) {
    const auto & block =event.block(bid);
    PDebug::write("Splitting {}", block);
    auto unlink = findEdgesToUnlink(block);
    simplifyPFBlock(unlink, block, simplifiedblocks, history);
  }
}

void simplifyPFBlock(const Edges& toUnlink, const PFBlock& block, Blocks& simplifiedBlocks, Nodes& history) {
  if (toUnlink.size() == 0) {
    // no change to this block
    // make a copy of the block and put it in the simplified blocks
    Edges newedges = block.edges();  // copy edges
    PFBlock newblock(block.elementIds(), newedges, simplifiedBlocks.size(), 's');
    PDebug::write("Made {}", newblock);
    simplifiedBlocks.emplace(newblock.id(), std::move(newblock));
    // update history
    makeHistoryLinks(block.elementIds(), {newblock.id()}, history);

  } else {
    Edges modifiedEdges;
    for (auto edge : block.edges()) {  // copying edges
      Edge e = edge.second;
      if (toUnlink.find(edge.first) != toUnlink.end()) {
        e.setLinked(false);
      }
      modifiedEdges.emplace(e.key(), std::move(e));
    }
    // create new blocks and add into simplifiedBlocks
    buildPFBlocks(block.elementIds(), std::move(modifiedEdges), 's', simplifiedBlocks, history);
  }
}

Edges findEdgesToUnlink(const PFBlock& block) {
  Edges toUnlink;
  Ids ids = block.elementIds();
  if (ids.size() > 1) {
    Ids linkedIds;//std::set<Edge::EdgeKey> linkedIds; //unordered?
    bool firstHCAL;
    double minDist = -1;
    for (auto id : ids) {
      if (IdCoder::isTrack(id)) {
        linkedIds = block.linkedIds(id, Edge::EdgeType::kHcalTrack);
        if (linkedIds.size() > 0) {
          firstHCAL = true;
          for (auto elem : linkedIds) {  // find minimum distance between track and Hcals
            if (firstHCAL) {
              minDist = block.findEdge(Edge::makeKey(id, elem)).distance();
              firstHCAL = false;
            } else {
              minDist = fmin(minDist, block.findEdge(Edge::makeKey(id, elem)).distance());
            }
          }
          // unlink anything that is greater than minimum distance
          for (auto elem : linkedIds) {
            auto key = Edge::makeKey(id, elem);
            if (block.findEdge(key).distance() > minDist) {  // (could be more than one at zero distance)
              toUnlink[key] = block.findEdge(key);           // should toUnlink be list of keys rather than edges
            }
          }
        }
      }
    }
  }
  return toUnlink;  // move
}

}  // end namespace papas
