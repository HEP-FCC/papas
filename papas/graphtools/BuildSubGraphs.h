#ifndef RECONSTRUCTION_BUILDSUBGRAPHS_H
#define RECONSTRUCTION_BUILDSUBGRAPHS_H

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"

namespace papas {

/**
 * buildSubGraphs is a function taking a list of identifiers and an unordered map of associated edges containing
 * distance and link (ie true/false) info that describe a graph.
 * The graph can be thought of as having the ids as the nodes and the edges as the connecting lines.
 * buildSubGraphs uses the distances/links between elements to construct a set of connected subgraphs.
 * Each id will end up in one (and only one) subgraph
 
 Usage example:
@code
 std:vector<const Ids> subGraphs = buildSubGraphs(ids, edges);
 for (const auto& s : subGraphs) {
 ...
 }
 @endcode
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */

  /** buildSubGraphs function
   * @param[in] ids : vector of identifiers eg of tracks, clusters etc
   * @param[in] edges : unordered_map of edges which contains all edges between the ids (and maybe more),
   *            an edge records the distance and links between two ids. The edges will be moved into the
   *            resulting blocks and become owned by them.
   */
  std::vector<const Ids> buildSubGraphs(const Ids& ids, const Edges& edges);

  }  // end namespace papas
#endif /* BuildSubGraphs_h */
