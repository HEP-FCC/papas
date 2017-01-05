#ifndef TestSimulator_h
#define TestSimulator_h


#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include "papas/simulation/HelixPropagator.h"
#include "papas/datatypes/SimParticle.h"
#include "papas/simulation/StraightLinePropagator.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "papas/datatypes/Track.h"

namespace papas {
  class Detector;
  class DetectorElement;
  class SurfaceCylinder;
  
  /** Simulator clas simulates particles and keeps a collection of the Particles and resulting Clusters, Tracks and
   of the relations (history) between them
   The Tracks and Clusters are created and then smeared (and stored separately). Clusters are separated into
   Ecal and Hcal collections.
   Simulator supports simulation of neutral and charged Hadrons and Photons.
   
   Usage:
   Nodes history;
   auto sim = Simulator(CMSDetector, Nodes);
   SimParticle& ptc = sim.addParticle(211, M_PI / 2. + 0.5 , 0, 40. );
   sim.simulateHadron(ptc);
   */
  class TestSimulator {
    
  public:
    /** Constructor
     * @param[in] const Detector& : Detector to be used as basis for simulation
     * @param[nodes] Nodes& : Collection of Nodes which will be used to store history. It may be empty.
     */
    //Simulator(const Detector&, Nodes& nodes);
    TestSimulator(const Particles& particles, const Detector& detector,
              Clusters& ecalClusters,
              Clusters& hcalClusters,
              Clusters& smearedEcalClusters,
              Clusters& smearedHcalClusters,
              Tracks& tracks,
              Tracks& smearedtracks,
              SimParticles& simParticles,
              Nodes& history);

    void simulateParticle(const Particle& ptc);
    const Cluster& cluster(IdType clusterId) const;                  ///< retreive a cluster with this unique id
    const Clusters& ecalClusters() const { return m_ecalClusters; }  ///<return Ecal clusters collection
    const Clusters& hcalClusters() const { return m_hcalClusters; }  ///<return Hcal clusters collection
    const Clusters& smearedEcalClusters() const { return m_smearedEcalClusters; }  ///<return smeared Ecal clusters coll.
    const Clusters& smearedHcalClusters() const { return m_smearedHcalClusters; }  ///<return smeared Hcal clusters coll.
    const Tracks& tracks() const { return m_tracks; }                              ///<return tracks collection
    const Tracks& smearedTracks() const { return m_smearedTracks; }                ///<return smeared tracks collection
    Nodes& historyNodes() { return m_history; }                      /// return a reference to history nodes collection
    const SimParticles& particles() const { return m_particles; }  ///< Return particles collection
    Cluster smearCluster(const Cluster& cluster,
                         papas::Layer detectorLayer = papas::Layer::kNone);  ///<randomise cluster energy
    void clear();  ///< Clear all the collections of clusters, particles, tracks
    
    /**
     Makes a new SimParticle using random uniform distribution for theta, phi (-pi to +pi), energy
     @param[in] int pdgid: particle id (eg 22 for a photon)
     @param[in] double charge: charge of particle eg -1
     @param[in] double thetamin: minimum theta (uniform distribution between mintheta maxtheta)
     @param[in] double thetamax: maximum theta
     @param[in] double ptmin: minimum pt (uniform distribution between minpt maxpt)
     @param[in] double ptmax: maximum pt
     @param[in] const TVector3& vertex: start point of particle
     @return SimParticle& the newly created particle
     */
    // move this somewhere else
    SimParticle& addGunParticle(int pdgid, double charge, double thetamin, double thetamax, double ptmin, double ptmax,
                                const TVector3& vertex = TVector3(0., 0., 0.));  // TODO move elsewhere
    
  private:
    void simulatePhoton(SimParticle& ptc);    ///< Simulates cluster from Photon
    void simulateHadron(SimParticle& ptc);    ///< Simulates clusters and track from a Hadron
    void simulateNeutrino(SimParticle& ptc);  ///< Simulates neutrino
    void smearElectron(SimParticle& ptc);     ///< Does not smear so far as I can see
    void simulateElectron(SimParticle& ptc);
    void smearMuon(SimParticle& ptc);         ///< Does not smear so far as I can see
    void simulateMuon(SimParticle& ptc);
    /**
     Makes a new SimParticle
     @param[in] int pdgid: particle id (eg 22 for a photon)
     @param[in] const TLorentzVector& tlv: particle momentum
     @param[in] const TVector3& vertex: start point of particle
     @return SimParticle& the newly created particle
     */
    SimParticle makeSimParticle(int pdgid, double charge, const TLorentzVector& tlv,
                                const TVector3& vertex = TVector3(0., 0., 0.)) const;
    
    /**
     Makes a new SimParticle
     @param[in] int pdgid: particle id (eg 22 for a photon)
     @param[in] double charge: charge of particle eg -1
     @param[in] double theta: initial direction of particle
     @param[in] double phi: initial direction of particle
     @param[in] double energy: energy of particle
     @param[in] const TVector3& vertex: start point of particle
     @return SimParticle& the newly created particle
     */
    SimParticle makeSimParticle(int pdgid, double charge, double theta, double phi, double energy,
                                const TVector3& vertex = TVector3(0., 0., 0.)) const;
    SimParticle& storeSimParticle(SimParticle&& simParticle, IdType parentId);
    Cluster makeCluster(const SimParticle& ptc, papas::Layer layer, double fraction = 1., double csize = -1., char subtype='t') const;
    const Cluster& storeEcalCluster(Cluster&& cluster, IdType parentId);  ///<Store and add to history
    const Cluster& storeHcalCluster(Cluster&& cluster, IdType parentId);  ///<Store and add to history
    bool acceptSmearedCluster(const Cluster& smearedCluster, papas::Layer detectorLayer = papas::Layer::kNone,
                              papas::Layer acceptLayer = papas::Layer::kNone, bool accept = false) const;
    const Cluster& storeSmearedCluster(Cluster&& smearedCluster, IdType parentId);
    const Track& storeTrack(Track&& track, IdType parentId);  ///<move track into tracks collection and history
    Track smearTrack(const Track& track, double resolution) const;               ///< randomisation of the energy of a track
    bool acceptSmearedTrack(const Track& smearedtrack, bool accept = false) const;  ///< check if track is detected
    bool acceptElectronSmearedTrack(const Track& smearedTrack, bool accept) const;
    const Track& storeSmearedTrack(Track&& smearedtrack,
                                   IdType parentid);  ///<move into the smearedtracks collection and history
    
    void propagate(const SurfaceCylinder& cylinder, SimParticle& ptc);  ///< find where particle hits cylinder
    void propagateAllLayers(SimParticle& ptc);                          ///< find where particle hits detector cylinders
    void addNode(const IdType newid, const IdType parentid = 0);        ///<update history nodes
    std::shared_ptr<const DetectorElement> elem(papas::Layer layer) const;
    
    void testing();                                        // temp
    Ids linkedEcalSmearedClusterIds(IdType nodeid) const;  // TODO move to helper/history class?
    Ids linkedParticleIds(IdType nodeid) const;            // TODO move to helper/history class?
    Ids parentParticleIds(IdType nodeid) const;            // TODO move to helper/history class?
    Ids linkedRawTrackIds(IdType nodeid) const;            // TODO move to helper/history class?
    Ids linkedSmearedTrackIds(IdType nodeid) const;        // TODO move to helper/history class?
    Ids linkedIds(IdType nodeid) const;                    // TODO move to helper/history class?
    const Detector& m_detector;
    Clusters& m_ecalClusters;         ///< ecal clusters (prior to smearing)
    Clusters& m_hcalClusters;         ///< hcal clusters (prior to smearing)
    Clusters& m_smearedEcalClusters;  ///< smeared ecal clusters
    Clusters& m_smearedHcalClusters;  ///< smeared hcal clusters
    Tracks& m_tracks;                 ///< tracks
    Tracks& m_smearedTracks;          ///< smeared tracks
    SimParticles& m_particles;        ///< all particles
    
    Nodes& m_history;  ///< Records relationships of everything that is simulated
    
    StraightLinePropagator m_propStraight;  ///<used to determine the path of uncharged particles
    HelixPropagator m_propHelix;            ///<used to determine the path of charged particles
  };
  
}  // end namespace papas

#endif /* TestSimulator_h */
