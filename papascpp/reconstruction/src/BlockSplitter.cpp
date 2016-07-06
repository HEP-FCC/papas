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
  m_blocks = bbuilder.blocks();
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

int test_BlockSplitter() {
  Id::Type id1 = Id::makeEcalId();
  Id::Type id2 = Id::makeHcalId();
  Id::Type id3 = Id::makeTrackId();

  Id::Type id4 = Id::makeEcalId();
  Id::Type id5 = Id::makeHcalId();
  Id::Type id6 = Id::makeTrackId();

  std::vector<Id::Type> ids{id1, id2, id3, id4, id5, id6};

  Edge edge = Edge(id1, id2, false, 0.00023);
  Edge edge1 = Edge(id1, id3, true, 10030.0);
  Edge edge2 = Edge(id2, id3, true, 0.00005);

  Edge edge4 = Edge(id4, id5, false, 3.1234);
  Edge edge5 = Edge(id4, id6, true, 0.1234);
  Edge edge6 = Edge(id5, id6, true, 123.0);

  Edges edges;

  edges.emplace(edge.key(), std::move(edge));
  edges.emplace(edge1.key(), std::move(edge1));
  edges.emplace(edge2.key(), std::move(edge2));
  edges.emplace(edge4.key(), std::move(edge4));
  edges.emplace(edge5.key(), std::move(edge5));
  edges.emplace(edge6.key(), std::move(edge6));

  // create history nodes
  Nodes historyNodes;
  for (auto id : ids)
    historyNodes.emplace(id, std::move(PFNode(id)));

  Nodes emptyNodes;
  auto blockbuilder = BlockBuilder(ids, edges, historyNodes);

  Edges to_unlink;
  to_unlink[edge1.key()] = edge1;
  for (auto& block : blockbuilder.blocks()) {
    auto blocksplitter = BlockSplitter(to_unlink, block.second, emptyNodes);
    std::cout << block.second;
    std::cout << blocksplitter;
  }

  return 0;
}

}  // end namespace papas
