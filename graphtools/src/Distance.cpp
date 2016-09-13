//
//  distance.cpp
//  fastsim
//
//  Created by Alice Robson on 01/02/16.
//
//

#include "Distance.h"
//#include "Id.h"
#include "Cluster.h"
#include "DeltaR.h"
#include "Path.h"
#include "pTrack.h"

namespace papas {

Distance::Distance(const Cluster& cluster1, const Cluster& cluster2) : m_distance(-1), m_isLinked(false) {
  // TODO check this is a bottom layer cluster
  m_distance = deltaR(cluster1.position().Theta(), cluster1.position().Phi(), cluster2.position().Theta(),
                      cluster2.position().Phi());
  m_isLinked = (m_distance < cluster1.angularSize() + cluster2.angularSize());
}

void Distance::setDistanceToPoint(const TVector3& point, const Cluster& cluster) {
  m_distance = (cluster.position() - point).Mag();
  m_isLinked = m_distance < cluster.size();
}

Distance::Distance() : m_distance(-1), m_isLinked(false) {}

Distance::Distance(bool islinked, double dist) : m_distance(dist), m_isLinked(islinked) {}

Distance::Distance(const Cluster& cluster, const Track& track) : m_distance(-1), m_isLinked(false) {
  papas::Position cyl_layer = papas::Position::kEcalIn;
  if (Id::isHcal(cluster.id())) {
    cyl_layer = papas::Position::kHcalIn;
  }
  if (track.path()->hasNamedPoint(cyl_layer)) {  // check exists
    TVector3 pos = track.path()->namedPoint(cyl_layer);
    setDistanceToPoint(pos, cluster);
  }
  // TODO throw error
}

}  // end namespace papas
