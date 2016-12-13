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
#include "papas/utility/PDebug.h"
#include "papas/reconstruction/PFBlock.h"

namespace papas {

BlockBuilder::BlockBuilder(const Ids& ids, Edges&& edges, Nodes& historynodes, char subtype)
    : GraphBuilder(ids, std::move(edges)), m_historyNodes(historynodes), m_blocks() {
  makeBlocks(subtype);
}

void BlockBuilder::makeBlocks(char subtype) {
  /** uses the base class  GraphBuilder to work out which elements are connected (a subGraph)
   Each subGraph will be used to make a new PFBlock
   */

  for (auto& elementIds : m_subGraphs) {
    if (elementIds.size() > 1) {
      sortIds(elementIds);  // TODO allow sorting by energy using a helper class
    }
    auto block = PFBlock(elementIds, m_edges, subtype);  // make the block
    PDebug::write("Made {}", block);
    // put the block in the unordered map of blocks using move
    IdType id = block.uniqueId();
    m_blocks.emplace(id, std::move(block));

    // update the history nodes (if they exist)
    if (m_historyNodes.size() > 0) {
      // make a new history node for the block and add into the history Nodes
      m_historyNodes.emplace(id, std::move(PFNode(id)));  // move
      auto storedBlocknode = m_historyNodes[id];
      // add in the links between the block elements and the block
      for (auto elemid : m_blocks[id].elementIds()) {
        m_historyNodes[elemid].addChild(storedBlocknode);
      }
    }
  }
}

std::ostream& operator<<(std::ostream& os, const BlockBuilder& builder) {
  // TODO move to helper
  for (const auto& block : builder.m_blocks) {
    os << block.second << std::endl;
  }
  return os;
}

/* keep for now as might just be useful if we want to sort by energy
 bool BlockBuilder::compareEdges(long long key1, long long key2, IdType uniqueid) const//TODO check direction of sort
 {
 //sort by the type eg ecal hcal
 // and then in order of decreasing energy
 Edge e1 = m_edges.find(key1)->second; // should part of this be a static function in Edges?
 Edge e2 = m_edges.find(key2)->second;
 if (e1.distance() < e2.distance())
 return true;
 else if (e1.distance() > e2.distance())
 return false;
 // the distance for edge1 and edge 2 is same
 // so return based on edgetype and end energy comparison for the items
 // at the other end from uniqueId
 double energy1 = m_pfEvent.energy(e1.otherid(uniqueid));
 double energy2 = m_pfEvent.energy(e2.otherid(uniqueid));

 return (energy1 > energy2) ;
 }*/

}  // end namespace papas
