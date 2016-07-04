#ifndef RECONSTRUCTION_PFEVENT_H
#define RECONSTRUCTION_PFEVENT_H

/** @class   rec::PFEvent Reconstruction/Reconstruction/PFEvent.h PFEvent.h
 *
 *  @brief An PFEvent created blocks of connected elements
 *
 *  Example usage: PFEvent b = PFEvent...;
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */

#include "DefinitionsNodes.h"
#include "DefinitionsCollections.h"
#include "PFBlock.h"
#include "SimParticle.h"
#include "pTrack.h"
#include "Cluster.h"

namespace papas {

class Simulator;
class PFBlockBuilder;

class PFEvent {
public:
  PFEvent(const Clusters& ecals,const Clusters& hcals, const Tracks& tracks, Nodes& historyNodes);
  PFEvent(Simulator& sim); 
  bool compare(Id::Type id1, Id::Type id2) const;
  double energy(Id::Type id1) const;
  const Track& track(Id::Type id ) const;
  const Cluster& cluster(Id::Type id) const;
  const class Cluster& ECALCluster(Id::Type id) const;
  const class Cluster& HCALCluster(Id::Type id) const;
  Ids elementIds() const;
  Ids mergedElementIds() const;
  Nodes& historyNodes() { return m_historyNodes;} //allow these to be changed
  const Clusters& ecalClusters() const { return m_ecals;}
  const Clusters& hcalClusters() const { return m_hcals;}
  const Tracks& tracks() const { return m_tracks;}
  const Blocks& blocks() const { return m_blocks;}
  const Particles& reconstructedParticles() const { return m_reconstructedParticles;}
  
  void setBlocks(Blocks&& blocks) {m_blocks = blocks;}
  void setBlocks(PFBlockBuilder& builder);  //temp for python
  void setReconstructedParticles(Particles&& particles) {m_reconstructedParticles = particles;}
  
  void mergeClusters();
  void clear();
  friend std::ostream& operator<<(std::ostream& os, const PFEvent& pfevent); //TODO move to helper class??
  
private:
  
  const Clusters& m_ecals;
  const Clusters& m_hcals;
  Clusters m_mergedEcals;
  Clusters m_mergedHcals;
  const Tracks& m_tracks;
  Nodes& m_historyNodes; //should this be owned?
  Blocks m_blocks;
  Particles m_reconstructedParticles; //should this be owned?
  
};

 
} // end namespace papas

#endif /* PFEvent_h */
