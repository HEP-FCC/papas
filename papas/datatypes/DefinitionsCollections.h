#ifndef DefinitionsCollections_h
#define DefinitionsCollections_h

#include "papas/utility/PapasPreProcessor.h"
#include "papas/datatypes/IdCoder.h"


#include <list>
#include <unordered_map>
#if WITHSORT or PAPASWITHSORT
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

typedef std::list<Particle> ListParticles;         ///< list of Particles
typedef std::unordered_map<uint64_t, Edge> Edges;  ///< collection of Edge objects
#if PAPASWITHSORT
typedef std::set<Identifier, std::greater<Identifier>> Ids;  ///< set containing Identifiers
#else
typedef std::unordered_set<Identifier> Ids;  ///< set containing Identifiers
#endif
typedef std::unordered_map<Identifier, Track> Tracks;        ///< collection containing Track objects
typedef std::unordered_map<Identifier, PFBlock> Blocks;      ///< collection containing Block objects
typedef std::unordered_map<Identifier, Cluster> Clusters;    ///< collection containing Cluster objects
typedef std::unordered_map<Identifier, Particle> Particles;  ///< collection containing Particle objects

/// Folder is an unordered map in which we store pointers to collections that in turn contain Particle objects
typedef std::unordered_map<IdCoder::SubType, const Particles*> ParticlesFolder;

/// Folder is an unordered map in which we store pointers to collections that in turn contain Track objects
typedef std::unordered_map<IdCoder::SubType, const Tracks*> TracksFolder;

///< Folder is an unordered map in which we store pointers to collections that in turn contain Cluster objects
typedef std::unordered_map<IdCoder::SubType, const Clusters*> ClustersFolder;

///< Folder is an unordered map in which we store pointers to collections that in turn contain PFBlock objects
typedef std::unordered_map<IdCoder::SubType, const Blocks*> BlocksFolder;
}
#endif /* DefinitionsCollections_h */
