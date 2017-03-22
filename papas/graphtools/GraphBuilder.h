#ifndef RECONSTRUCTION_GRAPHBUILDER_H
#define RECONSTRUCTION_GRAPHBUILDER_H

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"

namespace papas {

/**
 *
 * GraphBuilder takes a list of identifiers and an unordered map of associated edges which have distance and link
 * (ie true/false) info.
 * The graph can be thought of as having the ids as the nodes and the edges as the connecting lines.
 * It uses the distances/links between elements to construct a set of connected blocks.
 * Each element will end up in one (and only one) block.
 * Blocks retain information of the elements and edges including the distances between elements.
 * The blocks can be used for future particle reconstruction.
 * The ids of all elements must be unique and are expected to come from the IdCoder class.


 Usage example:
@code
 GraphBuilder builder(ids, edges);
 for (b in builder.blocks()) {
 ...
 }
 @endcode
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */

class GraphBuilder {
public:
  /** Constructor

   * @param[in] ids : vector of identifiers eg of tracks, clusters etc
   * @param[in] edges : unordered_map of edges which contains all edges between the ids (and maybe more),
   *            an edge records the distance and links between two ids. The edges will be moved into the
   *            resulting blocks and become owned by them.
   */
  GraphBuilder(const Ids& ids, Edges&& edges);  /// edges are moved in and will end up owned by generated blocks

  /// returns a vector containing vectors of Ids each of which is a subgraph
  const std::vector<Ids>& subGraphs() const { return m_subGraphs; };

protected:
  void sortIds(Ids& ids);        ///<sorted by Identifier
  Edges m_edges;                 ///< must contain all edges corresponding to all pairs of ids for ids in m_elementIda
  std::vector<Ids> m_subGraphs;  ///< vector of subgraphs made by graphbuilder
private:
  Ids m_elementIds;    ///<identifiers to be grouped into subgraphs
  Nodes m_localNodes;  ///<local nodes used in building subgraphs
};
}  // end namespace papas
#endif /* GraphBuilder_h */
