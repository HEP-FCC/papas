//
//  Ruler.cpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//

#include "Ruler.h"
#include "Cluster.h"
#include "Distance.h"
#include "pTrack.h"

namespace papas {

Distance Ruler::clusterClusterDistance(const Cluster& cluster1, const Cluster& cluster2) const {
  /// Decides whether these are merged clusters or a simple cluster.
  if (cluster1.subClusters().size() <= 1 && cluster2.subClusters().size() <= 1) {
    return Distance(cluster1, cluster2);  /// If both are simple clusters then returns the distance between the two
  } else {
    /// Otherwise deal with merged cluster(s).
    /// Examine all cluster cluster distances within the subclusters
    /// and look for the closest overlap between the mergedclusters, returning the minimum distance found.
    std::vector<double> allDistances;
    std::vector<double> linkedDistances;
    bool isLinked = false;

    for (const auto c1 : cluster1.subClusters()) {
      for (const auto c2 : cluster2.subClusters()) {
        Distance d = clusterClusterDistance(*c1, *c2);  // recursive call
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
    return Distance{isLinked, mindist};  // will be moved
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
    return Distance{cluster, track};  // will be moved
}

}  // end namespace papas