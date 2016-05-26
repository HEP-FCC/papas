//
//  PFBlockBuilder.cpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//
#include "PFBlockBuilder.h"
#include "BlockBuilder.h"
#include "PFBlock.h"
#include "PFEvent.h"
#include "Distance.h"
#include "Ruler.h"
#include "DefinitionsCollections.h"

namespace papas {

PFBlockBuilder::PFBlockBuilder(PFEvent& pfevent) :
  m_pfEvent(pfevent),
  m_historyNodes(pfevent.historyNodes()),
  m_uniqueIds(pfevent.elementIds())
{
  

  if (m_historyNodes.size()==0) {
    //create local nodes ready to use to make the blocks
    for (auto id : m_uniqueIds)
      m_historyNodes.emplace(id, PFNode(id));
  }
  
  //TODO thhink hard about best way to deal with distance /ruler / edges etc
  //compute edges between each pair of nodes
  Edges edges;
  Ruler ruler(pfevent);
  for (auto id1 : m_uniqueIds) {
    for (auto id2 : m_uniqueIds) {
      if (id1 < id2) {
        Distance dist = ruler.distance(id1,id2);
        Edge edge{id1, id2, dist.isLinked(), dist.distance()};
        //the edge object is added into the edges dictionary
        edges.emplace(edge.key(),std::move(edge));
      }
    }
  }
  m_blocks = std::move(BlockBuilder(m_uniqueIds, edges, m_historyNodes).blocks());
  for (auto & b : m_blocks) {
    std::cout << b.second;
  }
 
}

} // end namespace papas

