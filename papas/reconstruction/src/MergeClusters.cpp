#include "papas/reconstruction/MergeClusters.h"

#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Event.h"
#include "papas/datatypes/IdCoder.h"
#include "papas/graphtools/BuildSubGraphs.h"
#include "papas/graphtools/Distance.h"
#include "papas/graphtools/Edge.h"
#include "papas/graphtools/EventRuler.h"
#include "papas/graphtools/EventRuler.h"
#include "papas/utility/PDebug.h"

#include <algorithm>

namespace papas {

void mergeClusters(const Event& event, const std::string& typeAndSubtype, const EventRuler& ruler, Clusters& merged,
                   Nodes& history) {
  // extract the clusters collection from the event
  const auto& clusters = event.clusters(typeAndSubtype);
  // make list of all the ids in this collection
  Ids ids;
  for (auto const& cluster : clusters) {
    ids.push_back(cluster.first);
  }
#if WITHSORT
  ids.sort(std::greater<Identifier>());  // sort in descending order
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
  auto subGraphs = buildSubGraphs(ids, edges);
  std::list<const Clusters*> overlappingClusters;
  for (const auto& subgraph : subGraphs) {
    std::list<const Cluster*> overlappingClusters;
    for (const auto& c : subgraph) {
      overlappingClusters.push_back(&clusters.at(c));
    }
    // create the merged Cluster
    Cluster mergedCluster(overlappingClusters, merged.size(), 'm');
    makeHistoryLinks(subgraph, {mergedCluster.id()}, history);
    PDebug::write("Made Merged{}", mergedCluster);
    merged.emplace(mergedCluster.id(), std::move(mergedCluster));  // create a new cluster based on existing cluster
  }
}
}  // end namespace papas