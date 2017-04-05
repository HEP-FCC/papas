//
//  DefinitionsCollections.h
//
//  Created by Alice Robson on 26/05/16.
//
//

#ifndef DefinitionsCollections_h
#define DefinitionsCollections_h

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/IdCoder.h"
#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Track.h"
#include "papas/graphtools/Edge.h"
#include <list>
#include <unordered_map>
#if WITHSORT
#include <set>
#else
#include <unordered_set>
#endif

namespace papas {
class Cluster;
class PFBlock;
class Track;
class Edge;
class Particle;

typedef std::unordered_map<Identifier, Particle> Particles;  ///< collection of Particles
typedef std::list<Particle> ListParticles;                   ///< collection of Particles
typedef std::unordered_map<unsigned long long, Edge> Edges;  ///< collection of Edges
#if WITHSORT
typedef std::set<Identifier, std::greater<Identifier>> Ids;  ///< collection of Ids
#else
typedef std::unordered_set<Identifier> Ids;
#endif
typedef std::unordered_map<Identifier, Track> Tracks;      ///< collection of Tracks
typedef std::unordered_map<Identifier, PFBlock> Blocks;    ///< collection of Blocks
typedef std::unordered_map<Identifier, Cluster> Clusters;  ///< collection of Clusters

typedef std::unordered_map<IdCoder::SubType, const Particles*> CollectionParticles;  ///< collection of Particles
typedef std::unordered_map<IdCoder::SubType, const Tracks*> CollectionTracks;        ///< collection of Tracks
typedef std::unordered_map<IdCoder::SubType, const Clusters*> CollectionClusters;    ///< collection of Clusters
typedef std::unordered_map<IdCoder::SubType, const Blocks*> CollectionBlocks;        ///< collection of Clusters
}
#endif /* DefinitionsCollections_h */
