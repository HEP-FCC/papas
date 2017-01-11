#ifndef MergedClusterBuilder_h
#define MergedClusterBuilder_h

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/graphtools/Ruler.h"
#include "papas/datatypes/PapasEvent.h"
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
     * @param[in] PapasEvent : much contain the Clusters collection that is to be merged
     * @param[in] typeAndSubtype : the tyepAndSubType of the clusters collection that is to be merged (as a string)
     * @param[in] ruler : measures distance between clusters
     * @param[in] merged : an empty unordered_amp into which the merged Clusters will be place
     * @param[inout] history : an unordered_map into which new history will be added
     */
    MergedClusterBuilder(const PapasEvent& papasEvent, const std::string& typeAndSubtype, const Ruler& ruler, Clusters& merged, Nodes& history);
    /** Access mergedCluster collection
        @return reference to the merged Clusters collection
     */
    const Clusters& mergedClusters() const { return m_merged; }  //remove this?
    /** Access history nodes
        @return reference to the history
     */
    const Nodes& history() const { return m_history; }    //remove this?
    
  private:
    Clusters& m_merged;      ///< the merged Clusters //remove this?
    Nodes& m_history;  ///history is appended to this //remove this?
  };
}  // end namespace papas

#endif /* MergedClusterBuilder_h */
