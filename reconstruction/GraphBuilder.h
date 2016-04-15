#ifndef RECONSTRUCTION_GRAPHBUILDER_H
#define RECONSTRUCTION_GRAPHBUILDER_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "directedacyclicgraph.h"

class PFEvent;
class PFBlock;
class Edge;


/** @class   rec::GraphBuilder Reconstruction/Reconstruction/GraphBuilder.h GraphBuilder.h
 *
 * GraphBuilder takes a vector of identifiers and an unordered map of associated edges which have distance and link info
 * It uses the distances/links between elements to construct a set of connected blocks
 * Each element will end up in one (and only one) block
 * Blocks retain information of the elements and the distances between elements
 * The blocks can be used for future particle reconstruction
 * The ids must be unique and are expected to come from the Identifier class
 
 
 Usage example:
 
 GraphBuilder builder {ids, edges, history_nodes, pfevent};
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
extern Nodes emptyNodes;
extern const Nodes emptyconstNodes;

class GraphBuilder {
public:
  /** Constructor
   
   * @param[in] ids : vector of unique identifiers eg of tracks, clusters etc
   * @param[in] edges : unordered_map of edges which contains all edges between the ids (and maybe more)
   *            an edge records the distance and links between two ids
   * @param[inout] historyNodes : optional unordered_map that describes which elements are parents of which blocks
   *                     if a history_nodes tree is provided then
   *                     the new history will be added into the exisiting history
   */
  GraphBuilder(IDs ids,
               Edges& edges);
  GraphBuilder();
  //GraphBuilder& operator = (const GraphBuilder&) ;
  GraphBuilder& operator = (const GraphBuilder&) ;
  const std::vector<IDs>  subGraphs() const {return m_subGraphs;}; ///<return the unordered map of the resulting subgraphs;
  std::vector<IDs>  m_subGraphs;///< vector of subgraphs made by graphbuilder
  
protected:
   void sortIDs(IDs& ids);
   Edges& m_edges;
private:
  //void makeGraphs(); // does the main work
   //sorts elements by type
  
  //bool compareEdges( long long key1, long long key2, longID uniqueid) const; //todo move to helper class
  
  IDs m_elementIDs; ///<uniqueids to be grouped into subgraphs
   ///< all the edges corresponding to the ids
  Nodes m_localNodes; ///<local nodes used in building subgraphs
  
};


#endif /* GraphBuilder_h */
