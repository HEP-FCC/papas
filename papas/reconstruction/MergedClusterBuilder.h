#ifndef MergedClusterBuilder_h
#define MergedClusterBuilder_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/graphtools/Ruler.h"
#include "papas/datatypes/PapasEvent.h"
#include <iostream>


namespace papas {
  /// Takes a collection of clusters and writes a collection of merged clusters.
  /// The MergedClusterBuilder creates a collection of edges describing the distances between
  /// every possible pair of clusters.
  /// The cluster ids and corresponding edges are then passed to GraphBuilder which
  /// returns a set of blocks, each block is a set of overlapping clusters and becomes a new merged cluster.
  /// Blocks with only one cluster will also create a new merged cluster (a copy of the original cluster)
  /// The mergedcluster is created by making a new cluster and adding in each of the overlapping clusters
  /// The ids of the component clusters are stored in the subClusters vector of the Cluster class.
  ///
  class MergedClusterBuilder {
  public:
    /** Constructor
     * @param[in] PapasEvent : much contain the Clusters collection that is to be merged
     * @param[in] typeAndSubtype : the tyepAndSubType of the clusters collection that is to be merged (as a string)
     * @param[in] ruler : measures distance between clusters
     * @param[in] merged : an empty unordered_amp into which the merged Clusters will be place
     * @param[inout] history : an unordered_map into which new history will be added
     */
    MergedClusterBuilder(const PapasEvent& papasEvent, const std::string& typeAndSubtype, const Ruler& ruler, Clusters& merged, Nodes& history);
      private:
    Clusters& m_merged;      ///< external collection to which the merged Clusters will be added
    Nodes& m_history;  ///external collection to which new history info will be added
  };
}  // end namespace papas

#endif /* MergedClusterBuilder_h */
