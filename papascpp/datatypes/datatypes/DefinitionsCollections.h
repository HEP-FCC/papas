//
//  DefinitionsCollections.h
//  papas
//
//  Created by Alice Robson on 26/05/16.
//
//

#ifndef DefinitionsCollections_h
#define DefinitionsCollections_h

#include <unordered_map>
#include <vector>
#include "Id.h"
namespace papas {
class Cluster;
class PFBlock;
class Track;
class Edge;
class PFParticle;
typedef std::unordered_map<Id::Type, PFParticle> Particles;
typedef std::unordered_map<unsigned long long, class Edge> Edges;
typedef std::vector<Id::Type> Ids;
typedef std::unordered_map<Id::Type, Track> Tracks;
typedef std::unordered_map<Id::Type, PFBlock> Blocks;
typedef std::unordered_map<Id::Type, Cluster> Clusters;
}
#endif /* DefinitionsCollections_h */
