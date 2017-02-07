#include "papas/graphtools/Edge.h"
#include "math.h"

namespace papas {

Edge::Edge(IdType endId1, IdType endId2, bool isLinked, double distance)
  : m_endIds({{endId1, endId2}}), //extra braces to shutup buggy xcode warning
  m_isLinked(isLinked),
      m_distance(distance),
      m_key(Edge::makeKey(endId1, endId2)) {}

/** Static function. Makes a unique key that can be used to locate the required edge
 */
Edge::EdgeKey Edge::makeKey(IdType id1, IdType id2) {

  EdgeKey key;
    // Shift all the parts and join together
    // NB uint64_t is needed to make sure the shift is carried out over 64 bits, otherwise
    // if the m_bitshift is 32 or more the shift is undefined and can return 0
  auto temp = id1;
  if (id1 > id2) {
      id1 = id2;
      id2 = temp;
  }
  
  
    IdType typeShift1 = (uint64_t)Identifier::itemType(id1) << m_bitshift1;
    IdType subtypeShift1 = (uint64_t) static_cast<int>(tolower(Identifier::subtype(id1))) << m_bitshift;
    IdType typeShift2 = (uint64_t)Identifier::itemType(id2) << (m_bitshift1 +32) ;
    IdType subtypeShift2 = (uint64_t) static_cast<int>(tolower(Identifier::subtype(id2))) << (m_bitshift + 32);
    IdType shiftid2 = (uint64_t)Identifier::uniqueId(id2) << 32;
  //std::cout <<Identifier::itemType(id1) << Identifier::subtype(id1) << Identifier::uniqueId(id1) << " : "<< Identifier::itemType(id2) << Identifier::subtype(id2)<< Identifier::uniqueId(id2) << std::endl;
  
    key = (uint64_t)subtypeShift1 | (uint64_t)typeShift1 | (uint64_t)Identifier::uniqueId(id1) | (uint64_t)shiftid2
  | (uint64_t)subtypeShift2 | (uint64_t)typeShift2;
  
  
  
   /* std::cout <<((key >> m_bitshift1) & (uint64_t)(pow(2, 3) - 1));
  std::cout << static_cast<char>((key >> m_bitshift) & (uint64_t)(pow(2, m_bitshift1 - m_bitshift) - 1));
  std::cout <<((key) & (uint64_t)(pow(2, m_bitshift) - 1));
  std::cout << " : ";
  std::cout << ((key >> (m_bitshift1+32)) & (uint64_t)(pow(2, 3) - 1));
  std::cout << static_cast<char>((key >> (m_bitshift+32)) & (uint64_t)(pow(2, m_bitshift1 - m_bitshift) - 1));
  std::cout << ((key >> 32) & (uint64_t)(pow(2, m_bitshift) - 1));*/
                                                                                                                                
    return key;
}
  
  
  /*bool Edge::checkValid(uint64_t uid, IdType id, IdType id2) {
    return ((id >> m_bitshift1) & (uint64_t)(pow(2, 3) - 1)) == Identifier::itemType(id);
    
  }*/

IdType Edge::otherid(IdType id) const {
  if (m_endIds[0] == id)
    return m_endIds[1];
  else if (m_endIds[1] == id)
    return m_endIds[0];
  return 0;
}

  //perhaps this should live somewhere else as it makes Edge class less general
Edge::EdgeType Edge::edgeType() const{
  // Produces an EdgeType enumeration such as kEcalTrack
  // NB for one track and one ecal the type will always be kEcalTrack (and never be a kTrackEcal)

  // get one letter abbreviation of type eg 't' for a track
  auto shortid1 = Identifier::typeLetter(m_endIds[0]);
  auto shortid2 = Identifier::typeLetter(m_endIds[1]);

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
