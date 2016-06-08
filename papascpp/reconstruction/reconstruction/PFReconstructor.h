//
//  PFReconstructor.hpp
//  fastsim
//
//  Created by Alice Robson on 20/04/16.
//
//

#ifndef PFReconstructor_h
#define PFReconstructor_h

#include "TVector3.h"
#include "DefinitionsNodes.h"
#include "DefinitionsCollections.h"

namespace papas {
class PFEvent;

class PFReconstructor {
  
public:
  PFReconstructor(PFEvent& pfEvent);
  void reconstruct();
  
private:
  Blocks simplifyBlock(PFBlock& block);
  void reconstructBlock(const PFBlock& block);
  void reconstructHcal(const PFBlock& block, Id::Type hcalId);
  PFParticle reconstructTrack(const Track& track);
  PFParticle reconstructCluster(const Cluster& cluster, papas::Layer layer, double energy = -1,
                                 TVector3 vertex= TVector3());
  void insertParticle(const PFBlock& block, PFParticle&& particle);
  double neutralHadronEnergyResolution(const Cluster& hcal) const;
  double nsigmaHcal(const Cluster& cluster) const;
  
  PFEvent& m_pfEvent;
  Nodes& m_historyNodes;
  Blocks m_blocks; //TODO should this be a reference? - nb blocks will get changed (disactivated during splitting)
  Particles m_particles; //owns the particles it makes
  bool m_hasHistory;
  Ids m_unused;
  std::unordered_map<Id::Type, bool> m_locked;
  
};
} // end namespace papas
#endif /* PFReconstructor_h */
