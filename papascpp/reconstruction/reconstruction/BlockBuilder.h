#ifndef RECONSTRUCTION_BLOCKBUILDER_H
#define RECONSTRUCTION_BLOCKBUILDER_H

#include "GraphBuilder.h"
#include "PFBlock.h"

/** @class   rec::BlockBuilder Reconstruction/Reconstruction/BlockBuilder.h BlockBuilder.h
 *
 * BlockBuilder takes a vector of Ids and an unordered map of associated edges which have distance and link info
 * It uses the distances/links between elements to construct a set of connected blocks
 * Each element will end up in one (and only one) block
 * Blocks retain information of the elements and the distances between elements
 * The blocks can be used for future particle reconstruction
 * The ids must be unique and are expected to come from the Id class


 Usage example:

 BlockBuilder builder {ids, edges, history_nodes, pfevent};
 for (b in builder.blocks()) {
 ...
 }
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */

namespace papas {
class PFEvent;

class BlockBuilder : public GraphBuilder {
public:
  /** Constructor

   * @param[in] ids : vector of unique identifiers eg of tracks, clusters etc
   * @param[in] edges : unordered_map of edges which contains all edges between the ids (and maybe more)
   *            an edge records the distance and links between two ids
   * @param[inout] historyNodes : optional unordered_map that describes which elements are parents of which blocks
   *                     if a history_nodes tree is provided then
   *                     the new history will be added into the exisiting history
   */
  BlockBuilder(Ids ids, Edges& edges, Nodes& historynodes);
  Blocks& blocks() { return m_blocks; };  ///<return the unordered map of the resulting blocks;
                                          // TODO should this be move
  friend std::ostream& operator<<(std::ostream& os, const BlockBuilder& blockbuilder);  // TODO move to helper class?

private:
  void makeBlocks();      // does the main work
  Nodes& m_historyNodes;  ///<optional, allows history to be updated
  Blocks m_blocks;        ///< the blocks made by blockbuilder

  // bool compareEdges( long long key1, long long key2, Id::Type uniqueid) const; //todo move to helper class
  // void sortIds(Ids& ids); //sorts elements by type
};

}  // end namespace papas
#endif /* BlockBuilder_h */
