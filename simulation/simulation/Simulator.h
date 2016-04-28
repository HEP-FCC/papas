//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  SIMULATOR_H
#define  SIMULATOR_H
#include <string>
#include <unordered_map>
#include <map>
#include "TVector3.h"
#include "TLorentzVector.h"

#include "enums.h"
#include "directedacyclicgraph.h"
#include "propagator.h"

class PFParticle;
class Cluster;
class Track;
class Detector;
class DetectorElement;
class SurfaceCylinder;


//TODO home for typedefs
typedef DAG::Node<long> Node;
typedef std::unordered_map<long, Node> Nodes;
typedef std::unordered_map<long, Cluster> Clusters;
typedef std::unordered_map<long, Cluster> SmearedEcalClusters;
typedef std::unordered_map<long, Cluster> SmearedHcalClusters;
typedef std::unordered_map<long, Cluster> EcalClusters;
typedef std::unordered_map<long, Cluster> HcalClusters;
typedef std::unordered_map<long, Track> Tracks;
typedef std::unordered_map<long, PFParticle> Particles;
typedef std::vector<long> Ids;

class Simulator {

public:
  Simulator(const Detector&);  //AJR TODO add logger
  void simulate();
  void simulatePhoton(PFParticle& ptc);
  void simulateHadron(PFParticle& ptc);

  PFParticle& addParticle(int pdgid, double theta, double phi, double energy, TVector3 vertex = TVector3(0., 0., 0.));

  const Cluster& cluster(long clusterId) const;
  Clusters ecalClusters() const {return m_ecalClusters;} ///<Copy of Ecal clusters
  Clusters hcalClusters() const {return m_hcalClusters;} ///<Copy of Hcal clusters
  Clusters smearedEcalClusters() const {return m_smearedEcalClusters;} ///<Copy of smeared Ecal clusters
  Clusters smearedHcalClusters() const {return m_smearedHcalClusters;} ///<Copy of smeared Hcal clusters

  Tracks   tracks() const {return m_tracks;} ///<Copy of tracks
  Tracks   smearedTracks() const {return m_smearedTracks;}  ///<Copy of smeared tracks
  Nodes&   historyNodes() {return m_nodes;} ///reference to history //TODO decide if a copy should be made
  const Particles& particles() const {return m_particles;} ///<Copy of particles

  void testing();
  Ids linkedEcalSmearedClusterIds(long nodeid) const; //TODO move to helper/history class
  Ids linkedParticleIds(long nodeid) const ; //TODO move to helper/history class
  Ids parentParticleIds(long nodeid) const ; //TODO move to helper/history class

private:
  PFParticle& addParticle(int pdgid, TLorentzVector tlv, TVector3 vertex = TVector3(0., 0., 0.));
  void propagate(PFParticle& ptc, const SurfaceCylinder&); //more args needed
  long makeClusterId(fastsim::enumLayer layer, fastsim::enumSubtype subtype) const;
  long makeParticleid(fastsim::enumSource source) const;
  long addEcalCluster(PFParticle& ptc, long parentid = 0, double fraction = 1., double csize = 0.);
  long addHcalCluster(PFParticle& ptc, long parentid = 0, double fraction = 1., double csize = 0.);
  long addSmearedCluster(long parentClusterId);
  Cluster makeCluster(PFParticle& ptc, long parentid, fastsim::enumLayer layer, double fraction = 1., double csize = 0.);
  Cluster makeSmearedCluster(long parentClusterId);
  const Track& addTrack(PFParticle& ptc);
  long addSmearedTrack(const Track& track, bool accept = false);
  void addNode(const long newid, const long parentid = 0);
  std::shared_ptr<const DetectorElement> elem(fastsim::enumLayer layer) const;
  
  Ids linkedRawTrackIds(long nodeid) const; //TODO move to helper/history class
  Ids linkedSmearedTrackIds(long nodeid) const; //TODO move to helper/history class
  Ids linkedIds(long nodeid) const; //TODO move to helper/history class
  Ids getMatchingIds(long nodeid, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                     fastsim::enumSubtype type, fastsim::enumSource source) const; //TODO move to helper/history class
  Ids getMatchingParentIds(long nodeid, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                           fastsim::enumSubtype type, fastsim::enumSource source) const ;  //TODO move to helper/history class

  Clusters m_ecalClusters;
  Clusters m_hcalClusters;
  Clusters m_smearedEcalClusters;
  Clusters m_smearedHcalClusters;
  Tracks   m_tracks;        ///< pre smeared tracks
  Tracks   m_smearedTracks; ///< smeared tracks
  Particles m_particles;    ///< all particles

  Nodes m_nodes; ///< Records relationships of everything that is simulated
  const Detector& m_detector;
  StraightLinePropagator m_propStraight;
  HelixPropagator m_propHelix;
};

#endif

