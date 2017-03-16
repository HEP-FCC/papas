//
//  BlockBuilder.cpp
//  fastsim
//
//  Created by Alice Robson on 07/04/16.
//
//
#include "papas/reconstruction/BlockBuilder.h"
#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DirectedAcyclicGraph.h"
#include "papas/graphtools/FloodFill.h"
#include "papas/reconstruction/PFBlock.h"
#include "papas/utility/PDebug.h"

namespace papas {

BlockBuilder::BlockBuilder(const Ids& ids, Edges&& edges, Nodes& history, Blocks& blocks, char subtype)
    : GraphBuilder(ids, std::move(edges)), m_history(history), m_blocks(blocks) {
  makeBlocks(subtype);
}

void BlockBuilder::makeBlocks(char blockSubtype) {
  /* uses the base class  GraphBuilder to work out which elements are connected (a subGraph)
   Each subGraph will be used to make a new PFBlock
   */
  for (auto& elementIds : m_subGraphs) {
    if (elementIds.size() > 1) {
      sortIds(elementIds);
    }
    auto block = PFBlock(elementIds, m_edges, m_blocks.size(), blockSubtype);  // make the block
    PDebug::write("Made {}", block);
    // put the block in the unordered map of blocks using move
    IdType id = block.id();
    makeHistoryLinks(block.elementIds(), {id}, m_history);
    m_blocks.emplace(id, std::move(block));
  }
}

std::ostream& operator<<(std::ostream& os, const BlockBuilder& builder) {
  for (const auto& block : builder.m_blocks) {
    os << block.second << std::endl;
  }
  return os;
}

}  // end namespace papas
