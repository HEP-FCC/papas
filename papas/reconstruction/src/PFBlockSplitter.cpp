
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/graphtools/Distance.h"
#include "papas/graphtools/PapasEventRuler.h"
#include "papas/reconstruction/BlockBuilder.h"
#include "papas/reconstruction/PFBlock.h"
#include "papas/reconstruction/PFBlockSplitter.h"

// temp
#include "papas/datatypes/Identifier.h"

namespace papas {

PFBlockSplitter::PFBlockSplitter(const PapasEvent& papasEvent, char blockSubtype, Blocks& simplifiedblocks,
                                 Nodes& history)
    : m_papasEvent(papasEvent), m_simplifiedBlocks(simplifiedblocks), m_history(history) {
  const auto& blocks = m_papasEvent.blocks(blockSubtype);
  auto blockids = m_papasEvent.collectionIds<Blocks>(blocks);
#if WITHSORT
  std::sort(blockids.begin(), blockids.end());
#endif
  // go through each block and see if it can be simplified
  // in some cases it will end up being split into smaller blocks
  // Note that the old block will be marked as disactivated
  for (auto bid : blockids) {
    simplifyBlock(blocks.at(bid));
  }
}

void PFBlockSplitter::simplifyBlock(const PFBlock& block) {
  /* Block: a block which contains list of element ids and set of edges that connect them
        The goal is to remove, if needed, some links from the block so that each track links to
   at most one hcal within a block. In some cases this may separate a block into smaller
   blocks (splitblocks). The BlockSplitter is used to add the new smaller block into m_simplifiedBlocks. If a block is
   unchanged its content will be copied into a new Block with a new Block Id and stored in m_simplifiedBlocks.
   If history_nodes are provided then the history will be updated. Split blocks will
   have the tracks and cluster elements as parents, and also the original block as a parent
   */

  Edges toUnlink = findEdgesToUnlink(block);  // find any Edges that can be removed

  if (toUnlink.size() == 0) {
    // no change to this block
    // make a copy of the block and put it in the simplified blocks
    Edges newedges = block.edges();  // copy edges
    auto newblock = PFBlock(block.elementIds(), newedges, 's');
    m_simplifiedBlocks.emplace(newblock.id(), std::move(newblock));
    //amend history
    makeHistoryLinks(block.elementIds(), {newblock.id()} , m_history);
    
      
    // TODO m_history
    // if there is something to unlink then use the BlockBuilder to create new blocks
    // create a new modified set of edges with some edges unlinked
  } else {
    Edges modifiedEdges;
    for (auto edge : block.edges()) {  // copying edges
      Edge e = edge.second;
      if (toUnlink.find(edge.first) != toUnlink.end()) {
        e.setLinked(false);
      }
      modifiedEdges.emplace(e.key(), std::move(e));
    }
    // Blockbuilder will add the blocks it creates into m_simplifiedBlocks
    auto bbuilder = BlockBuilder(block.elementIds(), std::move(modifiedEdges), m_history, m_simplifiedBlocks, 's');
  }
}

Edges PFBlockSplitter::findEdgesToUnlink(const PFBlock& block) {
  Edges toUnlink;
  Ids ids = block.elementIds();
  if (ids.size() > 1) {
    std::vector<Edge::EdgeKey> linkedEdgeKeys;
    bool firstHCAL;
    double minDist = -1;
    for (auto id : ids) {
      if (Identifier::isTrack(id)) {
        linkedEdgeKeys = block.linkedEdgeKeys(id, Edge::EdgeType::kHcalTrack);
        if (linkedEdgeKeys.size() > 0) {
          firstHCAL = true;
          for (auto elem : linkedEdgeKeys) {  // find minimum distance between track and Hcals
            if (firstHCAL) {
              minDist = block.findEdge(elem).distance();
              firstHCAL = false;
            } else {
              minDist = fmin(minDist, block.findEdge(elem).distance());
            }
          }
          // unlink anything that is greater than minimum distance
          for (auto elem : linkedEdgeKeys) {
            if (block.findEdge(elem).distance() > minDist) {  // (could be more than one at zero distance)
              toUnlink[elem] = block.findEdge(elem);          // should toUnlink be list of keys rather than edges
            }
          }
        }
      }
      // TODO doublecheck this has gone in C++
      /*else if (Identifier::isEcal(id)) {
        // this is now handled  elsewhere in  Ruler::distance and so could be removed
        // remove all ecal-hcal links. ecal linked to hcal give rise to a photon anyway.
        linkedEdgeKeys = block.linkedEdgeKeys(id, Edge::EdgeType::kEcalHcal);  //"ecal_hcal")
        for (auto elem : linkedEdgeKeys) {
          toUnlink[elem] = block.findEdge(elem);
        }
      }*/
    }
  }
  return toUnlink;  // move
}

}  // end namespace papas