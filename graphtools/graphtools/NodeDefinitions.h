//
//  NodeDefinitions.h
//  papas
//
//  Created by Alice Robson on 12/05/16.
//
//

#ifndef NodeDefinitions_h
#define NodeDefinitions_h

#include <unordered_map>
#include <vector>
#include "directedacyclicgraph.h"
#include "Id.h"
namespace papas {

typedef DAG::Node<Id::Type> PFNode;
typedef std::unordered_map<Id::Type, PFNode> Nodes;
extern Nodes emptyNodes;
extern const Nodes emptyconstNodes;

//put these into individual classes
class Cluster;
class PFBlock;
class Track;
class Edge;
class PFParticle;
typedef std::unordered_map<Id::Type, PFParticle> Particles;
typedef std::unordered_map<long long, class Edge> Edges;
typedef std::vector<Id::Type> Ids;
typedef std::unordered_map<Id::Type, Track> Tracks;
typedef std::unordered_map<Id::Type, PFBlock> Blocks;
typedef std::unordered_map<Id::Type, Cluster> Clusters;

  } // end namespace papas

#endif /* NodeDefinitions_h */
