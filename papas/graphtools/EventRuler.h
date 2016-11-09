//
//  EventRuler.h
//  fastsim
//
//  Created by Alice Robson on 17/04/16.
//
//

#ifndef EventRuler_h
#define EventRuler_h

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Id.h"
#include "papas/graphtools/Ruler.h"
#include "papas/datatypes/Track.h"
#include <stdio.h>

namespace papas {

class Distance;
class PFEvent;

/// EventRuler is "wrapper" for the Ruler (distance measuring class), that allows distances between two elements
/// as identified by id to be found. It contains a reference to the event, which allows location of the underlying
/// element (cluster, track) from the id. The EventRuler measures distances/links between items(ids) that belong to
/// an event (eg cluster-cluster)
class EventRuler {

public:
  EventRuler(const PFEvent& pfevent);
  /**
   *   @brief  distance between id1 and id2
   *
   *   @param[in]  id1 : element uniqueid enerated from Id class. Must exist in PFEvent
   *   @param[in]  id2 : element2 uniqueid generated from Id class. Must exist in PFEvent
   *   @return  Distance (ie isLinked : boolean T/F and distance value)
   */
  Distance distance(IdType id1, IdType id2) const;

private:
  Distance clusterClusterDistance(IdType id1, IdType id2) const;
  Distance clusterTrackDistance(IdType id1, IdType id2) const;

  Ruler m_ruler;
  const PFEvent& m_pfEvent;
};
}  // end namespace papas

#endif /* EventRuler_hpp */
