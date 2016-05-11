#ifndef RECONSTRUCTION_BLOCKSPLITTER_H
#define RECONSTRUCTION_BLOCKSPLITTER_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "BlockBuilder.h"
#include "directedacyclicgraph.h"

class PFEvent;
class PFBlock;
class Edge;

/** @class   rec::BlockBuilder Reconstruction/Reconstruction/BlockBuilder.h BlockBuilder.h
 *
 * BlockBuilder takes a vector of identifiers and an unordered map of associated edges which have distance and link info
 * It uses the distances/links between elements to construct a set of connected blocks
 * Each element will end up in one (and only one) block
 * Blocks retain information of the elements and the distances between elements
 * The blocks can be used for future particle reconstruction
 * The ids must be unique and are expected to come from the Id class
 ''' BlockSplitter takes an exisiting block of particle flow element ids (clusters,tracks etc)
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


 Usage example:

 BlockBuilder builder {ids, edges, history_nodes, pfevent};
 for (b in builder.blocks()) {
 ...
 }
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */

// TODO namespace
class PFEvent;

// TODO figure out best place to keep the typedefs
typedef long longId;
typedef std::unordered_map<long long, class Edge> Edges;
typedef DAG::Node<longId> PFNode;
typedef std::unordered_map<longId, PFNode> Nodes;
typedef std::unordered_map<longId, PFBlock> Blocks;
typedef std::vector<longId> Ids;

// Allow optional parameters where construction arguments are references
extern Nodes emptyNodes;
extern const Nodes emptyconstNodes;

class BlockSplitter {
public:
  /** Constructor
   * @param[in] unlink_edges : list of edges where a link is to be removed
   * @param[inout] block  : Block to be split, will me marked as disactivated
   * @param[inout] historyNodes : optional unordered_map that describes which elements are parents of which blocks
   *                     if a history_nodes tree is provided then
   *                     the new history will be added into the exisiting history
   */
  BlockSplitter(const Edges& unlinkEdges, PFBlock& block, Nodes& historynodes = emptyNodes);
  Blocks blocks() { return m_blocks; };  ///<return the unordered map of the resulting blocks;
  friend std::ostream& operator<<(std::ostream& os, const BlockSplitter& builder);  // todo move to helper class

private:
  Blocks m_blocks;
};

#endif /* BlockSplitter_h */