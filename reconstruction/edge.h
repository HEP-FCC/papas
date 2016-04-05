#ifndef RECONSTRUCTION_EDGE_H
#define RECONSTRUCTION_EDGE_H
#include "identifier.h"


/**
 *  @file    Edge.cpp
 *  @author  Alice Robson
 *  @date    05/04/2016
 *
 *  @brief An Edge stores end node ids, distance between the nodes, and whether they are linked
 *
 */


class Edge {
public:
  typedef long longID; /// unique long
  enum class enumEdgeType {
    kEcalHcal, kEcalEcal, kEcalTrack, kHcalTrack, kHcalHcal, kTrackTrack, kUnknown
  };
  Edge(longID id1, longID id2, bool isLinked, double distance);
  bool isLinked() const {return m_isLinked;}  ///<boolean to mark if this edge links the two elements
  double distance() const {return m_distance;} ///<distance between the two elements
  long long key() const {return m_key;} ///<unique key for this edge that can be found from the two element ids
  enumEdgeType edgeType() const {return m_edgeType;} ///<describes what types of elements are connected
  static long long makeKey(longID id1, longID id2);  ///<static function to create a unique key
  
private:
  enumEdgeType makeEdgeType(); ///< sets the edge type enum
  long m_id1; ///< long identifier for one end of the edge (order does not matter)
  long m_id2; ///< long identifier for other end of ede (order does not matter)
  bool m_isLinked; ///< boolean to day if there is a link between the two edges
  double m_distance; ///< distance between two ends
  enumEdgeType m_edgeType; ///<enum describing the edge type eg ecal-hcal or track-track
  long long m_key; ///<unique key for this edge that allows lookup in an unordered_map of edges
};

#endif /* edge_h */
