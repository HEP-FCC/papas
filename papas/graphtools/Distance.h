//
//  distance.h
//  fastsim
//
//  Created by Alice Robson on 01/02/16.
//
//

#ifndef distance_h
#define distance_h

#include "TVector3.h"

namespace papas {

class Track;
class Cluster;
/// @brief Determines the distance between two items, eg clusters or clusters and tracks, and records whether or not they are linked.
class Distance {
public:
  /** Constructor
   */
  Distance();
  /** Constructor
   * @param[in] islinked boolean to say if the items are linked
   * @param[in] dist Distance between items
   */
  Distance(bool islinked, double dist);
  /** Constructor
   * Finds distance between cluster and track
   * @param[in] cluster Cluster
   * @param[in] track Track
   */
  Distance(const Cluster& cluster, const Track& track);
  /** Constructor
   * Finds distance between two clusters
   * @param[in] cluster1 First Cluster
   * @param[in] cluster2 Second Cluster
   */
  Distance(const Cluster& cluster1, const Cluster& cluster2);
  /** Constructor
   * Finds distance to a cluster from a point
   * @param[in] point position (3d)
   * @param[in] cluster Cluster
   */
  void setDistanceToPoint(const TVector3& point, const Cluster& cluster);
  bool isLinked() const { return m_isLinked; }; ///< true if the items are linked
  double distance() const { return m_distance; };  ///< distance between items

private:
  double m_distance; ///< distance between items
  bool m_isLinked; ///< true if the items are linked
};
}  // end namespace papas
#endif /* distance_h */
