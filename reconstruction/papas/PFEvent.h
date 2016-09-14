#ifndef RECONSTRUCTION_PFEVENT_H
#define RECONSTRUCTION_PFEVENT_H

#include "Cluster.h"
#include "DefinitionsCollections.h"
#include "DefinitionsNodes.h"
#include "PFBlock.h"
#include "SimParticle.h"
#include "pTrack.h"

namespace papas {

class Simulator;
class PFBlockBuilder;
/**
 *
 *  @brief A PFEvent keeps together ecal clusters, hcal cluster, tracks and history for an event
 *
 *  Example usage: TODO;
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */
class PFEvent {
public:
  PFEvent(const Clusters& ecals, const Clusters& hcals, const Tracks& tracks, Nodes& historyNodes);
  PFEvent(Simulator& sim);
  // bool compare(IdType id1, IdType id2) const; ///< test if these ids are same type
  double energy(IdType id1) const;
  const Track& track(IdType id) const;      ///< find a track from an id
  const Cluster& cluster(IdType id) const;  ///< find a cluster from an id
  const class Cluster& ECALCluster(IdType id) const;
  const class Cluster& HCALCluster(IdType id) const;
  Ids elementIds() const;                           ///< all element ids in this event (ecals, hcals, tracks)
  Ids mergedElementIds() const;                     ///< all merged element ids (merged ecal, merged hcals, tracks)
  Nodes& historyNodes() { return m_historyNodes; }  // allow these to be changed
  const Clusters& ecalClusters() const { return m_ecals; }
  const Clusters& hcalClusters() const { return m_hcals; }
  const Tracks& tracks() const { return m_tracks; }
  // const Blocks& blocks() const { return m_blocks; }
  const Particles& reconstructedParticles() const { return m_reconstructedParticles; }

  // void setBlocks(Blocks&& blocks) { m_blocks = std::move(blocks); }
  // void setBlocks(PFBlockBuilder& builder);  // temp for python

  void setReconstructedParticles(Particles&& particles) { m_reconstructedParticles = std::move(particles); }

  void mergeClusters();  ///< call cluster merging algorithms
  void clear();          ///< clear up everything before beginning a new event
  friend std::ostream& operator<<(std::ostream& os, const PFEvent& pfevent);  // TODO move to helper class??

private:
  const Clusters& m_ecals; 
  const Clusters& m_hcals;
  Clusters m_mergedEcals;
  Clusters m_mergedHcals;
  const Tracks& m_tracks;
  Nodes& m_historyNodes;  // should this be owned?
                          // Blocks m_blocks;
  Particles m_reconstructedParticles;
};

}  // end namespace papas

#endif /* PFEvent_h */
