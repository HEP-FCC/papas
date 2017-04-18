#include "papas/graphtools/Edge.h"
#include "papas/utility/PDebug.h"
#include <math.h>

#include "papas/datatypes/IdCoder.h"

namespace papas {

Edge::Edge(Identifier endId1, Identifier endId2, bool isLinked, double distance)
    : m_endIds({{endId1, endId2}}),  // extra braces to shutup buggy xcode warning
      m_isLinked(isLinked),
      m_distance(distance),
      m_key(Edge::makeKey(endId1, endId2)) {}

  Edge::~Edge() {
    PDebug::write("Delete {} ", *this);};

/** Static function. Makes a unique key that can be used to locate the required edge
 */
Edge::EdgeKey Edge::makeKey(Identifier id1, Identifier id2) {
  EdgeKey key;
  Identifier uid1 = IdCoder::uniqueId(id1);
  Identifier uid2 = IdCoder::uniqueId(id2);

  if (id1 < id2)  // ensure that the order of the ids does not matter
    std::swap(uid1, uid2);
  key = (((uint64_t)uid1) << 32) | ((uint64_t)uid2);
  return key;
}

Identifier Edge::otherId(Identifier id) const {
  if (m_endIds[0] == id)
    return m_endIds[1];
  else if (m_endIds[1] == id)
    return m_endIds[0];
  return 0;
}

// perhaps this should live somewhere else as it makes Edge class less general
Edge::EdgeType Edge::edgeType() const {
  // Produces an EdgeType enumeration such as kEcalTrack
  // NB for one track and one ecal the type will always be kEcalTrack (and never be a kTrackEcal)

  // get one letter abbreviation of type eg 't' for a track
  auto shortid1 = IdCoder::typeLetter(m_endIds[0]);
  auto shortid2 = IdCoder::typeLetter(m_endIds[1]);

  if (shortid1 == shortid2) {
    if (shortid1 == 'h')
      return EdgeType::kHcalHcal;
    else if (shortid1 == 'e')
      return EdgeType::kEcalEcal;
    else if (shortid1 == 't')
      return EdgeType::kTrackTrack;
  } else if ((shortid1 == 'h' && shortid2 == 't') || (shortid1 == 't' && shortid2 == 'h'))
    return EdgeType::kHcalTrack;
  else if ((shortid1 == 'e' && shortid2 == 't') || (shortid1 == 't' && shortid2 == 'e'))
    return EdgeType::kEcalTrack;
  else if ((shortid1 == 'e' && shortid2 == 'h') || (shortid1 == 'h' && shortid2 == 'e'))
    return EdgeType::kEcalHcal;
  return EdgeType::kUnknown;
}

std::ostream& operator<<(std::ostream& os, const Edge& edge) {
  os << "Edge: " << edge.m_endIds[0] << "<->" << edge.m_endIds[1] << ": " << edge.m_distance;
  os << " (link = " << std::boolalpha << edge.m_isLinked << ")";
  return os;
}

}  // end namespace papas
