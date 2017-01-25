//
//  DefinitionsCollections.h
//  papas
//
//  Created by Alice Robson on 26/05/16.
//
//

#ifndef DefinitionsCollections_h
#define DefinitionsCollections_h

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Identifier.h"
#include "papas/datatypes/PFParticle.h"
#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Track.h"
// include "papas/reconstruction/PFBlock.h"
#include "papas/graphtools/Edge.h"
//#include <map>
#include "papas/datatypes/DefinitionsCollections.h"
#include <list>
#include <string>
#include <unordered_map>

namespace papas {
class Cluster;
class PFBlock;
class Track;
class Edge;
class PFParticle;
class Particle;

typedef std::unordered_map<IdType, PFParticle> PFParticles;  ///< collection of Simulated Particles
typedef std::list<Particle> ListParticles;                   ///< collection of Particles
typedef std::unordered_map<IdType, Particle> Particles;      ///< collection of Particles
typedef std::unordered_map<unsigned long long, Edge> Edges;  ///< collection of Edges
typedef std::list<IdType> Ids;                               ///< collection of Ids
typedef std::unordered_map<IdType, Track> Tracks;            ///< collection of Tracks
typedef std::unordered_map<IdType, PFBlock> Blocks;          ///< collection of Blocks
typedef std::unordered_map<IdType, Cluster> Clusters;        ///< collection of Clusters

typedef std::unordered_map<Identifier::SubType, const PFParticles*> CollectionParticles;  ///< collection of Particles
typedef std::unordered_map<Identifier::SubType, const Tracks*> CollectionTracks;          ///< collection of Tracks
typedef std::unordered_map<Identifier::SubType, const Clusters*> CollectionClusters;      ///< collection of Clusters
typedef std::unordered_map<Identifier::SubType, const Blocks*> CollectionBlocks;          ///< collection of Clusters
}
#endif /* DefinitionsCollections_h */
