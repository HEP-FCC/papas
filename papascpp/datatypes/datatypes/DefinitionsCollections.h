//
//  DefinitionsCollections.h
//  papas
//
//  Created by Alice Robson on 26/05/16.
//
//

#ifndef DefinitionsCollections_h
#define DefinitionsCollections_h

#include "Id.h"
#include <map>
#include <unordered_map>
#include <vector>
namespace papas {
class Cluster;
class PFBlock;
class Track;
class Edge;
class SimParticle;
class Particle;

typedef std::unordered_map<Id::Type, SimParticle> SimParticles;
typedef std::unordered_map<Id::Type, Particle> Particles;          // temp changed to map for python comparison
typedef std::unordered_map<unsigned long long, class Edge> Edges;  // temp changed  to map for python comparison
typedef std::vector<Id::Type> Ids;
typedef std::unordered_map<Id::Type, Track> Tracks;
typedef std::unordered_map<Id::Type, PFBlock> Blocks;  // change from unordered_map to map for python comparison
typedef std::unordered_map<Id::Type, Cluster> Clusters;
}
#endif /* DefinitionsCollections_h */
