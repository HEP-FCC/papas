//
//  MergedClusterBuilder.h
//  papas
//
//  Created by Alice Robson on 07/06/16.
//
//

#ifndef MergedClusterBuilder_h
#define MergedClusterBuilder_h

#include "DefinitionsCollections.h"
#include "DefinitionsNodes.h"
#include "Ruler.h"
#include <iostream>

namespace papas {

class MergedClusterBuilder {
public:
  /** Constructor
   TODO update docs
   * @param[in] ids : vector of unique identifiers eg of tracks, clusters etc
   * @param[in] edges : unordered_map of edges which contains all edges between the ids (and maybe more)
   *            an edge records the distance and links between two ids
   * @param[inout] historyNodes : optional unordered_map that describes which elements are parents of which blocks
   *                     if a history_nodes tree is provided then
   *                     the new history will be added into the exisiting history
   */
  MergedClusterBuilder(const Clusters& clusters, EventRuler& ruler, Nodes& historyNodes);
  // const Ids elementIds() const { return m_elementIds;};///< return the blockbuilders element ids
  Clusters& mergedClusters() { return m_merged; }
  Nodes& historyNodes() { return m_historyNodes; }

private:
  void makeMergedClusters();  // does the main work
  Clusters m_merged;          ///< the merged Clusters
  Nodes& m_historyNodes;      ///<optional, allows history to be updated
};
}  // end namespace papas

#endif /* MergedClusterBuilder_h */
