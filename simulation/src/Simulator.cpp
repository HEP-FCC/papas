//
//  Created by Alice Robson on 09/11/15.
//
//
#include "Simulator.h"

#include <iostream>  //TODOAJR remove

#include "Cluster.h"
#include "Definitions.h"
#include "Id.h"
#include "PDebug.h"
#include "ParticlePData.h"
#include "Path.h"
#include "Random.h"
#include "SimParticle.h"
#include "pTrack.h"

namespace papas {

Simulator::Simulator(const Detector& d, Nodes& nodes)
    : m_nodes(nodes), m_detector(d), m_propStraight(), m_propHelix(d.field()->getMagnitude()) {}

void Simulator::simulateParticle(const Particle& ptc, IdType parentid) {
  int pdgid = ptc.pdgId();
  SimParticle simParticle = makeSimParticle(pdgid, ptc.charge(), ptc.p4(), TVector3{0, 0, 0});
  SimParticle& storedParticle = storeSimParticle(std::move(simParticle), parentid);
  PDebug::write("Made {}", storedParticle);

  if (ptc.charge() && ptc.pt() < 0.2 && abs(pdgid) >= 100) {
    // to avoid numerical problems in propagation
    return;
  }

  if (pdgid == 22) {
    simulatePhoton(storedParticle);
  } else if (abs(pdgid) == 11) {
    smearElectron(storedParticle);
  } else if (abs(pdgid) == 13) {
    smearMuon(storedParticle);
  } else if ((abs(pdgid) == 12) | (abs(pdgid) == 14) | (abs(pdgid) == 16)) {
    simulateNeutrino(storedParticle);
  } else if (abs(pdgid) >= 100) {
    simulateHadron(storedParticle);
  }
}

void Simulator::simulatePhoton(SimParticle& ptc) {
  PDebug::write("Simulating Photon");
  auto ecal_sp = m_detector.ecal();  // ECAL detector element

  // find where the photon meets the Ecal inner cylinder
  // make and smear the cluster
  propagate(ptc, ecal_sp->volumeCylinder().inner());
  auto detectorLayer = papas::Layer::kEcal;
  auto cluster = makeCluster(ptc, papas::Layer::kEcal);
  auto storedCluster = storeEcalCluster(std::move(cluster), ptc.id());
  auto smeared = smearCluster(storedCluster, detectorLayer);
  if (acceptSmearedCluster(smeared, detectorLayer)) {
    storeSmearedCluster(std::move(smeared), cluster.id());
  }
}

void Simulator::simulateHadron(SimParticle& ptc) {
  PDebug::write("Simulating Hadron");
  auto ecal_sp = m_detector.ecal();
  auto hcal_sp = m_detector.hcal();
  auto field_sp = m_detector.field();
  double fracEcal = 0.;  // TODO ask Colin

  // make a track if it is charged
  if (ptc.charge() != 0) {
    auto track = Track(ptc.p3(), ptc.charge(), ptc.path());
    auto storedtrack = storeTrack(std::move(track), ptc.id());
    auto smeared = smearTrack(storedtrack);  // smear it
    if (acceptSmearedTrack(smeared)) {
      storeSmearedTrack(std::move(smeared), storedtrack.id());
    }
  }
  // find where it meets the inner Ecal cyclinder
  propagate(ptc, ecal_sp->volumeCylinder().inner());
  double pathLength = ecal_sp->material().pathLength(ptc.isElectroMagnetic());

  if (pathLength < std::numeric_limits<double>::max()) {

    /// ecal path length can be infinite in case the ecal
    /// has lambda_I = 0 (fully transparent to hadrons)
    auto path = ptc.path();
    double timeEcalInner = path->timeAtZ(path->namedPoint(papas::Position::kEcalIn).Z());
    double deltaT = path->deltaT(pathLength);
    double timeDecay = timeEcalInner + deltaT;
    TVector3 pointDecay = path->pointAtTime(timeDecay);
    path->addPoint(papas::Position::kEcalDecay, pointDecay);
    if (ecal_sp->volumeCylinder().contains(pointDecay)) {
      fracEcal = randomgen::RandUniform(0., 0.7).next();
      auto cluster = makeCluster(ptc, papas::Layer::kEcal);
      auto storedCluster = storeEcalCluster(std::move(cluster), ptc.id());
      // For now, using the hcal resolution and acceptance for hadronic cluster
      // in the Ecal. That's not a bug!
      // addSmearedCluster(cluster(ecalId), papas::Layer::kHcal, papas::Layer::kEcal);

      auto detectorLayer = papas::Layer::kHcal;
      auto smeared = smearCluster(storedCluster, detectorLayer);
      if (acceptSmearedCluster(smeared, detectorLayer, papas::Layer::kEcal)) {
        storeSmearedCluster(std::move(smeared), cluster.id());
      }
    }
  }

  // now find where it reaches into HCAL
  propagate(ptc, hcal_sp->volumeCylinder().inner());

  auto hcalCluster = makeCluster(ptc, papas::Layer::kHcal, 1 - fracEcal);
  auto storedHcalCluster = storeHcalCluster(std::move(hcalCluster), ptc.id());
  auto hcalSmeared = smearCluster(storedHcalCluster, papas::Layer::kHcal);
  if (acceptSmearedCluster(hcalSmeared, papas::Layer::kHcal)) {
    storeSmearedCluster(std::move(hcalSmeared), hcalCluster.id());
  }
}

void Simulator::propagateAllLayers(SimParticle& ptc) {
  auto ecal_sp = m_detector.ecal();  // ECAL detector element
  auto hcal_sp = m_detector.hcal();  // HCAL detector element
  propagate(ptc, ecal_sp->volumeCylinder().inner());
  propagate(ptc, ecal_sp->volumeCylinder().outer());
  propagate(ptc, hcal_sp->volumeCylinder().inner());
  propagate(ptc, hcal_sp->volumeCylinder().outer());
}

void Simulator::simulateNeutrino(SimParticle& ptc) {
  PDebug::write("Simulating Neutrino \n");
  propagateAllLayers(ptc);
}

void Simulator::smearElectron(SimParticle& ptc) {
  PDebug::write("Smearing Electron");
  auto track = Track(ptc.p3(), ptc.charge(), ptc.path());
  storeTrack(std::move(track), ptc.id());
  auto ecal_sp = m_detector.ecal();  // ECAL detector element
  propagate(ptc, ecal_sp->volumeCylinder().inner());
  // SimParticle smeared{ptc}; //this line to match deepcopy on python
  // PDebug::write("Made Smeared{}", smeared)
  // TODO ask COLIN why bother when its not smearedsmeared = copy.deepcopy(ptc)
}

void Simulator::smearMuon(SimParticle& ptc) {
  PDebug::write("Smearing Muon");
  auto track = Track(ptc.p3(), ptc.charge(), ptc.path());
  storeTrack(std::move(track), ptc.id());
  propagateAllLayers(ptc);
}

void Simulator::propagate(SimParticle& ptc, const SurfaceCylinder& cylinder) {
  bool isNeutral = fabs(ptc.charge()) < 0.5;
  if (isNeutral)
    m_propStraight.propagateOne(ptc, cylinder);
  else
    m_propHelix.propagateOne(ptc, cylinder);
}

const Cluster& Simulator::cluster(IdType clusterId) const {
  if (Id::isEcal(clusterId))
    return m_ecalClusters.at(clusterId);
  else if (Id::isHcal(clusterId))
    return m_hcalClusters.at(clusterId);
  throw std::out_of_range("Cluster not found");
}

SimParticle Simulator::makeSimParticle(int pdgid, double charge, TLorentzVector tlv, TVector3 vertex) {

  double field = m_detector.field()->getMagnitude();
  IdType uniqueid = Id::makeParticleId();
  SimParticle simParticle{uniqueid, pdgid, charge, tlv, vertex, field};
  return std::move(simParticle);
}

SimParticle Simulator::makeSimParticle(int pdgid, double charge, double theta, double phi, double energy,
                                       TVector3 vertex) {
  double mass = ParticlePData::particleMass(pdgid);
  double momentum = sqrt(pow(energy, 2) - pow(mass, 2));
  double costheta = cos(theta);
  double sintheta = sin(theta);
  double cosphi = cos(phi);
  double sinphi = sin(phi);
  TLorentzVector p4(momentum * sintheta * cosphi, momentum * sintheta * sinphi, momentum * costheta, energy);
  return std::move(makeSimParticle(pdgid, charge, p4, vertex));
}

SimParticle& Simulator::storeSimParticle(SimParticle&& simParticle, IdType parentId) {
  auto id = simParticle.id();
  m_particles.emplace(id, std::move(simParticle));
  addNode(id, parentId);  // add node to history graph
  return m_particles[id];
}

SimParticle& Simulator::addGunParticle(int pdgid, double charge, double thetamin, double thetamax, double ptmin,
                                       double ptmax, TVector3 vertex) {
  double theta = randomgen::RandUniform(thetamin, thetamax).next();
  double phi = randomgen::RandUniform(-M_PI, M_PI).next();
  double energy = randomgen::RandUniform(ptmin, ptmax).next();
  double mass = ParticlePData::particleMass(pdgid);
  double costheta = cos(M_PI / 2 - theta);
  double sintheta = sin(M_PI / 2 - theta);
  double cosphi = cos(phi);
  double sinphi = sin(phi);
  double momentum = energy / sintheta;
  energy = sqrt(pow(momentum, 2) + pow(mass, 2));

  TLorentzVector p4(momentum * sintheta * cosphi, momentum * sintheta * sinphi, momentum * costheta, energy);
  auto simParticle = makeSimParticle(pdgid, charge, p4, vertex);
  return storeSimParticle(std::move(simParticle), 0);
}

Cluster Simulator::makeCluster(SimParticle& ptc, papas::Layer layer, double fraction, double csize) {
  double energy = ptc.p4().E() * fraction;
  papas::Position clayer = m_detector.calorimeter(layer)->volumeCylinder().innerLayer();
  TVector3 pos = ptc.pathPosition(clayer);
  if (csize == -1.) {  // ie value not provided
    csize = m_detector.calorimeter(layer)->clusterSize(ptc);
  }
  Cluster cluster{energy, pos, csize, Id::itemType(layer)};
  return std::move(cluster);
}

const Cluster& Simulator::storeEcalCluster(Cluster&& cluster, IdType parentId) {
  IdType id = cluster.id();
  addNode(id, parentId);
  m_ecalClusters.emplace(id, std::move(cluster));
  PDebug::write("Made {}", cluster);
  return m_ecalClusters[id];
}

const Cluster& Simulator::storeHcalCluster(Cluster&& cluster, IdType parentId) {
  IdType id = cluster.id();
  addNode(id, parentId);
  m_hcalClusters.emplace(id, std::move(cluster));
  PDebug::write("Made {}", cluster);
  return m_hcalClusters[id];
}
Cluster Simulator::smearCluster(const Cluster& parent, papas::Layer detectorLayer) {
  // detectorLayer will be used to choose which detector layer is used for energy resolution etc.
  // NB It is not always the same layer as the new smeared cluster
  if (detectorLayer == papas::Layer::kNone) detectorLayer = Id::layer(parent.id()); //default to same layer as cluster
  std::shared_ptr<const Calorimeter> sp_calorimeter = m_detector.calorimeter(detectorLayer);
  double energyresolution = sp_calorimeter->energyResolution(parent.energy(), parent.eta());
  double response = sp_calorimeter->energyResponse(parent.energy(), parent.eta());
  double energy = parent.energy() * randomgen::RandNormal(response, energyresolution).next();
  auto cluster = Cluster{energy, parent.position(), parent.size(), Id::itemType(parent.id())};
  PDebug::write("Made Smeared{}", cluster);
  return std::move(cluster);
}

bool Simulator::acceptSmearedCluster(const Cluster& smearedCluster, papas::Layer detectorLayer,
                                     papas::Layer acceptLayer, bool accept) {

  // Determine if this smeared cluster will be detected
  if (acceptLayer == papas::Layer::kNone) acceptLayer = detectorLayer;
  if (m_detector.calorimeter(acceptLayer)->acceptance(smearedCluster) || accept) {
    return true;
  } else {
    PDebug::write("Rejected Smeared{}", smearedCluster);
    return false;
  }
}

const Cluster& Simulator::storeSmearedCluster(Cluster&& smearedCluster, IdType parentId) {

  auto id = smearedCluster.id();
  addNode(id, parentId);
  if (Id::layer(parentId) == papas::Layer::kEcal) {
    m_smearedEcalClusters.emplace(id, std::move(smearedCluster));
    return m_smearedEcalClusters[id];
  } else {
    m_smearedHcalClusters.emplace(id, std::move(smearedCluster));
    return m_smearedHcalClusters[id];
  }
}

const Track& Simulator::storeTrack(Track&& track, IdType parentid) {
  IdType id = track.id();
  m_tracks.emplace(id, std::move(track));
  addNode(id, parentid);
  PDebug::write("Made {}", track);
  return m_tracks.at(id);
}

Track Simulator::smearTrack(const Track& track) {
  double ptResolution = m_detector.tracker()->ptResolution(track);
  double scale_factor = randomgen::RandNormal(1, ptResolution).next();
  Track smeared = Track{track.p3() * scale_factor, track.charge(), track.path()};
  PDebug::write("Made Smeared{}", smeared);
  return std::move(smeared);
}

const Track& Simulator::storeSmearedTrack(Track&& smearedtrack, IdType parentid) {
  auto id = smearedtrack.id();
  addNode(id, parentid);
  m_smearedTracks.emplace(smearedtrack.id(), std::move(smearedtrack));

  return m_smearedTracks[id];
}

bool Simulator::acceptSmearedTrack(const Track& smearedtrack, bool accept) {
  // decide whether the smearedTrack is detected
  if (m_detector.tracker()->acceptance(smearedtrack) || accept) {
    return true;
  } else {
    PDebug::write("Rejected Smeared{}", smearedtrack);
    return false;
  }
}

void Simulator::addNode(IdType newid, const IdType parentid) {
  // add the new node into the set of all nodes
  PFNode node{newid};
  m_nodes.emplace(newid, std::move(node));
  if (parentid) {
    PFNode& parent = m_nodes[parentid];
    PFNode& child = m_nodes[newid];
    parent.addChild(child);
  }
}

void Simulator::clear() {
  m_ecalClusters.clear();
  m_hcalClusters.clear();
  m_smearedEcalClusters.clear();
  m_smearedHcalClusters.clear();
  m_tracks.clear();
  m_smearedTracks.clear();
  m_particles.clear();
}

std::shared_ptr<const DetectorElement> Simulator::elem(papas::Layer layer) const { return m_detector.element(layer); }

void Simulator::testing() {
  DAG::BFSVisitor<PFNode> bfs;
  for (auto p : m_particles) {
    std::cout << "Connected to " << p.first << std::endl;
    auto res = bfs.traverseUndirected(m_nodes[p.first]);
    for (auto r : res)
      std::cout << "  " << r->value() << ": " << Id::itemType(r->value()) << std::endl;
  }
}

Ids Simulator::linkedIds(IdType nodeid) const {
  DAG::BFSVisitor<PFNode> bfs;
  Ids foundids;
  foundids.reserve(1000);  // TODO how
  auto res = bfs.traverseUndirected(m_nodes.at(nodeid));
  for (auto r : res) {
    foundids.push_back(r->value());
  }
  return foundids;
}

/*
 Ids Simulator::linkedEcalSmearedClusterIds(long nodeid) const {
 return matchingIds(nodeid,
 Id::DataType::kCluster,
 papas::Layer::kEcal,
 papas::SubType::SMEARED,
 papas::enumSource::SIMULATION);
 }

 Ids  Simulator::linkedRawTrackIds(long nodeid) const {
 return matchingIds(nodeid,
 Id::DataType::kTrack,
 papas::Layer::kNone,
 papas::SubType::RAW,
 papas::enumSource::SIMULATION);
 }

 Ids Simulator::linkedSmearedTrackIds(long nodeid) const {
 return matchingIds(nodeid,
 Id::DataType::kTrack,
 papas::Layer::kNone,
 papas::SubType::SMEARED,
 papas::enumSource::SIMULATION);
 }

 Ids Simulator::linkedParticleIds(long nodeid) const {
 return matchingIds(nodeid,
 Id::DataType::kParticle,
 papas::Layer::kNone,
 papas::SubType::RAW,
 papas::enumSource::SIMULATION);
 }

 Ids Simulator::parentParticleIds(long nodeid) const {
 return getMatchingParentIds(nodeid,
 Id::DataType::kParticle,
 papas::Layer::kNone,
 papas::SubType::RAW,
 papas::enumSource::SIMULATION);
 }*/

/*
 Ids Simulator::matchingIds(long nodeid, Id::DataType datatype, papas::Layer layer, papas::SubType type,
 papas::enumSource source) const
 {
 DAG::BFSVisitor<PFNode> bfs;
 Ids foundids;
 auto res =bfs.traverseUndirected(m_nodes.at(nodeid));
 for (auto r : res)
 {
 long id=r->value();
 if(Id::isUniqueIdMatch(id, datatype, layer, type, source)) {
 foundids.push_back(id);
 }
 }
 return foundids;
 }

 Ids Simulator::getMatchingParentIds(long nodeid, Id::DataType datatype, papas::Layer layer, papas::SubType
 type,papas::enumSource source) const
 {
 DAG::BFSVisitor<PFNode> bfs;
 Ids foundids;
 //foundids.reserve(1000); //TODO set sizes sensible.... how
 auto res =bfs.traverseParents(m_nodes.at(nodeid));
 for (auto r : res)
 {
 long id=r->value();
 if(Id::isUniqueIdMatch(id, datatype, layer, type, source)) {
 foundids.push_back(id);
 }
 }
 return foundids;
 }*/

}  // end namespace papas