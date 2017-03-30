
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/datatypes/Event.h"
#include "papas/graphtools/EventRuler.h"
#include <string>
namespace papas {
  
  class Event;
  class EventRuler;

  /// Function to take a collection of clusters and write to a collection of merged clusters.
  /// The MergedClusters function creates a collection of edges describing the distances between
  /// every possible pair of clusters.
  /// The cluster ids and corresponding edges are then passed to GraphBuilder which
  /// returns a set of block. Each block is a set of overlapping clusters and becomes a new merged cluster.
  /// Blocks with only one cluster will also create a new merged cluster (a copy of the original cluster)
  /// The mergedcluster is created by making a new cluster and adding in each of the overlapping clusters
  /// The ids of the component clusters are stored in the subClusters vector of the Cluster class.
  ///

    /**mergeClusters Function
     * @param[in] Event must contain the Clusters collection that is to be merged
     * @param[in] typeAndSubtype the tyepAndSubType of the clusters collection that is to be merged (as a string)
     * @param[in] ruler measures distance between clusters
     * @param[in] merged an empty unordered_map into which the merged Clusters will be place
     * @param[inout] history an unordered_map into which new history will be added
     */
    void mergeClusters(const Event& event, const std::string& typeAndSubtype, const EventRuler& ruler,
                         Clusters& merged, Nodes& history);
}//end namespace papas
