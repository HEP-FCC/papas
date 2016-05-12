//
//  PFBlockBuilder.hpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//

#ifndef PFBlockBuilder_h
#define PFBlockBuilder_h

#include <iostream>
#include "NodeDefinitions.h"

class PFEvent;

class PFBlockBuilder {
public:
  /** Constructor

   * @param[in] ids : vector of unique identifiers eg of tracks, clusters etc
   * @param[in] edges : unordered_map of edges which contains all edges between the ids (and maybe more)
   *            an edge records the distance and links between two ids
   * @param[inout] historyNodes : optional unordered_map that describes which elements are parents of which blocks
   *                     if a history_nodes tree is provided then
   *                     the new history will be added into the exisiting history
   */
  PFBlockBuilder(PFEvent& pfevent);
  // const Ids elementIds() const { return m_elementIds;};///< return the blockbuilders element ids
  Blocks& blocks() { return m_blocks; };  ///<return the unordered map of the resulting blocks;
  friend std::ostream& operator<<(std::ostream& os, const PFBlockBuilder& blockbuilder);  // TODO move to helper class

private:
  void makeBlocks();  // does the main work
  Blocks m_blocks;    ///< the blocks made by blockbuilder
  PFEvent& m_pfEvent;
  Nodes& m_historyNodes;  ///<optional, allows history to be updated
  Ids m_uniqueIds;
  // void sortIds(Ids& ids); //sorts elements by type
};

#endif /* PFBlockBuilder_hpp */