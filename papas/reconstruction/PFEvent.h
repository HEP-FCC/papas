#ifndef RECONSTRUCTION_PFEVENT_H
#define RECONSTRUCTION_PFEVENT_H

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/reconstruction/PFBlock.h"
#include "papas/datatypes/SimParticle.h"
#include "papas/datatypes/Track.h"

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
  PFEvent(const Clusters& ecals, const Clusters& hcals, const Tracks& tracks,
          Nodes& historyNodes);  // history gets added to
  PFEvent(Simulator& sim);       // improve this (history inside simulator gets updated)
  double energy(IdType id1) const;
  const Track& track(IdType id) const;      ///< find a track from an id
  const Cluster& cluster(IdType id) const;  ///< find a cluster from an id
  const Cluster& ECALCluster(IdType id) const;
  const Cluster& HCALCluster(IdType id) const;
  Ids elementIds() const;                           ///< all element ids in this event (ecals, hcals, tracks)
  Ids mergedElementIds() const;                     ///< all merged element ids (merged ecal, merged hcals, tracks)
  Nodes& historyNodes() { return m_historyNodes; }  // allow these to be changed
  const Clusters& ecalClusters() const { return m_ecals; }
  const Clusters& hcalClusters() const { return m_hcals; }
  const Tracks& tracks() const { return m_tracks; }
  const Blocks& blocks() const { return m_blocks; }
  const Particles& reconstructedParticles() const { return m_reconstructedParticles; }
  void setMergedEcals(Clusters&& ecals) { m_mergedEcals = std::move(ecals); }
  void setMergedHcals(Clusters&& hcals) { m_mergedHcals = std::move(hcals); }
  void setReconstructedParticles(Particles&& particles) { m_reconstructedParticles = std::move(particles); }
  void setBlocks(Blocks&& blocks) { m_blocks = std::move(blocks); }
  void clear();  ///< clear up everything before beginning a new event
  friend std::ostream& operator<<(std::ostream& os, const PFEvent& pfevent);  // TODO move to helper class??

private:
  const Clusters& m_ecals;
  const Clusters& m_hcals;
  const Tracks& m_tracks;
  Nodes& m_historyNodes;  // history gets updated
  Clusters m_mergedEcals;
  Clusters m_mergedHcals;
  Blocks m_blocks;
  Particles m_reconstructedParticles;
};

}  // end namespace papas

#endif /* PFEvent_h */
