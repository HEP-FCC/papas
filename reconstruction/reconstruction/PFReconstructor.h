//
//  PFReconstructor.hpp
//  fastsim
//
//  Created by Alice Robson on 20/04/16.
//
//

#ifndef PFReconstructor_h
#define PFReconstructor_h

#include <unordered_map>
#include "TVector3.h"
#include "Id.h"
#include "directedacyclicgraph.h"

class PFEvent;
class PFBlock;
class PFParticle;
class Cluster;
class Track;
class TVector3;

//TODO simparticles is not right
//TODO store typedefs centrally
typedef long longId;
typedef std::unordered_map<longId, PFParticle> Particles;
typedef std::vector<longId> Ids;
//typedef std::unordered_map<longId, Cluster> Clusters;
typedef DAG::Node<longId> PFNode;
typedef std::unordered_map<longId, PFNode> Nodes;
typedef std::unordered_map<longId, PFBlock> Blocks;
//typedef std::unordered_map<longId, Track> Tracks;

class PFReconstructor {
  
public:
  PFReconstructor(PFEvent& pfEvent);// TODO  detector, logger):
  void reconstruct();
  
private:
  Blocks simplifyBlock(PFBlock& block);
  void reconstructBlock(const PFBlock& block);
  void reconstructHcal(const PFBlock& block, longId hcalId);
  PFParticle reconstructTrack(const Track& track);
  PFParticle reconstructCluster(const Cluster& cluster, fastsim::enumLayer layer, double energy = -1,
                                 TVector3 vertex= TVector3());
  void insertParticle(const PFBlock& block, PFParticle&& particle);
  double neutralHadronEnergyResolution(const Cluster& hcal) const;
  double nsigmaHcal(const Cluster& cluster) const;
  
  PFEvent& m_pfEvent;
  Nodes& m_historyNodes;
  Blocks m_blocks; //TODO should this be a reference - nb blocks will get changed (disactivated during splitting)
  Particles m_particles; //owns the particles it makes
  bool m_hasHistory;
  Ids m_unused;
  std::unordered_map<longId, bool> m_locked;
  
};

#endif /* PFReconstructor_h */
