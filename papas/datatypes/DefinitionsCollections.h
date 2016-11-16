//
//  DefinitionsCollections.h
//  papas
//
//  Created by Alice Robson on 26/05/16.
//
//

#ifndef DefinitionsCollections_h
#define DefinitionsCollections_h

#include "papas/datatypes/Identifier.h"
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

typedef std::unordered_map<IdType, SimParticle> SimParticles;  ///< collection of Simulated Particles
typedef std::unordered_map<IdType, Particle> Particles;        ///< collection of Particles
typedef std::unordered_map<unsigned long long, Edge> Edges;      ///< collection of Edges
typedef std::vector<IdType> Ids;                               ///< collection of Ids
typedef std::unordered_map<IdType, Track> Tracks;              ///< collection of Tracks
typedef std::unordered_map<IdType, PFBlock> Blocks;            ///< collection of Blocks
typedef std::unordered_map<IdType, Cluster> Clusters;          ///< collection of Clusters

/*typedef std::map<IdType, SimParticle> SimParticles;    ///< collection of Simulated Particles
typedef std::map<IdType, Particle> Particles;          ///< collection of Particles
typedef std::map<unsigned long long, class Edge> Edges;  ///< collection of Edges
typedef std::vector<IdType> Ids;                                 ///< collection of Ids
typedef std::map<IdType, Track> Tracks;                ///< collection of Tracks
typedef std::map<IdType, PFBlock> Blocks;              ///< collection of Blocks
typedef std::map<IdType, Cluster> Clusters;            ///< collection of Clusters*/
}
#endif /* DefinitionsCollections_h */
