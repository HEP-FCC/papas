#ifndef RECONSTRUCTION_BLOCKSPLITTER_H
#define RECONSTRUCTION_BLOCKSPLITTER_H

#include "BlockBuilder.h"
#include <iostream>

class PFEvent;



namespace papas {
class PFEvent;
  /**
   *
   * BlockSplitter takes an exisiting block of particle flow element ids (clusters, tracks etc)
   and unlinks any specified edges. If any edges have been unlinked then it then recalculates a new set of sub-blocks
   using the underlying BlockBuilder class
   
   Usage example:
   @code
   splitter = BlockSplitter(block, unlink_edges)
   for b in splitter.blocks.itervalues():
   print b
   @endcode
   Notes: (to be extended)
   if a block is split it will be marked as block.is_active=false and it will "lose" its edges
   as it will effectively have been replaced by the new blocks after the split.
   ** explain how old block fits into history node **
   ...
   }
   *
   *  @author  Alice Robson
   *  @date    2016-04-06
   */
class BlockSplitter {
public:
  /** Constructor
   * @param[in] unlink_edges : list of edges where a link is to be removed
   * @param[inout] block  : Block to be split, will be marked as disactivated
   * @param[inout] historyNodes : unordered_map that describes which elements are parents of which blocks
   *                     if a history_nodes tree is non empty then
   *                     the new history will be added into the existing history
   */
  BlockSplitter(const Edges& unlinkEdges, PFBlock& block, Nodes& historynodes);
  Blocks& blocks() { return m_blocks; };  
  friend std::ostream& operator<<(std::ostream& os, const BlockSplitter& builder);  // todo move to helper class?

private:
  Blocks m_blocks; ///< unordered map of the newly calculated blocks;
};
}  // end namespace papas
#endif /* BlockSplitter_h */
