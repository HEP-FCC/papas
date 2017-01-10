//
//  PFBlcokSplitter.h
//  papas
//
//  Created by Alice Robson on 10/01/17.
//
//

#ifndef PFBlockSplitter_h
#define PFBlockSplitter_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/reconstruction/PFBlock.h"
#include <iostream>

namespace papas {
  class PapasEvent;
  
  /// The PFBlockBuilder takes a PFEvent and a set of ids (all of which must be in the PFEvent)
  /// It uses the PFEvent to access the elements corresponding to each id
  /// It proceeds by creating a collection of all possible edges (between ids)
  /// The blockbuilder is then used to create a set of distinct blocks, and the
  /// blocks look after the corresponding parts of the edges information.
  /// Class to create blocks of connected elements from a PF event eg connected tracs and clusters.
  /// It creates what is needed in order to call BlockBuilder
  class PFBlockSplitter {
  public:
    /** Constructor
     * @param[in]  pfevent contains collections of tracks, clusters and historyNodes
     * @param[in] ids  list of element ids from which to construct a block
     */
    //PFBlockBuilder(const Ids& ids, PFEvent& pfevent);  // history inside PFEvent will be updated (improve this)
    PFBlockSplitter(const PapasEvent& m_papasEvent, char blockSubtype,Blocks& simplifiedblocks, Nodes& history);
    Blocks& blocks() { return m_blocks; };   ///<return the unordered map of the resulting blocks;
    void simplifyBlock(const PFBlock& block);
  private:
    void makeBlocks();         ///< does the main work of creating the blocks
    const PapasEvent& m_papasEvent;  ///< contains the collections of tracks, clusters and historyNodes
    Blocks& m_blocks;           ///< the blocks made by blockbuilder
    Nodes& m_history;     ///< history will be updated to store block creation
    Ids m_uniqueIds;           ///< list of element ids from which to construct the blocks
  };
}  // end namespace papas
#endif /* PFBlcokSplitter_h */
