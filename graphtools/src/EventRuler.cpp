//
//  EventRuler.cpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//

#include "EventRuler.h"
#include "Cluster.h"
#include "Distance.h"
#include "PFEvent.h"
#include "pTrack.h"

namespace papas {

EventRuler::EventRuler(const PFEvent& pfevent) : m_ruler(), m_pfEvent(pfevent) {}

Distance EventRuler::distance(IdType id1, IdType id2) const {
  if (Id::isCluster(id1) && Id::isCluster(id2))
    if (Id::itemType(id1) == Id::itemType(id2))
      return clusterClusterDistance(id1, id2);
    else  // hcal ecal not linked
      return Distance();
  else if (Id::isTrack(id2) && Id::isCluster(id1))
    return clusterTrackDistance(id1, id2);
  else if (Id::isTrack(id1) && Id::isCluster(id2))
    return clusterTrackDistance(id2, id1);
  else if (Id::isTrack(id1) && Id::isTrack(id2))
    return Distance();
  std::cout<<Id::itemType(id1) << ":"<< Id::itemType(id2) << std::endl;
  throw "Distance between ids could not be computed";
  return Distance();
}

Distance EventRuler::clusterClusterDistance(IdType id1, IdType id2) const {
  const Cluster& cluster1 = m_pfEvent.cluster(id1);
  const Cluster& cluster2 = m_pfEvent.cluster(id2);
  return m_ruler.clusterClusterDistance(cluster1, cluster2);
}

Distance EventRuler::clusterTrackDistance(IdType clustId, IdType trackId) const {
  const Cluster& cluster = m_pfEvent.cluster(clustId);
  const Track& track = m_pfEvent.track(trackId);
  return m_ruler.clusterTrackDistance(cluster, track);
}

}  // end namespace papas