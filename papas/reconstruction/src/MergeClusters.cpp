#include "papas/reconstruction/MergeClusters.h"

#include "papas/datatypes/Event.h"
#include "papas/graphtools/BuildSubGraphs.h"
#include "papas/graphtools/Distance.h"
#include "papas/graphtools/EventRuler.h"
#include "papas/utility/PDebug.h"

#include <algorithm>

namespace papas {

void mergeClusters(const Event& event, const std::string& typeAndSubtype, const EventRuler& ruler, Clusters& merged,
                   Nodes& history) {
  auto ids = event.collectionIds(typeAndSubtype);

  // create unordered map containing all edge combinations, index them by edgeKey
  // the edges describe the distance between pairs of clusters
  Edges edges;
  for (auto id1 : ids) {
    for (auto id2 : ids) {
      if (id1 < id2) {
        Distance dist = ruler.distance(id1, id2);
        //PDebug::write("Distance {} {} {} {}", IdCoder::pretty(id1), //IdCoder::pretty(id2), dist.distance(), dist.isLinked());
        Edge edge{id1, id2, dist.isLinked(), dist.distance()};
        edges.emplace(edge.key(), std::move(edge));
      }
    }
  }
  // create a graph using the ids and the edges this will produces subgroups of ids each of which will form
  // a new merged cluster.
  auto subGraphs = buildSubGraphs(ids, edges);

  /* Note on debate as to safety of storing const Cluster* in overlappingClusters.
    In particular the question "Could the Clusters that are being pointed to move and thus the pointers become
     invalid?"
   (1) According to Stroustrup (4th edition chapter 31, page 886).  “When inserting and erasing elements in a vector,
   elements may be moved. In contrast elements stored in a list or an associative container do not move when new
   elements are inserted or other elements are erased.
   (2) In addition, at the time when we store const Cluster* into the list, the unordered_map of Cluster in which the
   Cluster lives  is already a const object.
    So this should be OK.
  */
  for (const auto& subgraph : subGraphs) {
    std::list<const Cluster*> overlappingClusters;
    for (const auto& cid : subgraph) {
      overlappingClusters.push_back(&(event.cluster(cid)));
    }
    // create the merged Cluster
    Cluster mergedCluster(overlappingClusters, merged.size(), 'm');
    makeHistoryLinks(subgraph, {mergedCluster.id()}, history);
    PDebug::write("Made Merged{}", mergedCluster);
    merged.emplace(mergedCluster.id(), std::move(mergedCluster));  // create a new cluster based on existing cluster
  }
}
}  // end namespace papas
