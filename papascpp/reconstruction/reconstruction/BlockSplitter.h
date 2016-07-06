#ifndef RECONSTRUCTION_BLOCKSPLITTER_H
#define RECONSTRUCTION_BLOCKSPLITTER_H

#include "BlockBuilder.h"
#include <iostream>

class PFEvent;

/** @class   rec::BlockSplitter Reconstruction/Reconstruction/Blocksplitter.h BlockSplitter.h
 *
 * BlockSplitter takes an exisiting block of particle flow element ids (clusters,tracks etc)
 and unlinks any specified edges. It then recalculates a new set of sub-blocks
 using the underlying BlockBuilder class

 Usage example:

 splitter = BlockSplitter(block, unlink_edges)
 for b in splitter.blocks.itervalues():
 print b

 Notes: (to be extended)
 if a block is split it will be marked as block.is_active=false
 as it will effectively be replaced by the new blocks after the split.
 ** explain how old block fits into history node **
 '''
...
 }
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */

namespace papas {
class PFEvent;

class BlockSplitter {
public:
  /** Constructor
   * @param[in] unlink_edges : list of edges where a link is to be removed
   * @param[inout] block  : Block to be split, will me marked as disactivated
   * @param[inout] historyNodes : optional unordered_map that describes which elements are parents of which blocks
   *                     if a history_nodes tree is provided then
   *                     the new history will be added into the exisiting history
   */
  BlockSplitter(const Edges& unlinkEdges, PFBlock& block, Nodes& historynodes);
  Blocks& blocks() { return m_blocks; };  ///<return the unordered map of the resulting blocks;
  friend std::ostream& operator<<(std::ostream& os, const BlockSplitter& builder);  // todo move to helper class?

private:
  Blocks m_blocks;
};
}  // end namespace papas
#endif /* BlockSplitter_h */
