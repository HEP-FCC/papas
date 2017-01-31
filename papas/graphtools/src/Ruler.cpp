//
//  Ruler.cpp

#include "papas/graphtools/Ruler.h"
#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Track.h"
#include "papas/graphtools/Distance.h"

namespace papas {

Distance Ruler::clusterClusterDistance(const Cluster& cluster1, const Cluster& cluster2) const {
  // Decides whether these are merged clusters or a simple cluster.
  if (cluster1.subClusters().size() <= 1 && cluster2.subClusters().size() <= 1) {
    return Distance(cluster1, cluster2);  /// If both are simple clusters then returns the distance between the two
  } else {
    // Otherwise deal with merged cluster(s).
    // Examine all cluster cluster distances within the subclusters
    // and look for the closest overlap between the mergedclusters, returning the minimum distance found.
    std::list<double> allDistances;
    std::list<double> linkedDistances;
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
  if (cluster.subClusters().size() > 1) { //its a merged cluster
    // distance is the minimum distance between the track and each of the subclusters
    std::list<double> distances;
    std::list<double> linkedDistances;
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
    return Distance{isLinked, mindist}; //move
  } else //its a non merged cluster
    return Distance{cluster, track};  //move
}

}  // end namespace papas
