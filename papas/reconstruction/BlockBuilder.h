#ifndef RECONSTRUCTION_BLOCKBUILDER_H
#define RECONSTRUCTION_BLOCKBUILDER_H

#include "papas/graphtools/GraphBuilder.h"
#include "papas/reconstruction/PFBlock.h"

namespace papas {
class PFEvent;
/**
 * BlockBuilder takes a list of Ids and an unordered map of associated edges which have distance and link info
 * It uses the distances/links between elements to construct a set of connected blocks.
 * Each element will end up in one (and only one) block.
 * Blocks retain information of the elements and the distances between elements.
 * The blocks can be used for future particle reconstruction.
 * The ids must be unique and are expected to come from the Id class.


 Usage example:
 @code
 BlockBuilder builder {ids, edges, history_nodes, pfevent};
 for (b in builder.blocks()) {
 ...
 }
 @endcode
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */
class BlockBuilder : public GraphBuilder {
public:
  /** Constructor
   * @param[in] ids : list of unique identifiers eg of tracks, clusters etc
   * @param[in] edges : unordered_map of edges which contains all edges between the ids (and maybe more)
   *            an edge records the distance and links between two ids
   * @param[inout] historyNodes : optional (ie can be empty) unordered_map that describes which elements are parents of
   * which blocks
   *                     if a history_nodes tree is provided then
   *                     the new history will be added into the existing history
   */
  BlockBuilder(const Ids& ids, Edges&& edges, Nodes& historynodes, char subtype);  // edges are moved, history can be updated
  /// Move the unordered map of the resulting blocks - note that this can only be done once and will empty m_blocks
  Blocks moveBlocks() { return std::move(m_blocks); };
  friend std::ostream& operator<<(std::ostream& os, const BlockBuilder& blockbuilder);  // TODO move to helper class?

private:
  void makeBlocks(char subtype);      // does the main work
  Nodes& m_historyNodes;  ///< history can be updated
  Blocks m_blocks;        ///< the blocks made by blockbuilder
};

}  // end namespace papas
#endif /* BlockBuilder_h */
