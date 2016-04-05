#include "edge.h"

/**
 *   @brief  Edge constructor
 *
 *   @param  id1 : element uniqueid enerated from Identifier class for one end
 *   @param  id2 : element2 uniqueid generated from Identifier class for other end
 *   @param  isLinked : boolean T/F
 *   @param  distance: distance between two elements
 */
Edge::Edge(longID id1, longID id2, bool isLinked, double distance) :
m_id1(id1),
m_id2(id2),
m_isLinked(isLinked),
m_distance(distance),
m_key(Edge::makeKey(id1, id2)),
m_edgeType(makeEdgeType())
{
}


/**
 *   @brief  Static function that creates a unique key given two longIDs
 *
 *   @description
 *     the key can be used to find an edge (within an unordered_map of edges) from its two end ids
 *     note that the order of id1 and id2 is not important
 *
 *   @param  id1 : element uniqueid enerated from Identifier class for one end
 *   @param  id2 : element2 uniqueid generated from Identifier class for other end
 */
long long Edge::makeKey(longID id1, longID id2) {

  long long key;
  if (id1 > id2) //ensure that the order of the ids does not matter
    key= (id1 << 32) | id2;
  else
    key= (id2 << 32) | id1;
  return key;
}

Edge::enumEdgeType Edge::makeEdgeType() {
  /** Produces an edge_type enumeration such as kEcalTrack
   the order of id1 an id2 does not matter,
   eg for one track and one ecal the type will always be kEcalTrack (and never be a kTrackEcal)
   */
  
  auto shortid1 = Identifier::typeShortCode(m_id1);
  auto shortid2 = Identifier::typeShortCode(m_id2);
  
  if (shortid1==shortid2) {
    if (shortid1== 'h')
      return enumEdgeType::kHcalHcal;
    else if (shortid1=='e')
      return enumEdgeType::kEcalEcal;
    else if (shortid1=='t')
      return enumEdgeType::kTrackTrack;
  }
  else if ((shortid1=='h' && shortid2=='t') || (shortid1=='t' && shortid2=='h'))
    return enumEdgeType::kHcalTrack;
  else if ((shortid1=='e' && shortid2=='t') || (shortid1=='t' && shortid2=='e'))
    return enumEdgeType::kEcalTrack;
  else if ((shortid1=='e' && shortid2=='h') || (shortid1=='h' && shortid2=='e'))
    return enumEdgeType::kEcalHcal;
  else
    return enumEdgeType::kUnknown;
  
}

