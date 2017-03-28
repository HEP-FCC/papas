#ifndef PFReconstructor_h
#define PFReconstructor_h

#include "TVector3.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Event.h"
#include "papas/detectors/Detector.h"
#include "papas/graphtools/DefinitionsNodes.h"

namespace papas {
  
  class StraightLinePropagator;
  class HelixPropagator;
  class Propagator;
class PFReconstructor {
  /** Handles reconstruction of particles from a Event
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
   - if excess = hcal energy + ecal energies - track energies > 0 and excess < ecal energies then turn the excess into an
   photon
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
   PFBlockBuilder bBuilder(m_pfEvent, ids);
   
   // do the reconstruction of the blocks
   PFReconstructor pfReconstructor(m_pfEvent);
   pfReconstructor.reconstruct(bBuilder.blocks());
   m_pfEvent.setReconstructedParticles(std::move(pfReconstructor.particles()));
   @endcode
   */
public:
  /** Constructor
   event must contain PFBlocks of linked tracks and clusters of type blockSubtype
   blockSubtype single character describing which blocks to use eg 's' for split blocks
   detector the detector being used
   particles a collection (owned elsewhere) into which the reconstructed particles will be added
   history a colelction of Nodes (owned elsewhere) into which history information will be added
  */
  PFReconstructor(const Event& event, char blockSubtype, const Detector& detector, Particles& particles,
                  Nodes& history);
  
  
  //const Particles& particles() const { return m_particles; }  //
private:
  /** Takes a block and reconstructs particles from if
      block the block to be reconstructed
  */
  void reconstructBlock(const PFBlock& block);
  /** Reconstructs particles from na hcal cluster
      @param block the block to which the hcal structure belongs
      @param hcalId the identifier of the Hcal cluster
  */
  void reconstructHcal(const PFBlock& block, Identifier hcalId);
  /** Reconstructs a charged hadron/electron/muon from an Hcal
   @param track the track which is to be reconstructed
   @param pdgId the type of particle to be reconstructed
   @param parentIds ids of parent objects which will be recorded in the history
   */
  void reconstructTrack(const Track& track, int pdgId,
                        const Ids& parentIds);  ///< constructs and returns particle(s) starting from a track
  /** Reconstruct photon (Ecal) of neutralHadron (hcal) from a cluster
  @param cluster cluster that is to be reconstructed into photon or neutral hadron
  @param layer Ecal or Hcal papas::Layer::kEcal or papas::Layer::kHcal
  @param parentIds ids of parent objects which will be recorded in the history
  @param energy Energy that is to be assigned to the particle, if not specified or if negative the cluster energy will be used
  @param vertex This will be the start vertex for the new particle
  */
  void reconstructCluster(
      const Cluster& cluster, papas::Layer layer, const Ids& parentIds, double energy = -1,
      const TVector3& vertex = TVector3());  ///< constructs and returns a particles starting from a cluster
  /** Identify any electrons in the block and reconstruct them
   @param block Block in which to check for and reconstruct electrons
  */
  void reconstructElectrons(const PFBlock& block);
  /** Identify any muons in the block and reconstruct them
   @param block Block in which to check for and reconstruct muons
   */
  void reconstructMuons(const PFBlock& block);
  // void insertParticle(const PFBlock& block, Particle&& particle);  ///< moves particle and adds into history
  /** Add new particle into history
   @param Ids Identifiers of parents of the new particle
   @param newparticle New particle that is to be added into history
   */

  void insertParticle(const Ids& parentIds, Particle& newparticle);
  /**  Checks if object identifier comes, directly or indirectly,
   from a particle of type typeAndSubtype, with this absolute pdgid.
   @param id Identifier of object
   @param typeAndSubtype type and subtype of particle eg 'ps' for simulated particle
   @param pdgid particle type eg muon
   @return True if there is a direct /indirect link between object and particle of this typeAndSubtype and absolute pdgid
  */
  bool isFromParticle(Identifier id, const std::string& typeAndSubtype, int pdgid) const;
  /** detector energy resolution of for a neutral Hadron
   @param energy Energy of deposit (?)
   @param resolution ?
  */
  double neutralHadronEnergyResolution(double energy, double resolution) const;
  /**
   @param cluster
   */
  double nsigmaHcal(const Cluster& cluster) const;
  std::shared_ptr<Propagator> propagator(double charge);
  const Event& m_event; ///< Contains history information and collections of clusters/blocks/tracks
  const Detector& m_detector; ///< Detector
  Particles& m_particles;  ///< the reconstructed particles created by this class
  Nodes& m_history; ///< History collection of Nodes (owned elsewhere) to which new history info will be added
  Ids m_unused; ///< List of ids (of clusters, tracks) which were not used in the particle reconstructions
  std::unordered_map<Identifier, bool> m_locked; ///< map of identifiers which have already been used in reconstruction
  std::shared_ptr<StraightLinePropagator> m_propStraight;  ///<used to determine the path of uncharged particles
  std::shared_ptr<HelixPropagator> m_propHelix;            ///<used to determine the path of charged particles
};
}  // end namespace papas
#endif /* PFReconstructor_h */
