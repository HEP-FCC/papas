#include "edge.h"
#include "identifier.h"

Edge::Edge(longID id1, longID id2, bool isLinked, double distance) :
m_id1(id1),
m_id2(id2),
m_isLinked(isLinked),
m_distance(distance),
m_edgeType(makeEdgeType()),
m_key(Edge::makeKey(id1, id2))
{
}


/** Static function. Makes a unique key that can be used to locate the required edge
 */
long long Edge::makeKey(longID id1, longID id2) {

  long long key;
  if (id1 > id2) //ensure that the order of the ids does not matter
    key= (id1 << 32) | id2;
  else
    key= (id2 << 32) | id1;
  return key;
}

Edge::longID Edge::otherID(longID id) const
{
  if (m_id1==id)
    return m_id2;
  else if (m_id2==id)
    return m_id1;
  return -1;
}

Edge::EdgeType Edge::makeEdgeType() const {
  // Produces an EdgeType enumeration such as kEcalTrack
  // the order of id1 an id2 does not matter,
  // eg for one track and one ecal the type will always be kEcalTrack (and never be a kTrackEcal)
 
  //get one letter abbreviation of type eg 't' for a track
  auto shortid1 = Identifier::typeShortCode(m_id1);
  auto shortid2 = Identifier::typeShortCode(m_id2);
  
  if (shortid1==shortid2) {
    if (shortid1== 'h')
      return EdgeType::kHcalHcal;
    else if (shortid1=='e')
      return EdgeType::kEcalEcal;
    else if (shortid1=='t')
      return EdgeType::kTrackTrack;
  }
  else if ((shortid1=='h' && shortid2=='t') || (shortid1=='t' && shortid2=='h'))
    return EdgeType::kHcalTrack;
  else if ((shortid1=='e' && shortid2=='t') || (shortid1=='t' && shortid2=='e'))
    return EdgeType::kEcalTrack;
  else if ((shortid1=='e' && shortid2=='h') || (shortid1=='h' && shortid2=='e'))
    return EdgeType::kEcalHcal;
  return EdgeType::kUnknown;
  
}


std::ostream& operator<<(std::ostream& os, const Edge& edge) {
  os << "Edge: " << edge.m_id1<<"<->" << edge.m_distance;
  os << " (link = " << std::boolalpha << edge.m_isLinked << ")" ;
  return os;
}

int test_edges() {
  Edge::longID id1=Identifier::makeECALClusterID();
  Edge::longID id2=Identifier::makeHCALClusterID();
  Edge::longID id3=Identifier::makeTrackID();
  
  Edge edge=Edge(id1,id2,false,0.0);
  Edge edge1=Edge(id1,id3,true,0.0);
  Edge edge2=Edge(id2,id3,true,0.0);
  
  std::cout<<edge1<<std::endl;
  
  
}

