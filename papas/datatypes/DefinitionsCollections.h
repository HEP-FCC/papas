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
#include "papas/datatypes/Cluster.h"
 #include "papas/datatypes/Particle.h"
 #include "papas/datatypes/Track.h"
 #include "papas/datatypes/SimParticle.h"
// #include "papas/reconstruction/PFBlock.h"
 #include "papas/graphtools/Edge.h"
//#include <map>
#include <unordered_map>
#include <vector>
#include <string>
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

  typedef std::string CollectionType;
  //typedef struct CollectionType { unsigned char x[2]; } CollectionType; ///two char descriptor of a collection eg 'em' for merged ecals
typedef std::unordered_map<CollectionType, SimParticles> CollectionParticles;  ///< collection of Particles
typedef std::unordered_map<CollectionType, Tracks> CollectionTracks;              ///< collection of Tracks
typedef std::unordered_map<CollectionType, Blocks> CollectionBlocks;            ///< collection of Blocks
typedef std::unordered_map<CollectionType, Clusters> CollectionClusters;          ///< collection of Clusters

}
#endif /* DefinitionsCollections_h */
