//
//  Ruler.cpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//

#include "Ruler.h"
//#include <unordered_map>
#include "Distance.h"
#include "PFEvent.h"
#include "PFBlock.h"
#include "Cluster.h"


Ruler::Ruler(const PFEvent& pfevent) :
m_pfEvent(pfevent)
{
}

Distance Ruler::distance(Id::Type id1, Id::Type id2)
{
  if (Id::isCluster(id1) && Id::isCluster(id2))
    if (Id::layer(id1) == Id::layer(id2))
      return clusterClusterDistance(id1, id2);
    else //hcal ecal not linked
      return Distance();
    else if (Id::isTrack(id2) && Id::isCluster(id1))
      return clusterTrackDistance(id1, id2);
    else if (Id::isTrack(id1) && Id::isCluster(id2))
      return clusterTrackDistance(id2, id1);
    else if (Id::isTrack(id1) && Id::isTrack(id2))
      return Distance();
  //TODO error
  return Distance();
}


Distance Ruler::clusterClusterDistance(Id::Type id1 , Id::Type id2)
{
  const Cluster& cluster1 = m_pfEvent.cluster(id1);
  const Cluster& cluster2 = m_pfEvent.cluster(id2);
  if (cluster1.subClusters().size() <= 1 && cluster2.subClusters().size() <= 1) {
    return Distance{cluster1, cluster2} ;
  }
  else  { //merged cluster
    std::vector<double> allDistances;
    std::vector<double> linkedDistances;
    bool isLinked = false;
    for (auto c1 : cluster1.subClusters()) {
      for (auto c2 : cluster2.subClusters()) { //TODO make subcluster return self if no subclusters and tidy the idea up
        Distance d = clusterClusterDistance(c1, c2);
        allDistances.push_back(d.distance());
        if (d.isLinked()) {
          linkedDistances.push_back(d.distance());
          isLinked = true;
        }
      }
    }
    double mindist = -1;
    if (isLinked)
      mindist =* std::min_element(std::begin(linkedDistances), std::end(linkedDistances));
    else
      mindist =* std::min_element(std::begin(allDistances), std::end(allDistances));
    return Distance{isLinked, mindist};
  }
}


Distance Ruler::clusterTrackDistance(Id::Type clustId , Id::Type trackId)
{
  const Cluster& cluster = m_pfEvent.cluster(clustId);
  const Track& track = m_pfEvent.track(trackId);
  
  if (cluster.subClusters().size() > 1) {
    std::vector<double> distances;
    std::vector<double> linkedDistances;
    bool isLinked = false;
    for (auto id : cluster.subClusters()) {
      Distance d{m_pfEvent.cluster(id), track};
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


Distance Ruler::distance()
{
  //could bypass this as its a null distance
  return Distance{};
}
