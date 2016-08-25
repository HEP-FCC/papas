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
/** Handles reconstruction of particles from a PFEvent.
* The PFevent contains the merged clusters and tracks and the history nodes and should already contain
* blocks of connected elements,
* The PFevent may (in theory) be constructed from clusters and tracks read in from file or may be simulated in PAPAS

 The following strategy is used to reconstuct particles from blocks (to be checked with Colin)
 
 ### single elements:
 - track -> charged hadron
 - hcal  -> neutral hadron
 - ecal  -> photon
 
 ### connected elements:
 -# has more than one hcal
  - each hcal is treated using rules below
 
 -# has an hcal with one or more connected tracks
  - add up all connected track energies, turn each track into a charged hadron
  - add up all ecal energies connected to the above tracks
  - if excess = hcal energy + ecal energies - track energies > 0 and excess < ecal energies then turn the excess into an photon
  - if excess > 0 and excess > ecal energies
      - make a neutral hadron with excess- ecal energies
      - make photon with ecal energies
 -# has hcal but no track (nb by design there will be no attached ecals because hcal ecal links have been removed
 so this will equate to single hcal. Two hcals should not occur as a single block
 because if they are close enough to be linked then they should already have been merged)
  - make a neutral hadron
 
 -# has track(s)
  - each track is turned into a charged hadron
 
 -# has track(s) and  ecal(s)
  - the tracks are turned into charged hadrons
  - the ecals are marked as locked but energy is not checked
    and no photons are made
  - TODO handle case where there is more energy in ecals than in the track and make some photons
 -# has only ecals
    - this should not occur because ecals that are close enough to be linked should already have been merged
 
 
 If history_nodes are provided then the particles are linked into the exisiting history
 
 
 Example usage:
 @code
 // get the ids of smeared and merged tracks and clusters that are to be reconstruced
 Ids ids = m_pfEvent.mergedElementIds();
 
 // create the blocks of linked ids
 auto bBuilder = PFBlockBuilder(m_pfEvent, ids);
 
 // do the reconstruction of the blocks
 auto pfReconstructor = PFReconstructor(m_pfEvent);
 pfReconstructor.reconstruct(bBuilder.blocks());
 m_pfEvent.setReconstructedParticles(std::move(pfReconstructor.particles()));
 @endcode
*/
class PFReconstructor {

public:
  //TODO consider if this is the best constructor function interface (could be the clusters tracks and history nodes)
  PFReconstructor(PFEvent& pfEvent);
  void reconstruct(Blocks& blocks);  ///< create reconstructed particles from the evnet
  Particles& particles() { return m_particles; }

private:
  Blocks simplifyBlock(PFBlock& block); ///< checks whether a block can be simplified eg if two hcals are attached to one track
  void reconstructBlock(const PFBlock& block); ///< turns a block into one or more particles
  void reconstructHcal(const PFBlock& block, IdType hcalId); ///< constructs particle(s) starting from Hcal cluster
  SimParticle reconstructTrack(const Track& track); ///< constructs particle(s) starting from a track
  SimParticle reconstructCluster(const Cluster& cluster, papas::Layer layer, double energy = -1,
                                 TVector3 vertex = TVector3()); ///< constructs particles starting from a cluster
  void insertParticle(const PFBlock& block, SimParticle&& particle); ///< adds particle into history
  double neutralHadronEnergyResolution(const Cluster& hcal) const;
  double nsigmaHcal(const Cluster& cluster) const;

  PFEvent& m_pfEvent;
  Nodes& m_historyNodes;
  Particles m_particles;  ///< the reconstructed particles created by this class
  bool m_hasHistory;
  Ids m_unused;
  std::unordered_map<Id::Type, bool> m_locked;
};
}  // end namespace papas
#endif /* PFReconstructor_h */
