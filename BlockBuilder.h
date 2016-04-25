#ifndef RECONSTRUCTION_BLOCKBUILDER_H
#define RECONSTRUCTION_BLOCKBUILDER_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "directedacyclicgraph.h"
#include "GraphBuilder.h"

class PFEvent;
class PFBlock;
class Edge;
class GraphBuilder;


/** @class   rec::BlockBuilder Reconstruction/Reconstruction/BlockBuilder.h BlockBuilder.h
 *
 * BlockBuilder takes a vector of identifiers and an unordered map of associated edges which have distance and link info
 * It uses the distances/links between elements to construct a set of connected blocks
 * Each element will end up in one (and only one) block
 * Blocks retain information of the elements and the distances between elements
 * The blocks can be used for future particle reconstruction
 * The ids must be unique and are expected to come from the Identifier class
 
 
 Usage example:
 
 BlockBuilder builder {ids, edges, history_nodes, pfevent};
 for (b in builder.blocks()) {
 ...
 }
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */

//TODO namespace
class PFEvent;

//TODO figure out best place to keep the typedefs
typedef long longID;
typedef std::unordered_map<long long, class Edge> Edges;
typedef DAG::Node<longID> PFNode;
typedef std::unordered_map<longID,PFNode> Nodes;
typedef std::unordered_map<longID, PFBlock> Blocks;
typedef std::vector<longID> IDs;

//Allow optional parameters where construction arguments are references
//TODO make these internal static members
extern Nodes emptyNodes;
extern const Nodes emptyconstNodes;

class BlockBuilder: public GraphBuilder {
public:
  /** Constructor
   
   * @param[in] ids : vector of unique identifiers eg of tracks, clusters etc
   * @param[in] edges : unordered_map of edges which contains all edges between the ids (and maybe more)
   *            an edge records the distance and links between two ids
   * @param[inout] historyNodes : optional unordered_map that describes which elements are parents of which blocks
   *                     if a history_nodes tree is provided then
   *                     the new history will be added into the exisiting history
   */
  BlockBuilder(IDs ids,
               Edges& edges,
               Nodes& historynodes = emptyNodes );
  Blocks& blocks() {return m_blocks;}; ///<return the unordered map of the resulting blocks;
                                       //TODO should this be move
  friend std::ostream& operator<<(std::ostream& os, const BlockBuilder& blockbuilder); //TODO move to helper class
  
private:
  void makeBlocks(); // does the main work
  Nodes& m_historyNodes; ///<optional, allows history to be updated
  Blocks m_blocks;///< the blocks made by blockbuilder
  
  //bool compareEdges( long long key1, long long key2, longID uniqueid) const; //todo move to helper class
  //void sortIDs(IDs& ids); //sorts elements by type
  
};


#endif /* BlockBuilder_h */
