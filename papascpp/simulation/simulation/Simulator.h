//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Cluster.h"
#include "Definitions.h"
#include "DefinitionsCollections.h"
#include "DefinitionsNodes.h"
#include "SimParticle.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "pTrack.h"
#include "propagator.h"

namespace papas {
class Detector;
class DetectorElement;
class SurfaceCylinder;

class Simulator {
  /** Simulator simulates particles and keeps a collection of the Particles and resulting Clusters, Tracks and
   of the relations (history) between them
   The Tracks and Clusters are created and then smeared (and stored separately). Clusters are separated into
   Ecal and Hcal collections.
   Simulator supports simulation of neutral and charged Hadrons and Photons.

   Usage:
   Simulator sim = Simulator{CMSDetector};
   SimParticle& ptc = sim.addParticle(211, M_PI / 2. + 0.5 , 0, 40. );
   sim.simulateHadron(ptc);
   */

public:
  /** Constructor
   * @param[in] const Detector& : Detector to be used as basis for simulation
   */
  Simulator(const Detector&, Nodes& nodes);
  void SimulateParticle(const Particle& ptc, IdType parentid = 0);

  // TODO consider whether the following should be private
  void simulatePhoton(SimParticle& ptc);    ///< Simulates cluster from Photon
  void simulateHadron(SimParticle& ptc);    ///< Simulates clusters and track from a Hadron
  void simulateNeutrino(SimParticle& ptc);  ///< Simulates neutrino
  void smearElectron(SimParticle& ptc);     ///< Does not smear so far as I can see
  void smearMuon(SimParticle& ptc);         ///< Does not smear so far as I can see

  /**
   Makes a new SimParticle and adds this into collection of particles
   @param[in] unsigned int pdgid: particle id (eg 22 for a photon)
   @param[in] TLorentzVector tlv: particle momentum
   @param[in] TVector3 vertex: start point of particle
   @return SimParticle& the newly created particle
   */
  SimParticle& addParticle(unsigned int pdgid, double charge, TLorentzVector tlv,
                           TVector3 vertex = TVector3(0., 0., 0.));

  /**
   Makes a new SimParticle and adds this into collection of particles
   @param[in] unsigned int pdgid: particle id (eg 22 for a photon)
   @param[in] double charge: charge of particle eg -1
   @param[in] double theta: initial direction of particle
   @param[in] double phi: initial direction of particle
   @param[in] double energy: energy of particle
   @param[in] TVector3 vertex: start point of particle
   @return SimParticle& the newly created particle
   */
  SimParticle& addParticle(unsigned int pdgid, double charge, double theta, double phi, double energy,
                           TVector3 vertex = TVector3(0., 0., 0.));

  /**
   Makes a new SimParticle using random uniform distribution for theta, phi (-pi to +pi), energy
   @param[in] unsgined int pdgid: particle id (eg 22 for a photon)
   @param[in] double charge: charge of particle eg -1
   @param[in] double thetamin: minimum theta (uniform distribution between mintheta maxtheta)
   @param[in] double thetamax: maximum theta
   @param[in] double ptmin: minimum pt (uniform distribution between minpt maxpt)
   @param[in] double ptmax: maximum pt
   @param[in] TVector3 vertex: start point of particle
   @return SimParticle& the newly created particle
   */
  SimParticle& addGunParticle(unsigned int pdgid, double charge, double thetamin, double thetamax, double ptmin,
                              double ptmax,
                              TVector3 vertex = TVector3(0., 0., 0.));  // TODO probably should live elsewhere

  const Cluster& cluster(Id::Type clusterId) const;                ///< retreive a cluster with this unique id
  const Clusters& ecalClusters() const { return m_ecalClusters; }  ///<return  Ecal clusters collection
  const Clusters& hcalClusters() const { return m_hcalClusters; }  ///<return Hcal clusters collection
  const Clusters& smearedEcalClusters() const { return m_smearedEcalClusters; }  ///<return smeared Ecal clusters
  const Clusters& smearedHcalClusters() const { return m_smearedHcalClusters; }  ///<return smeared Hcal clusters
  const Tracks& tracks() const { return m_tracks; }                              ///<return tracks  collection
  const Tracks& smearedTracks() const { return m_smearedTracks; }  ///<return a copy of smeared tracks  collection
  Nodes& historyNodes() { return m_nodes; }  /// return a reference to history //TODO decide if a copy should be made
  const SimParticles& particles() const { return m_particles; }  ///< Return copy of  particles collection

  void testing();                                          // temp
  Ids linkedEcalSmearedClusterIds(Id::Type nodeid) const;  // TODO move to helper/history class?
  Ids linkedParticleIds(Id::Type nodeid) const;            // TODO move to helper/history class?
  Ids parentParticleIds(Id::Type nodeid) const;            // TODO move to helper/history class?
  Ids linkedRawTrackIds(Id::Type nodeid) const;            // TODO move to helper/history class?
  Ids linkedSmearedTrackIds(Id::Type nodeid) const;        // TODO move to helper/history class?
  Ids linkedIds(Id::Type nodeid) const;                    // TODO move to helper/history class?
  void clear();

private:
  Id::Type addEcalCluster(SimParticle& ptc, double fraction = 1., double csize = -1);
  Id::Type addHcalCluster(SimParticle& ptc, double fraction = 1., double csize = -1);
  Id::Type addSmearedCluster(Id::Type parentClusterId, papas::Layer detlayer = papas::Layer::kNone,
                             papas::Layer acceptlayer = papas::Layer::kNone, bool accept = false);
  Cluster makeCluster(SimParticle& ptc, papas::Layer layer, double fraction = 1., double csize = -1.);
  Cluster makeSmearedCluster(Id::Type parentClusterId, papas::Layer detlayer = papas::Layer::kNone);
  const Track& addTrack(SimParticle& ptc);
  Id::Type addSmearedTrack(const Track& track, bool accept = false);
  void propagate(SimParticle& ptc, const SurfaceCylinder&);  // more args needed
  void propagateAllLayers(SimParticle& ptc);                 // more args needed

  void addNode(const Id::Type newid, const Id::Type parentid = 0);
  std::shared_ptr<const DetectorElement> elem(papas::Layer layer) const;

  Clusters m_ecalClusters;         ///< ecal clusters (prior to smearing)
  Clusters m_hcalClusters;         ///< hcal clusters (prior to smearing)
  Clusters m_smearedEcalClusters;  ///< smeared ecal clusters
  Clusters m_smearedHcalClusters;  ///< smeared hcal clusters
  Tracks m_tracks;                 ///< tracks
  Tracks m_smearedTracks;          ///< smeared tracks
  SimParticles m_particles;        ///< all particles

  Nodes& m_nodes;  ///< Records relationships of everything that is simulated
  const Detector& m_detector;
  StraightLinePropagator m_propStraight;  ///<used to determine the path of uncharged particles
  HelixPropagator m_propHelix;            ///<used to determine the path of charged particles
};

}  // end namespace papas
#endif