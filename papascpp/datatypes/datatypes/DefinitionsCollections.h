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
#include <map>
#include <vector>
#include "Id.h"
namespace papas {
class Cluster;
class PFBlock;
class Track;
class Edge;
class SimParticle;
class Particle;

typedef std::unordered_map<Id::Type, SimParticle> SimParticles;
typedef std::unordered_map<Id::Type, Particle> Particles;
typedef std::map<unsigned long long, class Edge> Edges; //changed from unordered_map to map for python comparison
typedef std::vector<Id::Type> Ids;
typedef std::unordered_map<Id::Type, Track> Tracks;
typedef std::map<Id::Type, PFBlock> Blocks; //changed from unordered_map to map for python comparison
typedef std::unordered_map<Id::Type, Cluster> Clusters;
  
  
extern Particles emptyParticles; //TODO check
}
#endif /* DefinitionsCollections_h */
