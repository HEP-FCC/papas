//
//  pfblock.cpp
//  fastsim
//
//  Created by Alice Robson on 05/04/16.
//
//

#include "PFBlock.h"

//#include <boost/format.hpp>
#include "Edge.h"
#include "Id.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <vector>
#
//#include "PFEvent.h"

int PFBlock::tempBlockCount = 0;

PFBlock::PFBlock(const Ids& element_ids, Edges& edges)
    : m_uniqueId(Id::makeBlockId()),
      m_isActive(true),
      m_blockCount(PFBlock::tempBlockCount),
      m_elementIds(element_ids) {
  PFBlock::tempBlockCount += 1;

  // extract the relevant parts of the complete set of edges and store this within the block
  // note the edges will be removed from the edges unordered_map
  for (auto id1 : m_elementIds) {
    for (auto id2 : m_elementIds) {
      if (id1 >= id2) continue;
      // move the edge from one unordered map to the other
      auto e = edges.find(Edge::makeKey(id1, id2));
      m_edges.emplace(e->second.key(), std::move(e->second));
      edges.erase(e);
    }
  }
}

PFBlock::PFBlock() : m_uniqueId(-1), m_isActive(false), m_blockCount(-1), m_elementIds() {}

int PFBlock::countEcal() const {
  // Counts how many ecal cluster ids are in the block
  return std::count_if(m_elementIds.begin(), m_elementIds.end(), [](Id::type elem) { return Id::isEcal(elem); });
}

int PFBlock::countHcal() const {
  // Counts how many hcal cluster ids are in the block
  return std::count_if(m_elementIds.begin(), m_elementIds.end(), [](Id::type elem) { return Id::isHcal(elem); });
}

int PFBlock::countTracks() const {
  // Counts how many track ids are in the block
  return std::count_if(m_elementIds.begin(), m_elementIds.end(), [](Id::type elem) { return Id::isTrack(elem); });
}

std::string PFBlock::shortName() const {
  // constructs a short summary name for blocks allowing sorting based on contents
  // eg 'E1H1T2' for a block with 1 ecal, 1 hcal, 2 tracks
  //
  std::string shortName = "";
  if (countEcal()) shortName = shortName + "E" + std::to_string(countEcal());
  if (countHcal()) shortName = shortName + "H" + std::to_string(countHcal());
  if (countTracks()) shortName = shortName + "T" + std::to_string(countTracks());

  return shortName;
}

int PFBlock::size() const { return m_elementIds.size(); }

std::vector<long long> PFBlock::linkedEdgeKeys(Id::type uniqueid, Edge::EdgeType matchtype) const {
  /**
   Returns list of keys of all edges of a given edge type that are connected to a given id.
   The list is sorted in order of increasing distance

   Arguments:
   uniqueid : is the id of item of interest
   edgetype : is an optional type of edge. If specified only links of the given edgetype will be returned
   */
  std::vector<long long> linkedEdgeKeys;
  for (auto edge : m_edges) {
    // if this is an edge that includes uniqueid
    if (edge.second.isLinked() && edge.second.otherid(uniqueid) > 0) {
      // include in list if either no matchtype is specified or if the edge is of the same matchtype
      if ((matchtype == Edge::EdgeType::kUnknown) || matchtype == edge.second.edgeType())
        linkedEdgeKeys.push_back(edge.first);
    }
  }
  return linkedEdgeKeys;
}
/*

 #this is a bit yucky and temporary solution as need to make sure the order returned is consistent
 # maybe should live outside of this class
 linked_edges.sort( key = lambda x: (x.distance is None, x.distance))
 return linked_edges*/

std::vector<long> PFBlock::linkedIds(Id::type uniqueid, Edge::EdgeType edgetype) const {
  /// Returns list of all linked ids of a given edge type that are connected to a given id -
  /// sorted in order of increasing distance
  /** returns a list of the otherids sorted by distance to uniqueid and by decreasing energies

   eg if uniqueid is an hcal
   and other ids are  track1 energy = 18, dist to hcal = 0.1
   track2 energy = 9,  dist to hcal = 0
   track3 energy = 4,  dist to hcal = 0
   this will return {track2, track3, track1}
   */
  Ids linkedIds;
  for (auto key : linkedEdgeKeys(uniqueid, edgetype)) {
    linkedIds.push_back(m_edges.find(key)->second.otherid(uniqueid));
  }
  // std::sort(linkedIds.begin(), linkedIds.end(), [this, uniqueid](Id::type a, Id::type b) -> bool
  //                  { return this->compareEdges(a, b, uniqueid); } );
  return linkedIds;
}

std::string PFBlock::elementsString() const {
  /** Construct a string descrip of each of the elements in a block:-
   The elements are given a short name E/H/T according to ecal/hcal/track
   and then sequential numbering starting from 0, this naming is also used to index the
   matrix of distances. The full unique id is also given.
   For example:-
   elements: {
   E0:1104134446736:SmearedCluster : ecal_in       0.57  0.33 -2.78
   H1:2203643940048:SmearedCluster : hcal_in       6.78  0.35 -2.86
   T2:3303155568016:SmearedTrack   :    5.23    4.92  0.34 -2.63
   }
   */
  int count = 0;
  std::string offset = "      ";
  std::string s = offset + "elements: \n";
  for (auto id : m_elementIds) {
    s += offset + "          " + Id::typeShortCode(id) + std::to_string(count) + ": " + std::to_string(id) +
        "\n";
    count = count + 1;
  }
  /*elemdetails = "\n      elements: {\n"
   for (auto uid : m_elementUniqueIds:
   elemdetails += "      {shortName}{count}:{strdescrip}".format(shortName=Identifier.type_short_code(uid),
   count=count,
   strdescrip=self.pfevent.get_object(uid).__str__() )

   std::string s = (boost::format("%d") % 1).str();
   count = count + 1*/
  return s;
}

std::string PFBlock::edgeMatrixString() const {
  /* produces a string containing the the lower part of the matrix of distances between elements
   elements are ordered as ECAL(E), HCAL(H), Track(T)
  for example:-

  distances:
         E0       H1       T2       T3
  E0       .
  H1  0.0267        .
  T2  0.0000   0.0000        .
  T3  0.0287   0.0825      ---        .
  */

  // make the header line for the matrix
  int count = 0;
  std::ostringstream os;
  os.precision(2);
  std::string offset = "      ";
  // os<< offset + "distances:\n     " + offset;
  os << offset + "distances:  ";

  std::string shortid;
  for (auto e1 : m_elementIds) {
    // will produce short id of form E2 H3, T4 etc in tidy format
    shortid = Id::typeShortCode(e1) + std::to_string(count);
    os << std::setw(9) << shortid;
    count += 1;
  }
  os << std::endl;

  // for each element find distances to all other items that are in the lower part of the matrix
  int countrow = 0;
  std::string rowstr = "";
  std::string rowname = "";
  std::string colname = "";
  for (auto e1 : m_elementIds) {  // this will be the rows
    rowstr = "";
    // make short name for the row element eg E3, H5 etc
    os << std::setw(18) << Id::typeShortCode(e1) + std::to_string(countrow);
    countrow += 1;
    for (auto e2 : m_elementIds) {  // these will be the columns
      if (e1 == e2) {
        os << "        .";
        break;
      } else if (edge(e1, e2).distance() < 0)
        os << "      ---";
      else if (edge(e1, e2).isLinked() == false)
        os << "      xxx";
      else {
        os << std::setw(9) << std::fixed << edge(e1, e2).distance();
      }
    }
    os << " \n";
  }
  return os.str();
}

const Edge& PFBlock::edge(Id::type id1, Id::type id2) const {
  /// Find the edge corresponding to e1 e2
  ///                      Note that make_key deals with whether it is get_edge(e1, e2) or get_edge(e2, e1) (either
  ///                      order gives same result)
  ///                        '''
  return m_edges.find(Edge::makeKey(id1, id2))->second;
}

std::ostream& operator<<(std::ostream& os, const PFBlock& block) {

  if (block.m_isActive)
    os << "block: ";
  else
    os << "deactivated block: ";
  os << block.shortName();
  os << " id=" << block.m_blockCount << " uid=" << block.m_uniqueId;
  os << " ecals= " << block.countEcal() << " hcals= " << block.countHcal() << " tracks= " << block.countTracks()
     << "\n";

  os << block.elementsString() << std::endl;
  os << block.edgeMatrixString() << std::endl;

  //    return descrip
  return os;
}

int test_blocks() {
  Id::type id1 = Id::makeECALClusterId();
  Id::type id2 = Id::makeHCALClusterId();
  Id::type id3 = Id::makeTrackId();

  Id::type id4 = Id::makeECALClusterId();
  Id::type id5 = Id::makeHCALClusterId();
  Id::type id6 = Id::makeTrackId();

  PFBlock::Ids ids{id1, id2, id3};
  PFBlock::Ids ids2{id4, id5, id6};

  Edge edge = Edge(id1, id2, false, 0.00023);
  Edge edge1 = Edge(id1, id3, true, 10030.0);
  Edge edge2 = Edge(id2, id3, true, 0.00005);

  Edge edge4 = Edge(id4, id5, false, 3.1234);
  Edge edge5 = Edge(id4, id6, true, 0.1234);
  Edge edge6 = Edge(id5, id6, true, 123.0);

  PFBlock::Edges edges;
  edges.reserve(100);

  // edges.emplace(10000.0,  std::move(edge));
  edges.emplace(edge.key(), std::move(edge));
  edges.emplace(edge1.key(), std::move(edge1));
  edges.emplace(edge2.key(), std::move(edge2));
  edges.emplace(edge4.key(), std::move(edge4));
  edges.emplace(edge5.key(), std::move(edge5));
  edges.emplace(edge6.key(), std::move(edge6));

  PFBlock block(ids, edges);
  PFBlock block2(ids2, edges);

  std::cout << block;
  std::cout << block2;
  std::cout << edges.size();
  return 0;
}

/*

def __str__(self):
Block description which includes list of elements and a matrix of distances
Example:
block: E1H1T1       id=  39 :uid= 6601693505424: ecals = 1 hcals = 1 tracks = 1
elements: {
E0:1104134446736:SmearedCluster : ecal_in       0.57  0.33 -2.78
H1:2203643940048:SmearedCluster : hcal_in       6.78  0.35 -2.86
T2:3303155568016:SmearedTrack   :    5.23    4.92  0.34 -2.63
}
distances:
E0       H1       T2
E0       .
H1  0.0796        .
T2  0.0210   0.0000        .
}
*/
/*
 if (self.is_active):
 descrip= "\nblock:"
 else:
 descrip= "\ndeactivated block:"

 descrip += str('{shortName:<12} id={blockid:4.0f} :uid= {uid}: ecals = {count_ecal} hcals = {count_hcal} tracks =
 {count_tracks}'.format(
 shortName    = self.short_name(),
 blockid      = self.block_count,
 uid          = self.uniqueid,
 count_ecal   = self.count_ecal(),
 count_hcal   = self.count_hcal(),
 count_tracks = self.count_tracks() )
 )
 descrip += self.elements_string()
 descrip += self.edge_matrix_string()
 return descrip

 def __repr__(self):
 return self.__str__()

 */