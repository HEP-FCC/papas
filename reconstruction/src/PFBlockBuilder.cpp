//
//  PFBlockBuilder.cpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//
#include "PFBlockBuilder.h"
#include "BlockBuilder.h"
#include "DefinitionsCollections.h"
#include "Distance.h"
#include "EventRuler.h"
#include "PFBlock.h"
#include "PFEvent.h"

// temp
#include "Id.h"

namespace papas {

PFBlockBuilder::PFBlockBuilder(PFEvent& pfevent, Ids& ids)
    : m_pfEvent(pfevent), m_historyNodes(pfevent.historyNodes()), m_uniqueIds(ids) {

  if (m_historyNodes.size() == 0) {
    // create local nodes ready to use to make the blocks
    for (auto id : m_uniqueIds)
      m_historyNodes.emplace(id, PFNode(id));
  }

  // TODO think hard about best way to deal with distance /ruler / edges etc
  // compute edges between each pair of nodes
  Edges edges;
  EventRuler ruler(pfevent);
  for (auto id1 : m_uniqueIds) {
    for (auto id2 : m_uniqueIds) {
      if (id1 < id2) {
        Distance dist = ruler.distance(id1, id2);
        Edge edge{id1, id2, dist.isLinked(), dist.distance()};
        // the edge object is added into the edges dictionary
        edges.emplace(edge.key(), std::move(edge));
      }
    }
  }
  BlockBuilder bb(m_uniqueIds, edges, m_historyNodes);
  m_blocks = bb.moveBlocks();
}

}  // end namespace papas
