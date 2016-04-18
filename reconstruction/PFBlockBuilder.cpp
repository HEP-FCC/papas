//
//  PFBlockBuilder.cpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//

#include "PFBlockBuilder.h"

//import itertools
#include "BlockBuilder.h"
#include "PFBlock.h"
#include "Edge.h"
#include "PFEvent.h"
#include "Distance.h"
#include "Ruler.h"
#include "directedacyclicgraph.h"

class PFEvent;


class PFBlockBuilder  {
  /** Constructor
   
   * @param[in] ids : vector of unique identifiers eg of tracks, clusters etc
   * @param[in] edges : unordered_map of edges which contains all edges between the ids (and maybe more)
   *            an edge records the distance and links between two ids
   * @param[inout] historyNodes : optional unordered_map that describes which elements are parents of which blocks
   *                     if a history_nodes tree is provided then
   *                     the new history will be added into the exisiting history
   */
  PFBlockBuilder(PFEvent& pfevent);
  //BlockBuilder(); //needed for unordered_map to compile but not actually used
  //BlockBuilder& operator = (const BlockBuilder&) ;
  //const IDs elementIDs() const { return m_elementIDs;};///< return the blockbuilders element ids
  Blocks& blocks() {return m_blocks;}; ///<return the unordered map of the resulting blocks;
  friend std::ostream& operator<<(std::ostream& os, const BlockBuilder& blockbuilder); //TODO move to helper class
  
private:
  void makeBlocks(); // does the main work
  Blocks m_blocks;///< the blocks made by blockbuilder
  PFEvent& m_pfEvent;
  Nodes& m_historyNodes; ///<optional, allows history to be updated
  IDs m_uniqueIDs;
  //bool compareEdges( long long key1, long long key2, longID uniqueid) const; //todo move to helper class
  //void sortIDs(IDs& ids); //sorts elements by type
};

PFBlockBuilder::PFBlockBuilder(PFEvent& pfevent) :
  m_pfEvent(pfevent),
  m_historyNodes(pfevent.historyNodes()),
  m_uniqueIDs(pfevent.elementIDs())
{
  

  if (m_historyNodes.size()==0) {
    //create local nodes ready to use to make the blocks
    for (auto id : m_uniqueIDs)
      m_historyNodes.emplace(id, PFNode(id));
  }
  
  //TODO thhink hard about bets way to deal with distance /ruler / edges etc
  //compute edges between each pair of nodes
  Edges edges;
  Ruler ruler(pfevent);
  for (auto id1 : m_uniqueIDs) {
    for (auto id2 : m_uniqueIDs) {
      if (id1 < id2) {
        Distance dist = ruler.distance(id1,id2);
        Edge edge{id1, id2, dist.isLinked(), dist.distance()};
        //the edge object is added into the edges dictionary
        edges.emplace(edge.key(),std::move(edge));
      }
    }
  }
  m_blocks = std::move(BlockBuilder(m_uniqueIDs, edges, m_historyNodes).blocks());

}



