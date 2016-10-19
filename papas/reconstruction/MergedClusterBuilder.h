//
//  MergedClusterBuilder.h
//  papas
//
//  Created by Alice Robson on 07/06/16.
//
//

#ifndef MergedClusterBuilder_h
#define MergedClusterBuilder_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/graphtools/EventRuler.h"
#include <iostream>

namespace papas {
/// Takes a collection of clusters and produces a collection of merged clusters.
/// The MergedClusterBuilder creates a collection of edges describing the distances between
/// every possible pair of clusters.
/// The cluster ids and corresponding edges are then passed to GraphBuilder which
/// returns a set of blocks, each block is a set of overlapping clusters and if it
/// contains more than one element forms a new merged cluster.
/// The mergedcluster is created by making a new cluster and adding in each of the overlapping clusters
/// The ids of these clusters are stored in the subClusters vector of the Cluster class.
///
class MergedClusterBuilder {
public:
  /** Constructor
   TODO update docs
   * @param[in] clusters : collections of clusters
   * @param[in] ruler : measures distance between clusters
   * @param[inout] historyNodes : unordered_map that describes which elements are parents of which blocks
   *                     if  historyNodes is provided it must contain nodes corresponding to each cluster
   *                     if it is empty it will be ignored
   */
  MergedClusterBuilder(const Clusters& clusters, const EventRuler& ruler,
                       Nodes& historyNodes);                 // history will be updated
  Clusters mergedClusters() { return std::move(m_merged); }  // empties m_merged - is this best way?
  Nodes& historyNodes() { return m_historyNodes; }           // history nodes can be updated

private:
  Clusters m_merged;      ///< the merged Clusters
  Nodes& m_historyNodes;  ///<optional, allows history to be updated
};
}  // end namespace papas

#endif /* MergedClusterBuilder_h */
