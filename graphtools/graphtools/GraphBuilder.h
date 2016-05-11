#ifndef RECONSTRUCTION_GRAPHBUILDER_H
#define RECONSTRUCTION_GRAPHBUILDER_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "directedacyclicgraph.h"
#include "Id.h"

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
 * The ids must be unique and are expected to come from the Id class


 Usage example:

 GraphBuilder builder {ids, edges, history_nodes, pfevent};
 for (b in builder.blocks()) {
 ...
 }
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */

// TODO namespace
class PFEvent;

// TODO figure out best place to keep the typedefs
typedef std::unordered_map<long long, class Edge> Edges;
typedef DAG::Node<Id::type> PFNode;
typedef std::unordered_map<Id::type, PFNode> Nodes;
typedef std::unordered_map<Id::type, PFBlock> Blocks;
typedef std::vector<Id::type> Ids;

// Allow optional parameters where construction arguments are references
// TODO make sattic class member
extern Nodes emptyNodes;
extern const Nodes emptyconstNodes;

class GraphBuilder {
public:
  /** Constructor

   * @param[in] ids : vector of unique identifiers eg of tracks, clusters etc
   * @param[in] edges : unordered_map of edges which contains all edges between the ids (and maybe more)
   *            an edge records the distance and links between two ids
   */
  GraphBuilder(Ids ids, Edges& edges);
  // GraphBuilder();
  GraphBuilder& operator=(const GraphBuilder&);
  const std::vector<Ids> subGraphs() const {
    return m_subGraphs;
  };                             ///<return the unordered map of the resulting subgraphs;
  std::vector<Ids> m_subGraphs;  ///< vector of subgraphs made by graphbuilder

protected:
  void sortIds(Ids& ids);  ///<sorted by energy
  Edges& m_edges;

private:
  Ids m_elementIds;    ///<uniqueids to be grouped into subgraphs
  Nodes m_localNodes;  ///<local nodes used in building subgraphs
};

#endif /* GraphBuilder_h */