#ifndef RECONSTRUCTION_EDGE_H
#define RECONSTRUCTION_EDGE_H

#include "array"
#include "papas/datatypes/IdCoder.h"
#include <iostream>

namespace papas {
/**
 *
 *  @brief An Edge stores end node ids, distance between the nodes, and whether they are linked
 *
 *  Example usage: Edge e = Edge(obj1.unqiueid, obj2.uniqueid, True, 0.0);
 *
 *  Each edge will have a unique key which is constructed from the two end ids, this key is indep of the
 *  order in which the two ids are provided. Given two ids, it is possible to find the corresponding edge key
 *  and (assuming edges have been stored in an unordered map) to locate the corresponding edge
 *
 *  @author  Alice Robson
 *  @date    2016-04-05
 */

class Edge {
public:
  /** @enum Edge::EdgeType
   *  enumeration to describe the type of edge eg an hcal to hcal edge is type kHcalHcal
   the order of the ends does not matter,
   eg for one track and one ecal the type will always be kEcalTrack (and never be a kTrackEcal)
   */
  enum EdgeType { kUnknown = 0, kEcalHcal, kEcalEcal, kEcalTrack, kHcalTrack, kHcalHcal, kTrackTrack };
  typedef uint64_t EdgeKey;
  /// Constructor
  Edge() : m_endIds({{0, 0}}), m_isLinked(false), m_distance(0){};  // extra braces to shut buggy xcode warning

  /**
   *   @brief  Edge constructor - note that the ordering of idEnd1 and idEnd2 does not matter
   *
   *   @param[in]  idEnd1 uniqueid enerated from Id class for one end
   *   @param[in]  idEnd2 uniqueid generated from Id class for other end
   *   @param[in]  isLinked whethert the two ends are linked boolean T/F
   *   @param[in]  distance distance between two elements
   */
  Edge(Identifier idEnd1, Identifier idEnd2, bool isLinked, double distance);
  std::array<Identifier, 2> endIds() const { return m_endIds; }  ///< array of ids of ends
  bool isLinked() const { return m_isLinked; }                   ///<is the edge a link
  void setLinked(bool link) { m_isLinked = link; };              ///< set the link status (to true or false)
  double distance() const { return m_distance; }                 ///<distance between the two elements
  Edge::EdgeKey key() const { return m_key; }  ///<unique key for this edge that can be found from the end ids
  EdgeType edgeType() const;                   ///<finds the EdgeType eg kEcalTrack
  Identifier otherId(Identifier id) const;     /// return the id of the other end, or -1 if id is not part of this edge
  friend std::ostream& operator<<(std::ostream& os, const Edge& egde);
  /**
    *   @brief  Static function that creates a unique key given two Identifiers
    *
    *   @description
    *     the key can be used to find an edge (within an unordered_map of edges) from its two end ids
    *     note that the order of id1 and id2 is not important
    *
    *   @param  id1 element uniqueid enerated from Id class for one end
    *   @param  id2 element2 uniqueid generated from Id class for other end
    */
  static EdgeKey
  makeKey(Identifier id1,
          Identifier id2);  ///<static function to create a unique key, key is indep of the ordering of id1 and id2
private:
  std::array<Identifier, 2> m_endIds;  ///< long identifiers for the two ends
  bool m_isLinked;                     ///< boolean to day if there is a link between the two edges
  double m_distance;                   ///< distance between two ends
  EdgeKey m_key;                       ///<unique key for this edge that allows lookup in an unordered_map of edges
};

}  // end namespace papas

#endif /* edge_h */
