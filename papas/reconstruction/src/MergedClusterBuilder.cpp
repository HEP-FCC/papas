#include "papas/reconstruction/MergedClusterBuilder.h"
#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/IdCoder.h"
#include "papas/graphtools/Distance.h"
#include "papas/graphtools/Edge.h"
#include "papas/graphtools/GraphBuilder.h"
#include "papas/graphtools/EventRuler.h"
#include "papas/utility/PDebug.h"

#include <algorithm>

namespace papas {

MergedClusterBuilder::MergedClusterBuilder(const Event& event,
                                           const std::string& typeAndSubtype,
                                           const EventRuler& ruler,
                                           Clusters& merged,
                                           Nodes& history)
    : m_merged(merged), m_history(history) {
  // extract the clusters collection from the event
  const auto& clusters = event.clusters(typeAndSubtype);
  // make list of all the ids in this collection
  Ids ids;
  for (auto const& cluster : clusters) {
    ids.push_back(cluster.first);
  }
#if WITHSORT
  ids.sort(std::greater<IdType>()); //sort in descending order
#endif
  // create unordered map containing all edge combinations, index them by edgeKey
  // the edges describe the distance between pairs of clusters
  Edges edges;
  for (auto id1 : ids) {
    for (auto id2 : ids) {
      if (id1 < id2) {
        Distance dist = ruler.distance(id1, id2);
        Edge edge{id1, id2, dist.isLinked(), dist.distance()};
        edges.emplace(edge.key(), std::move(edge));
      }
    }
  }
  // create a graph using the ids and the edges this will produces subgroups of ids each of which will form
  // a new merged cluster.
   auto grBuilder = GraphBuilder(ids, std::move(edges));
  for (auto subgraph : grBuilder.subGraphs()) {
#if WITHSORT
    subgraph.sort(std::greater<IdType>()); //sort in descending order
#endif
    auto id = *subgraph.begin();
    double totalenergy = 0.;
    for (const auto& c : subgraph) {
      totalenergy += clusters.at(c).energy();
    }
    // create the merged Cluster
    // Note we could try to do this in one shot as in the latest Python version... but its a little complicated
    //for several reasons so this is probably more straightforward
    auto mergedCluster =
        Cluster(clusters.at(id), merged.size(), IdCoder::itemType(id), 'm', totalenergy);  // create a new cluster based on old one
    if (id == mergedCluster.id()) {
      throw "MergedCluster has same id as existing cluster";
    }
    // merge the original clusters into the new merged cluster
    // also add in the links between the block elements and the block into the history_nodes
    if (subgraph.size() > 1) {
      for (auto elemid : subgraph) {
        if (elemid != id) {  // we have already handled the first element
          mergedCluster += clusters.at(elemid);
        }
      }
    }
    makeHistoryLinks(subgraph, {mergedCluster.id()}, m_history);
    PDebug::write("Made Merged{}", mergedCluster);
    m_merged.emplace(mergedCluster.id(), std::move(mergedCluster));  // create a new cluster based on existing cluster
  }
}
}  // end namespace papas
