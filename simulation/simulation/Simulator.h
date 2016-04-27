//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  SIMULATOR_H
#define  SIMULATOR_H
#include <string>
#include <unordered_map>
#include <map>
#include "enums.h"
#include "detector.h"
#include "datatypes.h"
#include "directedacyclicgraph.h"
#include "Identifier.h"
#include "propagator.h"

class Particle;

//TODO home for typedefs
typedef DAG::Node<long> Node;
typedef std::unordered_map<long, Node> Nodes;
typedef std::unordered_map<long, Cluster> Clusters;
typedef std::unordered_map<long, Cluster> SmearedECALClusters;
typedef std::unordered_map<long, Cluster> SmearedHCALClusters;
typedef std::unordered_map<long, Cluster> ECALClusters;
typedef std::unordered_map<long, Cluster> HCALClusters;
typedef std::unordered_map<long, Track> Tracks;
typedef std::unordered_map<long, PFParticle> Particles;
typedef std::vector<long> IDs;

class Simulator {

public:
  Simulator(const BaseDetector&);  //AJR TODO add logger
  void simulate();
  void simulatePhoton(PFParticle& ptc);
  void simulateHadron(PFParticle& ptc);

  PFParticle& addParticle(int pdgid, double theta, double phi, double energy, TVector3 vertex = TVector3(0., 0., 0.));

  const Cluster& cluster(long clusterID) const;
  Clusters ECALClusters() const {return m_ECALClusters;} ///<Copy of ECAL clusters
  Clusters HCALClusters() const {return m_HCALClusters;} ///<Copy of HCAL clusters
  Clusters smearedECALClusters() const {return m_smearedECALClusters;} ///<Copy of smeared ECAL clusters
  Clusters smearedHCALClusters() const {return m_smearedHCALClusters;} ///<Copy of smeared HCAL clusters

  Tracks   tracks() const {return m_tracks;} ///<Copy of tracks
  Tracks   smearedTracks() const {return m_smearedTracks;}  ///<Copy of smeared tracks
  Nodes&   historyNodes() {return m_nodes;} ///reference to history //TODO decide if a copy should be made
  const Particles& particles() const {return m_particles;} ///<Copy of particles

  void testing();
  IDs linkedECALSmearedClusterIDs(long nodeid) const; //TODO move to helper/history class
  IDs linkedParticleIDs(long nodeid) const ; //TODO move to helper/history class
  IDs parentParticleIDs(long nodeid) const ; //TODO move to helper/history class

private:
  PFParticle& addParticle(int pdgid, TLorentzVector tlv, TVector3 vertex = TVector3(0., 0., 0.));
  void propagate(PFParticle& ptc, const SurfaceCylinder&); //more args needed
  long makeClusterID(fastsim::enumLayer layer, fastsim::enumSubtype subtype) const;
  long makeParticleID(fastsim::enumSource source) const;
  long addECALCluster(PFParticle& ptc, long parentid = 0, double fraction = 1., double csize = 0.);
  long addHCALCluster(PFParticle& ptc, long parentid = 0, double fraction = 1., double csize = 0.);
  long addSmearedCluster(long parentClusterID);
  Cluster makeCluster(PFParticle& ptc, long parentid, fastsim::enumLayer layer, double fraction = 1., double csize = 0.);
  Cluster makeSmearedCluster(long parentClusterID);
  const Track& addTrack(PFParticle& ptc);
  long addSmearedTrack(const Track& track, bool accept = false);
  void addNode(const long newid, const long parentid = 0);
  std::shared_ptr<const DetectorElement> elem(fastsim::enumLayer layer) const;
  
  IDs linkedRawTrackIDs(long nodeid) const; //TODO move to helper/history class
  IDs linkedSmearedTrackIDs(long nodeid) const; //TODO move to helper/history class
  IDs linkedIDs(long nodeid) const; //TODO move to helper/history class
  IDs getMatchingIDs(long nodeid, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                     fastsim::enumSubtype type, fastsim::enumSource source) const; //TODO move to helper/history class
  IDs getMatchingParentIDs(long nodeid, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                           fastsim::enumSubtype type, fastsim::enumSource source) const ;  //TODO move to helper/history class

  Clusters m_ECALClusters;
  Clusters m_HCALClusters;
  Clusters m_smearedECALClusters;
  Clusters m_smearedHCALClusters;
  Tracks   m_tracks;        /// pre smeared tracks
  Tracks   m_smearedTracks; /// smeared tracks
  Particles m_particles;    /// all particles

  //this will have a keyed entry for everything that has
  //been simulated and so acts as a lookup table
  Nodes m_nodes;
  const BaseDetector& m_detector;
  StraightLinePropagator m_propStraight;
  HelixPropagator m_propHelix;
};

#endif

