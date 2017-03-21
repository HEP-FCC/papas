#ifndef RECONSTRUCTION_BLOCKBUILDER_H
#define RECONSTRUCTION_BLOCKBUILDER_H

#include "papas/graphtools/GraphBuilder.h"
#include "papas/reconstruction/PFBlock.h"

namespace papas {

/**
 * BlockBuilder takes a list of Ids and an unordered map of associated edges which have distance and link info
 * It uses the distances/links between elements to construct a set of connected blocks.
 * Each element will end up in one (and only one) block.
 * Blocks retain information of the elements and the distances between elements.
 * The blocks can be used for future particle reconstruction.
 * The ids must be unique and are expected to come from the IdCoder class.

 Usage example:
 @code
 auto builder = BlockBuilder (ids, edges, history, blocks, 'r');

 }
 @endcode

 */
class BlockBuilder : public GraphBuilder {
public:
  /** Constructor
   * @param[in] ids : list of identifiers eg of tracks, clusters etc
   * @param[in] edges : unordered_map of edges which contains all edges between the ids (and maybe more)
   *            an edge records the distance and links between two ids
   * @param[inout] history : external collection of Nodes to which parent child relations can be added
   * @param[inout] blocks: external collection into which new blocks will be added
   * @param[in] subtype: the subtype (eg 'r') which will be given to newly created blocks
   */
  BlockBuilder(const Ids& ids, Edges&& edges, Nodes& history, Blocks& blocks, char subtype);
  friend std::ostream& operator<<(std::ostream& os, const BlockBuilder& blockbuilder);  // TODO move to helper class?

private:
  void makeBlocks(char blockSubtype);  // does the main work
  Nodes& m_history;               ///< externally owned and will be updated by BlockBuilder
  Blocks& m_blocks;               ///< externally owned and will be updated by BlockBuilder
};

}  // end namespace papas
#endif /* BlockBuilder_h */
