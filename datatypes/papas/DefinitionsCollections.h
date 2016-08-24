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
//#include <map>
#include <unordered_map>
#include <vector>
namespace papas {
class Cluster;
class PFBlock;
class Track;
class Edge;
class SimParticle;
class Particle;

typedef std::unordered_map<Id::Type, SimParticle> SimParticles;  ///< collection of Simulated Particles
typedef std::unordered_map<Id::Type, Particle> Particles;          ///< collection of Particles
typedef std::unordered_map<unsigned long long, class Edge> Edges;  ///< collection of Edges
typedef std::vector<Id::Type> Ids; ///< collection of Ids
typedef std::unordered_map<Id::Type, Track> Tracks; ///< collection of Tracks
typedef std::unordered_map<Id::Type, PFBlock> Blocks; ///< collection of Blocks
typedef std::unordered_map<Id::Type, Cluster> Clusters; ///< collection of Clusters
}
#endif /* DefinitionsCollections_h */
