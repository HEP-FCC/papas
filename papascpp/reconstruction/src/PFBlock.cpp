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
#include "spdlog/details/format.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

//#include "PFEvent.h"

namespace papas {

int PFBlock::tempBlockCount = 0;

PFBlock::PFBlock(const Ids& element_ids, Edges& edges)
    : m_uniqueId(Id::makeBlockId()), m_isActive(true), m_elementIds(element_ids) {
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

PFBlock::PFBlock() : m_uniqueId(-1), m_isActive(false), m_elementIds() {}

int PFBlock::countEcal() const {
  // Counts how many ecal cluster ids are in the block
  return std::count_if(m_elementIds.begin(), m_elementIds.end(), [](Id::Type elem) { return Id::isEcal(elem); });
}

int PFBlock::countHcal() const {
  // Counts how many hcal cluster ids are in the block
  return std::count_if(m_elementIds.begin(), m_elementIds.end(), [](Id::Type elem) { return Id::isHcal(elem); });
}

int PFBlock::countTracks() const {
  // Counts how many track ids are in the block
  return std::count_if(m_elementIds.begin(), m_elementIds.end(), [](Id::Type elem) { return Id::isTrack(elem); });
}

std::string PFBlock::shortName() const {
  // constructs a short summary name for blocks allowing sorting based on contents
  // eg 'E1H1T2' for a block with 1 ecal, 1 hcal, 2 tracks
  std::string shortName = "";
  if (countEcal()) shortName = shortName + "E" + std::to_string(countEcal());
  if (countHcal()) shortName = shortName + "H" + std::to_string(countHcal());
  if (countTracks()) shortName = shortName + "T" + std::to_string(countTracks());
  return shortName;
}

int PFBlock::size() const { return m_elementIds.size(); }

std::vector<Edge::EdgeKey> PFBlock::linkedEdgeKeys(Id::Type uniqueid, Edge::EdgeType matchtype) const {
  /**
   Returns list of keys of all edges of a given edge type that are connected to a given id.
   
   Arguments:
   uniqueid : is the id of item of interest
   edgetype : is an optional type of edge. If specified only links of the given edgetype will be returned
   */
  std::vector<Edge::EdgeKey> linkedEdgeKeys;
  for (auto edge : m_edges) {
    // if this is an edge that includes uniqueid
    std::cout<<" E" << edge.second << " "<<edge.second.otherid(uniqueid) << " "<<edge.second.isLinked()<<std::endl;
    if (edge.second.isLinked() && edge.second.otherid(uniqueid) > 0) {
      // include in list if either no matchtype is specified or if the edge is of the same matchtype
      if ((matchtype == Edge::EdgeType::kUnknown) || matchtype == edge.second.edgeType())
        linkedEdgeKeys.push_back(edge.first);
    }
  }
  //TODO consider sorting
  return linkedEdgeKeys;
}

std::vector<Id::Type> PFBlock::linkedIds(Id::Type uniqueid, Edge::EdgeType edgetype) const {
  /// Returns list of all linked ids of a given edge type that are connected to a given id -
  /// TODO think about sorting
  Ids linkedIds;
  for (auto key : linkedEdgeKeys(uniqueid, edgetype)) {
    linkedIds.push_back(m_edges.find(key)->second.otherid(uniqueid));
  }
  // std::sort(linkedIds.begin(), linkedIds.end(), [this, uniqueid](Id::Type a, Id::Type b) -> bool
  //                  { return this->compareEdges(a, b, uniqueid); } );
  return linkedIds;
}

std::string PFBlock::elementsString() const {
  /** Construct an index to the elements linking shortname used in matrix to the pretty item identifier
   For example:-
   elements: {
   E1 = E429
   H1 = H430
   T2 = T428
   }
   */
  int count = 0;
  fmt::MemoryWriter out;
  out.write("    elements:\n");
  for (auto id : m_elementIds) {
    out.write("{:>7}{} = {:9} ({})\n", Id::typeShortCode(id), count, Id::pretty(id), id);
    count = count + 1;
  }
  return out.str();
}

std::string PFBlock::edgeMatrixString() const {
  /** produces a string containing the the lower part of the matrix of distances between elements
   elements are ordered as ECAL(E), HCAL(H), Track(T)
  for example:-

  distances:
         E0       H1       T2       T3
  E0       .
  H1  0.0267        .
  T2  0.0000   0.0000        .
  T3  0.0287   0.0825      ---        .
  */
  
  int count = 0;
  fmt::MemoryWriter out;
  std::string shortid;

  if (m_elementIds.size() > 1) { // make the header line for the matrix
    out.write("    distances:\n        ");
    for (auto e1 : m_elementIds) {
      // will produce short id of form E2 H3, T4 etc in tidy format
      shortid = Id::typeShortCode(e1) + std::to_string(count);
      out.write("{:>8}", shortid);
      count += 1;
    }

    // for each element find distances to all other items that are in the lower part of the matrix
    int countrow = 0;
    for (auto e1 : m_elementIds) {  // each iteration produces the next row of the matrix
      // make short name for the row element eg E3, H5 etc
      shortid = Id::typeShortCode(e1) + std::to_string(countrow);
      out.write("\n{:>8}", shortid);
      countrow += 1;
      for (auto e2 : m_elementIds) {  // these will be the columns
        if (e1 == e2) {
          out.write("       ."); //diagonal
          break;
        } else if (edge(e1, e2).distance() < 0)
          out.write("     ---"); //-ve distance
        else if (edge(e1, e2).isLinked() == false)
          out.write("     xxx"); // not linked
        else { //linked and has distance
          out.write("{:8.4f}", edge(e1, e2).distance());
        }
      }
    }
  }
  return out.str();
}
const Edge& PFBlock::edge(Id::Type id1, Id::Type id2) const {
  /// Find the edge corresponding to e1 e2
  ///                      Note that make_key deals with whether it is get_edge(e1, e2) or get_edge(e2, e1) (either
  ///                      order gives same result)
  ///                        '''
  return m_edges.find(Edge::makeKey(id1, id2))->second;
}
std::string PFBlock::info() const {
  fmt::MemoryWriter out;
  out.write("{:8} :{:9}: ecals = {} hcals = {} tracks = {}",
            shortName(), Id::pretty(m_uniqueId), countEcal(), countHcal(), countTracks());
  return out.str();
}

std::ostream& operator<<(std::ostream& os, const PFBlock& block) {
  if (block.isActive())
    os << "block:";
  else
    os << "deactivated block:";
  os << block.info() << std::endl;
  os << block.elementsString();
  os << block.edgeMatrixString();
  return os;
}

}  // end namespace papas

int test_blocks() {
  using namespace papas;
  Id::Type id1 = Id::makeEcalId();
  Id::Type id2 = Id::makeHcalId();
  Id::Type id3 = Id::makeTrackId();

  Id::Type id4 = Id::makeEcalId();
  Id::Type id5 = Id::makeHcalId();
  Id::Type id6 = Id::makeTrackId();

  Ids ids{id1, id2, id3};
  Ids ids2{id4, id5, id6};

  Edge edge = Edge(id1, id2, false, 0.00023);
  Edge edge1 = Edge(id1, id3, true, 10030.0);
  Edge edge2 = Edge(id2, id3, true, 0.00005);

  Edge edge4 = Edge(id4, id5, false, 3.1234);
  Edge edge5 = Edge(id4, id6, true, 0.1234);
  Edge edge6 = Edge(id5, id6, true, 123.0);

  Edges edges;
  //edges.reserve(100);
  std::cout <<"ee " <<":"<<Id::pretty(id1)<<":"<<edge.key()<<edge<<std::endl;
  std::cout <<":"<<Id::pretty(id2)<<":"<<edge1.key()<<edge1<<std::endl;
  std::cout <<":"<<Id::pretty(id3)<<":"<<edge2.key()<<edge2<<std::endl;
  std::cout <<":"<<Id::pretty(id4)<<":"<<edge4.key()<<edge4<<std::endl;
  std::cout <<":"<<Id::pretty(id5)<<":"<<edge5.key()<<edge5<<std::endl;
  std::cout <<":"<<Id::pretty(id6)<<":"<<edge6.key()<<edge6<<std::endl;
  

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

