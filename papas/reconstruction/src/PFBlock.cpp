#include "papas/reconstruction/PFBlock.h"

#include <algorithm>
#include <iomanip>  //needed for lxplus
#include <vector>

#include "papas/utility/PDebug.h"

namespace papas {

int PFBlock::tempBlockCount = 0;

bool blockIdComparer(Identifier id1, Identifier id2) {
  if (IdCoder::type(id1) == IdCoder::type(id2))
    return id1 > id2;
  else
    return IdCoder::type(id1) < IdCoder::type(id2);
}

PFBlock::PFBlock(const Ids& element_ids, const Edges& edges, unsigned int index, char subtype)
    : m_id(IdCoder::makeId(index, IdCoder::kBlock, subtype, element_ids.size())), m_elementIds(element_ids) {
  PFBlock::tempBlockCount += 1;
  // extract the relevant parts of the complete set of edges and store this within the block
  // note the edges will be removed from the edges unordered_map
  for (auto id1 : m_elementIds) {
    for (auto id2 : m_elementIds) {
      if (id1 >= id2) continue;
      // move the edge from one unordered map to the other
      auto e = edges.find(Edge::makeKey(id1, id2)); //now makes a copy
      if (e != edges.end()) {
        m_edges.emplace(e->second.key(), std::move(e->second));
        //edges.erase(e);
      }
    }
  }
}
  
  /*PFBlock:: ~PFBlock(){
    PDebug::write("Delete {}", *this);
    m_elementIds.clear();
    m_edges.clear();
  };*/
  

int PFBlock::countEcal() const {
  // Counts how many ecal cluster ids are in the block
  return std::count_if(m_elementIds.begin(), m_elementIds.end(), [](Identifier elem) { return IdCoder::isEcal(elem); });
}

int PFBlock::countHcal() const {
  // Counts how many hcal cluster ids are in the block
  return std::count_if(m_elementIds.begin(), m_elementIds.end(), [](Identifier elem) { return IdCoder::isHcal(elem); });
}

int PFBlock::countTracks() const {
  // Counts how many track ids are in the block
  return std::count_if(m_elementIds.begin(), m_elementIds.end(),
                       [](Identifier elem) { return IdCoder::isTrack(elem); });
}

std::string PFBlock::shortName() const {
  // constructs a short summary name for blocks allowing sorting based on contents
  // eg 'E1H1T2' for a block with 1 ecal, 1 hcal, 2 tracks
  fmt::MemoryWriter out;
  if (countEcal()) out.write("E{}", countEcal());
  if (countHcal()) out.write("H{}", countHcal());
  if (countTracks()) out.write("T{}", countTracks());
  return out.str();
}

const Edge& PFBlock::edge(Edge::EdgeKey key) const {
  auto edge = m_edges.find(key);
  if (edge == m_edges.end()) throw std::range_error("Edge not found");
  return edge->second;
}

const Edge& PFBlock::edge(Identifier id1, Identifier id2) const { return edge(Edge::makeKey(id1, id2)); }

std::list<Edge::EdgeKey> PFBlock::linkedEdgeKeys(Identifier id, Edge::EdgeType matchtype) const {
  std::list<Edge::EdgeKey> linkedEdgeKeys;
  for (auto const& edge : m_edges) {
    // if this is an edge that includes the id
    if (edge.second.isLinked() && edge.second.otherId(id) > 0) {
      // include in list if either no matchtype is specified or if the edge is of the same matchtype
      if ((matchtype == Edge::EdgeType::kUnknown) || matchtype == edge.second.edgeType())
        linkedEdgeKeys.push_back(edge.first);
    }
  }
  return linkedEdgeKeys;  // todo consider sorting
}

Ids PFBlock::linkedIds(Identifier id, Edge::EdgeType edgetype) const {
  /// Returns list of all linked ids of a given edge type that are connected to a given id -
  Ids linkedIds;
  for (auto key : linkedEdgeKeys(id, edgetype)) {
    auto found = m_edges.find(key);
    if (found == m_edges.end()) throw std::range_error("Required EdgeKey is missing from Linked Edges collection");
    linkedIds.insert(found->second.otherId(id));
  }
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
    out.write("{:>7}{} = {:9} value={:5.1f} ({})\n", IdCoder::typeLetter(id), count, IdCoder::pretty(id),
              IdCoder::value(id), id);
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

  if (m_elementIds.size() > 1) {  // make the header line for the matrix
    out.write("    distances:\n        ");
    for (auto e1 : m_elementIds) {
      // will produce short id of form E2 H3, T4 etc in tidy format
      shortid = IdCoder::typeLetter(e1) + std::to_string(count);
      out.write("{:>8}", shortid);
      count += 1;
    }

    // for each element find distances to all other items that are in the lower part of the matrix
    int countrow = 0;
    for (auto e1 : m_elementIds) {  // each iteration produces the next row of the matrix
      // make short name for the row element eg E3, H5 etc
      shortid = IdCoder::typeLetter(e1) + std::to_string(countrow);
      out.write("\n{:>8}", shortid);
      countrow += 1;
      for (auto e2 : m_elementIds) {  // these will be the columns
        if (e1 == e2) {
          out.write("       .");  // diagonal
          break;
        } else if (hasEdge(e1, e2)) {
          const auto& edge = PFBlock::edge(e1, e2);
          if (edge.distance() < 0 || edge.isLinked() == false)
            out.write("     ---");  // not linked/no distance
          else {                    // linked and has distance
            out.write("{:8.4f}", edge.distance());
          }
        } else {                  // no edge so no link
          out.write("     ---");  // not linked
        }
      }
    }
    out.write("\n");
  }
  return out.str();
}

bool PFBlock::hasEdge(Identifier id1, Identifier id2) const {
  /// Find the edge corresponding to e1 e2
  ///                      Note that make_key deals with whether it is get_edge(e1, e2) or get_edge(e2, e1) (either
  ///                      order gives same result)
  ///
  bool found = true;
  auto edge = m_edges.find(Edge::makeKey(id1, id2));
  if (edge == m_edges.end()) found = false;
  return found;
}

std::string PFBlock::info() const {  // One liner summary of PFBlock
  fmt::MemoryWriter out;
  out.write("{:8} :{:6}:{} ecals = {} hcals = {} tracks = {}", shortName(), IdCoder::pretty(m_id), m_id, countEcal(),
            countHcal(), countTracks());
  return out.str();
}

std::ostream& operator<<(std::ostream& os, const PFBlock& block) {
  os << "block:" << block.info() << std::endl;
  os << block.elementsString();
  os << block.edgeMatrixString();
  return os;
}

}  // end namespace papas
