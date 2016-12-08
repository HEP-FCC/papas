//
//  EventRuler.cpp
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//

#include "papas/graphtools/PapasEventRuler.h"
#include "papas/datatypes/Cluster.h"
#include "papas/graphtools/Distance.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/datatypes/Track.h"

namespace papas {

PapasEventRuler::PapasEventRuler(const PapasEvent& papasevent) : m_ruler(), m_papasEvent(papasevent) {}

Distance PapasEventRuler::distance(IdType id1, IdType id2) const {
  if (Identifier::isCluster(id1) && Identifier::isCluster(id2))
    if (Identifier::itemType(id1) == Identifier::itemType(id2))
      return std::move(clusterClusterDistance(id1, id2));
    else  // hcal ecal not linked
      return Distance();
  else if (Identifier::isTrack(id2) && Identifier::isCluster(id1))
    return std::move(clusterTrackDistance(id1, id2));
  else if (Identifier::isTrack(id1) && Identifier::isCluster(id2))
    return std::move(clusterTrackDistance(id2, id1));
  else if (Identifier::isTrack(id1) && Identifier::isTrack(id2))
    return std::move(Distance());
  std::cout << Identifier::itemType(id1) << ":" << Identifier::itemType(id2) << std::endl;
  throw "Distance between ids could not be computed";
  return std::move(Distance());
}

Distance PapasEventRuler::clusterClusterDistance(IdType id1, IdType id2) const {
  const Cluster& cluster1 = m_papasEvent.cluster(id1);
  const Cluster& cluster2 = m_papasEvent.cluster(id2);
  return m_ruler.clusterClusterDistance(cluster1, cluster2);
}

Distance PapasEventRuler::clusterTrackDistance(IdType clustId, IdType trackId) const {
  const Cluster& cluster = m_papasEvent.cluster(clustId);
  //const Track& track = m_papasEvent.track(trackId);
  throw "TODO track";
  return Distance();
  //return m_ruler.clusterTrackDistance(cluster, track);
}

}  // end namespace papas
