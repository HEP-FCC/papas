#ifndef RECONSTRUCTION_PFBLOCK_H
#define RECONSTRUCTION_PFBLOCK_H


#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

#include "Edge.h"

/** @class   rec::PFBlock Reconstruction/Reconstruction/PFBlock.h PFBlock.h
 *
 *  @brief PFBlock
 *  Example usage: PFBlock
 *
 *  @author  Alice Robson
 *  @date    2016-04-05
 */

//TODO namespace

/** A Particle Flow Block stores a set of element ids that are connected to each other
 together with the edge data (distances) for each possible edge combination
 
 class attributes:
 
 uniqueid : the block's unique id generated from Identifier class
 element_uniqueids : list of uniqueids of its elements
 pfevent : contains the tracks and clusters and a get_object method to allow access to the
 underlying objects given their uniqueid
 edges : Dictionary of all the edge cominations in the block dict{edgekey : Edge}
 use  get_edge(id1,id2) to find an edge
 is_active : bool true/false, set to false if the block is subsequently subdivided
 
 Usage:
 block = PFBlock(element_ids,  edges, pfevent)
 for uid in block.element_uniqueids:
 print pfevent.get_object(uid).__str__() + "\n"
 
 */
class PFEvent;

//import itertools

class PFBlock {

  
public:
  typedef long longID;
  typedef long long edgeKey;
  typedef std::vector<longID>  IDs;
  typedef std::unordered_map<long long, class Edge> Edges;

  /** Constructor
   @param[in] element_ids:  vector of uniqueids of the elements to go in this block [id1,id2,...]
   @param[inout] edges: is an unordered map of edges, it must contain at least all needed edges. It is not a problem
   if it contains additional edges as only the ones needed will be extracted. Note that edges that are extracted will be
   removed from the Edges object.
   @param[in] pfevent: allows access to the underlying elements given a uniqueid
   must provide a get_object function
   */
  PFBlock(const IDs&  elementIDs, Edges& edges);
  PFBlock();
  const IDs elementIDs() const { return m_elementIDs;}
  Edge Edge(long long key) { return m_edges.find(key)->second;}
  const class Edge& Edge(long long key) const { return m_edges.find(key)->second;}
  
  /**
  Returns list of all edges of a given edge type that are connected to a given id.
  The list is sorted in order of increasing distance
  @param[in] uniqueid : is the id of item of interest
  @param[in] edgetype : is an optional type of edge. If specified only links of the given edgetype will be returned
  @return vector of long long keys to linked edges
 */
  std::vector<long long> linkedEdgeKeys(longID uniqueid, Edge::EdgeType matchtype = Edge::EdgeType::kUnknown) const;
  
  /**
  Returns list of all linked ids of a given edge type that are connected to a given id 
   @param[in] uniqueid : is the id of item of interest
   @param[in] edgetype : is an optional type of edge. If specified only links of the given edgetype will be returned
   @return vector of longIDs that are linked to the uniqueid
  */
   IDs linkedIDs(longID uniqueID, Edge::EdgeType edgetype = Edge::EdgeType::kUnknown) const;
  

  std::string shortName() const; ///< Short descriptor of block such as E3H1T2 (three Ecals, 1 Hcal, 2 tracks)
  int  countEcal() const; ///< Counts how many ecal cluster ids are in the block
  int  countHcal() const ; ///< Counts how many hcal cluster ids are in the block
  int  countTracks() const; ///< Counts how many tracks are in the block
  int  size() const ; ///< length of the element_unqiueids
  longID uniqueID() const {return m_uniqueID;}; ///<Unique ID of the block
  bool isActive() const {return m_isActive;}; ///Blocks that have been split will be deactivated
  void setActive(bool active) {m_isActive=active;}; ///active/ deactivate block
  Edges& edges() {return m_edges;}
  
  friend std::ostream& operator<<(std::ostream& os, const PFBlock& block); ///< print block
private:;
  std::string elementsString() const;
  std::string edgeMatrixString() const;
  
  longID m_uniqueID; //  make a uniqueid for this block
  bool m_isActive; // if a block is subsequently split it will be deactivated
  static int tempBlockCount; //sequential numbering of blocks, not essential but helpful for debugging
  int m_blockCount ; //sequential numbering of blocks, not essential but helpful for debugging
  IDs  m_elementIDs; //elements in this block ordered by type and decreasing energy
  Edges m_edges; //all the edges for elements in this block
  const class Edge& edge(longID id1, longID id2) const;
  
  
} ;
#endif /* PFBlock_h */
