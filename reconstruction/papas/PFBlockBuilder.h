//
//  PFBlockBuilder.hpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//

#ifndef PFBlockBuilder_h
#define PFBlockBuilder_h

#include "DefinitionsCollections.h"
#include "DefinitionsNodes.h"
#include "PFBlock.h"
#include <iostream>

namespace papas {
class PFEvent;

class PFBlockBuilder {
public:
  /** Constructor
   * @param[in] PFEvent& pfevent : contains collections of tracks, clusters and historyNodes
   * @param[in] Ids& ids : list of element ids from which to construct a block
   *  The constuctor creates the blocks from the ids
   */
  PFBlockBuilder(PFEvent& pfevent, Ids& ids);
  Blocks& blocks() { return m_blocks; };  ///<return the unordered map of the resulting blocks;

private:
  void makeBlocks();      ///< does the main work of creating the blocks
  Blocks m_blocks;        ///< the blocks made by blockbuilder
  PFEvent& m_pfEvent;     ///< contains the collections of tracks, clusters and historyNodes
  Nodes& m_historyNodes;  ///< optional, allows history to be updated
  Ids m_uniqueIds;        ///< list of element ids from which to construct the blocks
  // void sortIds(Ids& ids); //TODO ? sorts elements by type
};
}  // end namespace papas
#endif /* PFBlockBuilder_hpp */
