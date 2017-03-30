#include "papas/reconstruction/MergeClusters.h"

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/IdCoder.h"
#include "papas/graphtools/Distance.h"
#include "papas/graphtools/Edge.h"
#include "papas/graphtools/EventRuler.h"
#include "papas/graphtools/GraphBuilder.h"
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
  GraphBuilder grBuilder(ids, std::move(edges));
  for (const auto& subgraph : grBuilder.subGraphs()) {
    const auto& id = *subgraph.begin();
    double totalenergy = 0.;
    for (auto c : subgraph) {
      totalenergy += clusters.at(c).energy();
    }
    // create the merged Cluster
    // Note we could try to do this in one shot as in the latest Python version... but its a little complicated
    // for several reasons so this is probably more straightforward

    Cluster mergedCluster(clusters.at(id), merged.size(), IdCoder::type(id), 'm',
                          totalenergy);  // create a new cluster based on old one
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
    makeHistoryLinks(subgraph, {mergedCluster.id()}, history);
    PDebug::write("Made Merged{}", mergedCluster);
    merged.emplace(mergedCluster.id(), std::move(mergedCluster));  // create a new cluster based on existing cluster
  }
}
}  // end namespace papas
