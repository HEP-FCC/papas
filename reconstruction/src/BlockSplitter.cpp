//
//  BlockSplitter.cpp
//  fastsim
//
//  Created by Alice Robson on 12/04/16.
//
//

#include "BlockSplitter.h"
#include "Definitions.h"
#include "Edge.h"
#include "PFBlock.h"

namespace papas {

BlockSplitter::BlockSplitter(const Edges& unlinkEdges, PFBlock& block, Nodes& historynodes) : m_blocks() {
  Edges modifiedEdges;
  for (auto edge : block.edges()) {  // copy edges
    Edge e = edge.second;
    if (unlinkEdges.find(edge.first) != unlinkEdges.end()) {
      e.setLinked(false);
    }
    modifiedEdges.emplace(e.key(), std::move(e));
  }

  BlockBuilder bbuilder{block.elementIds(), modifiedEdges, historynodes};
  m_blocks = bbuilder.moveBlocks();
  block.setActive(false);
}

std::ostream& operator<<(std::ostream& os, const BlockSplitter& builder) {
  // TODO move to helper?
  for (auto const& block : builder.m_blocks) {
    os << block.second << std::endl;
  }
  return os;
}

/*
 #Plan 1 was to link new blocks to original blocks. But this makes use of the history nodes
 # rather cumbersome. needs to be debated further removed for now
 ##the new blocks are subblocks of the original block
 ##so keep note of this in the history (at least for now)
 ## nb in some cases the new block will be the same as the original block although
 ## the edges will have changed (for python these changes will also be seen in
 ## the original block)
 #if (self.history_nodes != None ):
 #for subblockid in self.blocks.keys():
 #if len(block.element_uniqueids)>5:
 #print "*****split", block
 #pass
 ##self.history_nodes[block.uniqueid].add_child(history_nodes[subblockid])

 #set the original block to be inactive*/
#include "Id.h"

}  // end namespace papas
