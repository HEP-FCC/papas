
#include "papas/reconstruction/PFBlockSplitter.h"
#include "papas/reconstruction/BlockSplitter.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/Distance.h"
#include "papas/graphtools/PapasEventRuler.h"
#include "papas/reconstruction/PFBlock.h"
#include "papas/datatypes/PapasEvent.h"

// temp
#include "papas/datatypes/Identifier.h"

namespace papas {
  
  
  
  PFBlockSplitter::PFBlockSplitter(const PapasEvent& papasEvent, char blockSubtype, Blocks& simplifiedblocks, Nodes& history)
  : m_papasEvent(papasEvent), m_blocks(simplifiedblocks), m_history(history), m_uniqueIds() {
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
     history_nodes: optional dictionary of Nodes with element identifiers in each node
     
     returns None or a dictionary of new split blocks
     
     The goal is to remove, if needed, some links from the block so that each track links to
     at most one hcal within a block. In some cases this may separate a block into smaller
     blocks (splitblocks). The BlockSplitter is used to return the new smaller blocks.
     If history_nodes are provided then the history will be updated. Split blocks will
     have the tracks and cluster elements as parents, and also the original block as a parent
     */
    
    Ids ids = block.elementIds();
    
    if (ids.size() <= 1) {  // if block is just one element therer are no links to remove
      Edges newedges = block.edges();
      auto newblock = PFBlock(block.elementIds(), newedges,  's');
      m_blocks.emplace(newblock.id(), std::move(newblock));
    }
    
    /* work out any links that need to be removed
     - for tracks unink all hcals except the closest hcal
     - for ecals unlink hcals */
    
    
    Edges toUnlink;  // TODO think about copy
    std::vector<Edge::EdgeKey> linkedEdgeKeys;
    bool firstHCAL;
    double minDist = -1;
    for (auto id : ids) {
      if (Identifier::isTrack(id)) {
        linkedEdgeKeys = block.linkedEdgeKeys(id, Edge::EdgeType::kHcalTrack);
        if (linkedEdgeKeys.size() > 0) {
          firstHCAL = true;
          // find minimum distance
          for (auto elem : linkedEdgeKeys) {
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
      } else if (Identifier::isEcal(id)) {
        // this is now handled  elsewhere in  Ruler::distance and so could be removed
        // remove all ecal-hcal links. ecal linked to hcal give rise to a photon anyway.
        linkedEdgeKeys = block.linkedEdgeKeys(id, Edge::EdgeType::kEcalHcal);  //"ecal_hcal")
        for (auto elem : linkedEdgeKeys) {
          toUnlink[elem] = block.findEdge(elem);
        }
      }
    }
    
    // if there is something to unlink then use the BlockSplitter
    if (toUnlink.size() > 0) {
      BlockSplitter(toUnlink, block, m_history, m_blocks);
    }
    
  }

  
  
  
}  // end namespace papas
