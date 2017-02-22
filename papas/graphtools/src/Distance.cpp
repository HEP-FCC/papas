//
//  distance.cpp
//  fastsim
//
//  Created by Alice Robson on 01/02/16.
//
//

#include "papas/graphtools/Distance.h"
//#include "papas/datatypes/Identifier.h"
#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Path.h"
#include "papas/datatypes/Track.h"
#include "papas/utility/DeltaR.h"

namespace papas {

Distance::Distance(const Cluster& cluster1, const Cluster& cluster2) : m_distance(-1), m_isLinked(false) {
  // TODO check this is a bottom layer cluster
  m_distance = deltaR(cluster1.position().Theta(), cluster1.position().Phi(), cluster2.position().Theta(),
                      cluster2.position().Phi());
  m_isLinked = (m_distance < cluster1.angularSize() + cluster2.angularSize());
}

Distance::Distance() : m_distance(-1), m_isLinked(false) {}

Distance::Distance(bool islinked, double dist) : m_distance(dist), m_isLinked(islinked) {}

Distance::Distance(const Cluster& cluster, const Track& track) : m_distance(-1), m_isLinked(false) {
  papas::Position cyl_layer = papas::Position::kEcalIn;
  if (Identifier::isHcal(cluster.id())) {
    cyl_layer = papas::Position::kHcalIn;
  }
  if (track.path()->hasNamedPoint(cyl_layer)) {  // check exists
    TVector3 pos = track.path()->namedPoint(cyl_layer);
    m_distance = (cluster.position() - pos).Mag();
    m_isLinked = m_distance < cluster.size();
  }
  // TODO throw error
}

}  // end namespace papas
