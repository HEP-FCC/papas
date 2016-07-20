//
//  Ruler.cpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//

#include "Cluster.h"
#include "Distance.h"
#include "Ruler.h"
#include "pTrack.h"

namespace papas {

Distance Ruler::clusterClusterDistance(const Cluster& cluster1, const Cluster& cluster2) const {

  if (cluster1.subClusters().size() <= 1 && cluster2.subClusters().size() <= 1) {
    return Distance(cluster1, cluster2);
  } else {  // merged cluster
    std::vector<double> allDistances;
    std::vector<double> linkedDistances;
    bool isLinked = false;

    for (const auto c1 : cluster1.subClusters()) {
      for (const auto c2 : cluster2.subClusters()) {
        Distance d = clusterClusterDistance(*c1, *c2);
        allDistances.push_back(d.distance());
        if (d.isLinked()) {
          linkedDistances.push_back(d.distance());
          isLinked = true;
        }
      }
    }
    double mindist = -1;
    if (isLinked)
      mindist = *std::min_element(std::begin(linkedDistances), std::end(linkedDistances));
    else
      mindist = *std::min_element(std::begin(allDistances), std::end(allDistances));
    return Distance{isLinked, mindist};
  }
}

Distance Ruler::clusterTrackDistance(const Cluster& cluster, const Track& track) const {
  if (cluster.subClusters().size() > 1) {
    std::vector<double> distances;
    std::vector<double> linkedDistances;
    bool isLinked = false;
    for (const auto c : cluster.subClusters()) {
      Distance d = clusterTrackDistance(*c, track);
      distances.push_back(d.distance());
      if (d.isLinked()) {
        linkedDistances.push_back(d.distance());
        isLinked = true;
      }
    }
    double mindist = -1;
    if (isLinked)
      mindist = *std::min_element(std::begin(linkedDistances), std::end(linkedDistances));
    else
      mindist = *std::min_element(std::begin(distances), std::end(distances));
    return Distance{isLinked, mindist};
  } else
    return Distance{cluster, track};
}

}  // end namespace papas