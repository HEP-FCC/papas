//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  SIMULATOR_H
#define  SIMULATOR_H

#include "TVector3.h"
#include "TLorentzVector.h"
#include "Definitions.h"
#include "DefinitionsNodes.h"
#include "DefinitionsCollections.h"
#include "propagator.h"

namespace papas {
class PFParticle;
class Cluster;
class Track;
class Detector;
class DetectorElement;
class SurfaceCylinder;

class Simulator {

public:
  Simulator(const Detector&);  //AJR TODO add logger
  void simulate();
  void simulatePhoton(PFParticle& ptc);
  void simulateHadron(PFParticle& ptc);
  void simulateNeutrino(PFParticle& ptc);

  PFParticle& addParticle(int pdgid, double theta, double phi, double energy, TVector3 vertex = TVector3(0., 0., 0.));

  const Cluster& cluster(Id::Type clusterId) const;
  Clusters ecalClusters() const {return m_ecalClusters;} ///<Copy of Ecal clusters
  Clusters hcalClusters() const {return m_hcalClusters;} ///<Copy of Hcal clusters
  Clusters smearedEcalClusters() const {return m_smearedEcalClusters;} ///<Copy of smeared Ecal clusters
  Clusters smearedHcalClusters() const {return m_smearedHcalClusters;} ///<Copy of smeared Hcal clusters

  Tracks   tracks() const {return m_tracks;} ///<Copy of tracks
  Tracks   smearedTracks() const {return m_smearedTracks;}  ///<Copy of smeared tracks
  Nodes&   historyNodes() {return m_nodes;} ///reference to history //TODO decide if a copy should be made
  const Particles& particles() const {return m_particles;} ///<Copy of particles

  void testing();
  Ids linkedEcalSmearedClusterIds(Id::Type nodeid) const; //TODO move to helper/history class
  Ids linkedParticleIds(Id::Type nodeid) const ; //TODO move to helper/history class
  Ids parentParticleIds(Id::Type nodeid) const ; //TODO move to helper/history class

private:
  PFParticle& addParticle(int pdgid, TLorentzVector tlv, TVector3 vertex = TVector3(0., 0., 0.));
  void propagate(PFParticle& ptc, const SurfaceCylinder&); //more args needed
  //Id::Type makeClusterId(papas::Layer layer, papas::SubType subtype) const;
  //Id::Type makeParticleId(papas::enumSource source) const;
  Id::Type addEcalCluster(PFParticle& ptc, Id::Type parentid = 0, double fraction = 1., double csize = 0.);
  Id::Type addHcalCluster(PFParticle& ptc, Id::Type parentid = 0, double fraction = 1., double csize = 0.);
  Id::Type addSmearedCluster(Id::Type parentClusterId);
  Cluster makeCluster(PFParticle& ptc, Id::Type parentid, papas::Layer layer, double fraction = 1., double csize = 0.);
  Cluster makeSmearedCluster(Id::Type parentClusterId);
  const Track& addTrack(PFParticle& ptc);
  Id::Type addSmearedTrack(const Track& track, bool accept = false);
  void addNode(const Id::Type newid, const Id::Type parentid = 0);
  std::shared_ptr<const DetectorElement> elem(papas::Layer layer) const;
  static TLorentzVector makeTLorentzVector(int pdgid, double theta, double phi, double energy);
  Ids linkedRawTrackIds(Id::Type nodeid) const; //TODO move to helper/history class
  Ids linkedSmearedTrackIds(Id::Type nodeid) const; //TODO move to helper/history class
  Ids linkedIds(Id::Type nodeid) const; //TODO move to helper/history class
  /*Ids getMatchingIds(Id::Type nodeid, Id::DataType datatype, papas::Layer layer,
                     papas::SubType type, papas::enumSource source) const; //TODO move to helper/history class
  Ids getMatchingParentIds(Id::Type nodeid, Id::DataType datatype, papas::Layer layer,
                           papas::SubType type, papas::enumSource source) const ;  //TODO move to helper/history class
  */
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
} // end namespace papas
#endif
