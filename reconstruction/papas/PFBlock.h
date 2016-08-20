#ifndef RECONSTRUCTION_PFBLOCK_H
#define RECONSTRUCTION_PFBLOCK_H

#include "DefinitionsCollections.h"
#include "DefinitionsNodes.h"
#include "Edge.h"
#include <iostream>
#include <string>

/** @class   rec::PFBlock Reconstruction/Reconstruction/PFBlock.h PFBlock.h
 *
 *  @brief PFBlock
 *  Example usage: PFBlock
 *
 *  @author  Alice Robson
 *  @date    2016-04-05
 */

namespace papas {

/** A Particle Flow Block (PFBlock) stores a set of element ids that are connected to each other
 together with the edge data (distances) for each possible edge combination

 class attributes:

 IdType m_uniqueid : the block's unique id generated from Id class
 Ids m_elementIds : list of uniqueids of its elements

 Edges m_edges : Dictionary of all the edge cominations in the block dict{edgekey : Edge}
          use  findEdge(id1,id2) to find an edge
 bool m_isActive : bool true/false, set to false if the block is subsequently subdivided
 static int tempBlockCount: sequential numbering of blocks (useful for debugging/tracing etc)

 Usage:
 block = PFBlock(element_ids,  edges, pfevent)
 os << block;
 */

// import itertools

class PFBlock {

public:
  /** Constructor
   @param[in] const Ids& element_ids:  vector of uniqueids of the elements to go in this block [id1,id2,...]
   @param[inout] Edges& edges: is an unordered map of edges, it must contain at least all needed edges. It is not a
   problem
   if it contains additional edges as only the ones needed will be extracted. Note that edges that are extracted will be
   removed from the Edges object.
   */
  PFBlock(const Ids& elementIds, Edges& edges);
  PFBlock();
  ~PFBlock() = default;
  PFBlock(PFBlock&& pfblock) = default;

  const Ids elementIds() const { return m_elementIds; }  ///< returns vector of all ids in the block
  Edge& findEdge(Edge::EdgeKey key) { return m_edges.find(key)->second; }
  const Edge& findEdge(Edge::EdgeKey key) const { return m_edges.find(key)->second; }

  /**
  Returns list of all edges of a given edge type that are connected to a given id.
  The list is sorted in order of increasing distance
  @param[in] uniqueid : is the id of item of interest
  @param[in] edgetype : is an optional type of edge. If specified only links of the given edgetype will be returned
  @return vector of EdgeKeys to linked edges
 */
  std::vector<Edge::EdgeKey> linkedEdgeKeys(IdType uniqueid,
                                            Edge::EdgeType matchtype = Edge::EdgeType::kUnknown) const;

  /**
  Returns list of all linked ids of a given edge type that are connected to a given id
   @param[in] uniqueId : is the id of item of interest
   @param[in] edgetype : is an optional type of edge. If specified only links of the given edgetype will be returned
   @return vector of ids that are linked to the uniqueid
  */
  Ids linkedIds(IdType uniqueId, Edge::EdgeType edgetype = Edge::EdgeType::kUnknown) const;

  std::string shortName() const;  ///< Short descriptor of block such as E3H1T2 (three Ecals, 1 Hcal, 2 tracks)
  int countEcal() const;          ///< Counts how many ecal cluster ids are in the block
  int countHcal() const;          ///< Counts how many hcal cluster ids are in the block
  int countTracks() const;        ///< Counts how many tracks are in the block
  int size() const;               ///< length of the element_unqiueids
  IdType uniqueId() const { return m_uniqueId; };      ///<Unique ID of the block
  bool isActive() const { return m_isActive; };          /// Blocks that have been split will be deactivated
  void setActive(bool active) { m_isActive = active; };  /// active/ deactivate block
  Edges& edges() { return m_edges; }
  std::string info() const;
  std::string elementsString() const;
  std::string edgeMatrixString() const;
  const class Edge& edge(IdType id1, IdType id2) const;

private:
  PFBlock(PFBlock& pfblock) = default;           //{std::cout << "COPY BLOCK";};
  PFBlock(const PFBlock& pfblock) = default;     //{std::cout << "COPY CONST BLOCK";};
  PFBlock& operator=(const PFBlock&) = default;  //{std::cout << "= BLOCK"; };//return PFBlock(c);};

  IdType m_uniqueId;        //  make a uniqueid for this block
  bool m_isActive;            // if a block is subsequently split it will be deactivated
  Ids m_elementIds;           // elements in this block ordered by type and decreasing energy
  Edges m_edges;              // all the edges for elements in this block
  static int tempBlockCount;  // sequential numbering of blocks, not essential but helpful for debugging
};

std::ostream& operator<<(std::ostream& os, const PFBlock& block);
}  // end namespace papas

#endif /* PFBlock_h */
