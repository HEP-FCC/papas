//
//  PFReconstructor.hpp
//  fastsim
//
//  Created by Alice Robson on 20/04/16.
//
//

#ifndef PFReconstructor_h
#define PFReconstructor_h

#include "DefinitionsCollections.h"
#include "DefinitionsNodes.h"
#include "PFEvent.h"
#include "TVector3.h"

namespace papas {

class PFReconstructor {

public:
  PFReconstructor(PFEvent& pfEvent);
  void reconstruct(Blocks& blocks);
  Particles& particles() { return m_particles; }

private:
  Blocks simplifyBlock(PFBlock& block);
  void reconstructBlock(const PFBlock& block);
  void reconstructHcal(const PFBlock& block, Id::Type hcalId);
  SimParticle reconstructTrack(const Track& track);
  SimParticle reconstructCluster(const Cluster& cluster, papas::Layer layer, double energy = -1,
                                 TVector3 vertex = TVector3());
  void insertParticle(const PFBlock& block, SimParticle&& particle);
  double neutralHadronEnergyResolution(const Cluster& hcal) const;
  double nsigmaHcal(const Cluster& cluster) const;

  PFEvent& m_pfEvent;
  Nodes& m_historyNodes;
  //Blocks m_blocks;  // TODO should this be a reference? - nb blocks will get changed (disactivated during splitting)
  Particles m_particles;  // owns the particles it makes
  bool m_hasHistory;
  Ids m_unused;
  std::unordered_map<Id::Type, bool> m_locked;
};
}  // end namespace papas
#endif /* PFReconstructor_h */
