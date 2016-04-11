#ifndef RECONSTRUCTION_BLOCKBUILDER_H
#define RECONSTRUCTION_BLOCKBUILDER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "PFEvent.h"
#include "Edge.h"
#include "PFBlock.h"
#include "directedacyclicgraph.h"

/** @class   rec::BlockBuilder Reconstruction/Reconstruction/BlockBuilder.h BlockBuilder.h
 *
 *  @brief An BlockBuilder created blocks of connected elements
 *
 *  Example usage: BlockBuilder b = BlockBuilder...;
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */

//TODO namespace
class PFEvent;

typedef long longID;
typedef std::unordered_map<long long, const class Edge> Edges;
typedef DAG::Node<longID> PFNode;
typedef  std::unordered_map<longID,PFNode> Nodes;
typedef  std::unordered_map<longID, PFBlock> Blocks;
typedef  std::vector<longID> IDs;
const PFEvent emptyconstPFEvent;
Nodes emptyNodes;
const Nodes emptyconstNodes;


class BlockBuilder {
public:
  BlockBuilder(IDs ids,
               Edges& edges,
               Nodes& historynodes = emptyNodes ,
               const PFEvent& pfevent = emptyconstPFEvent );
  
  const Blocks blocks() const {return m_blocks;} ;
  //BlockBuilder(ids, edges);
  friend std::ostream& operator<<(std::ostream& os, const BlockBuilder& blockbuilder); ///< print block

private:
  void makeBlocks();
  void sortIDs(IDs& ids);
  //void sortEgdes
  bool compareEdges( long long key1, long long key2, longID uniqueid) const;
  //bool compare(longID id1, longID id2) const;
 
  IDs m_elementIDs; //
  Edges& m_edges; //< all the edges corresponding to the ids
  Nodes& m_historyNodes; //<optional, allows history to be updated
  const PFEvent& m_pfEvent; //allows access to the underlying objects
  Nodes m_localNodes; //<local nodes used in building blocks
  Blocks m_blocks;//< the blocks made by blockbuilder
  

};






#endif /* BlockBuilder_h */
