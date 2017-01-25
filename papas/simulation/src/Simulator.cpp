#include "papas/simulation/Simulator.h"
#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/Identifier.h"
#include "papas/datatypes/PFParticle.h"
#include "papas/datatypes/ParticlePData.h"
#include "papas/datatypes/Path.h"
#include "papas/datatypes/Track.h"
#include "papas/utility/PDebug.h"
#include "papas/utility/TRandom.h"
class Detector;

namespace papas {

Simulator::Simulator(const PapasEvent& papasevent, const ListParticles& particles, const Detector& detector,
                     Clusters& ecalClusters, Clusters& hcalClusters, Clusters& smearedEcalClusters,
                     Clusters& smearedHcalClusters, Tracks& tracks, Tracks& smearedTracks, PFParticles& simParticles,
                     Nodes& history)
    : m_papasEvent(papasevent),
      m_detector(detector),
      m_ecalClusters(ecalClusters),
      m_hcalClusters(hcalClusters),
      m_smearedEcalClusters(smearedEcalClusters),
      m_smearedHcalClusters(smearedHcalClusters),
      m_tracks(tracks),
      m_smearedTracks(smearedTracks),
      m_particles(simParticles),
      m_history(history),
      m_propHelix(detector.field()->getMagnitude()) {

  for (auto p : particles) {
    simulateParticle(p);
  }
}

void Simulator::simulateParticle(const Particle& ptc) {
  int pdgid = ptc.pdgId();
  PFParticle simParticle = makePFParticle(pdgid, ptc.charge(), ptc.p4(), ptc.startVertex());
  PFParticle& storedParticle = storePFParticle(std::move(simParticle), 0);
  PDebug::write("Made {}", storedParticle);

  if (ptc.charge() && ptc.pt() < 0.2 && abs(pdgid) >= 100) {
    // to avoid numerical problems in propagation
    return;
  }

  if (pdgid == 22) {
    simulatePhoton(storedParticle);
  } else if (abs(pdgid) == 11) {
    simulateElectron(storedParticle);
  } else if (abs(pdgid) == 13) {
    simulateMuon(storedParticle);
  } else if ((abs(pdgid) == 12) | (abs(pdgid) == 14) | (abs(pdgid) == 16)) {
    simulateNeutrino(storedParticle);
  } else if (abs(pdgid) >= 100) {
    simulateHadron(storedParticle);
  }
}

void Simulator::simulatePhoton(PFParticle& ptc) {
  PDebug::write("Simulating Photon");
  auto ecal_sp = m_detector.ecal();  // ECAL detector element

  // find where the photon meets the Ecal inner cylinder
  // make and smear the cluster
  propagate(ecal_sp->volumeCylinder().inner(), ptc);
  auto detectorLayer = papas::Layer::kEcal;
  auto cluster = makeCluster(ptc, papas::Layer::kEcal);
  const auto& storedCluster = storeEcalCluster(std::move(cluster), ptc.id());
  auto smeared = smearCluster(storedCluster, detectorLayer);
  if (acceptSmearedCluster(smeared, detectorLayer)) {
    storeSmearedCluster(std::move(smeared), cluster.id());
  }
}

void Simulator::simulateHadron(PFParticle& ptc) {
  PDebug::write("Simulating Hadron");
  auto ecal_sp = m_detector.ecal();
  auto hcal_sp = m_detector.hcal();
  auto field_sp = m_detector.field();
  double fracEcal = 0.;  // TODO ask Colin

  // make a track if it is charged
  if (ptc.charge() != 0) {
    auto track = Track(ptc.p3(), ptc.charge(), ptc.path(), 't');
    auto storedtrack = storeTrack(std::move(track), ptc.id());
    auto smeared = smearTrack(storedtrack, m_detector.tracker()->ptResolution(storedtrack));  // smear it
    if (acceptSmearedTrack(smeared)) {
      storeSmearedTrack(std::move(smeared), storedtrack.id());
    }
  }
  // find where it meets the inner Ecal cyclinder
  propagate(ecal_sp->volumeCylinder().inner(), ptc);
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
      // fracEcal = randomgen::RandUniform(0., 0.7).next();
      fracEcal = rootrandom::Random::uniform(0., 0.7);
      auto cluster = makeCluster(ptc, papas::Layer::kEcal, fracEcal);
      const auto& storedCluster = storeEcalCluster(std::move(cluster), ptc.id());
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
  propagate(hcal_sp->volumeCylinder().inner(), ptc);

  auto hcalCluster = makeCluster(ptc, papas::Layer::kHcal, 1 - fracEcal);
  const auto& storedHcalCluster = storeHcalCluster(std::move(hcalCluster), ptc.id());
  auto hcalSmeared = smearCluster(storedHcalCluster, papas::Layer::kHcal);
  if (acceptSmearedCluster(hcalSmeared, papas::Layer::kHcal)) {
    storeSmearedCluster(std::move(hcalSmeared), hcalCluster.id());
  }
}

void Simulator::propagateAllLayers(PFParticle& ptc) {
  auto ecal_sp = m_detector.ecal();  // ECAL detector element
  auto hcal_sp = m_detector.hcal();  // HCAL detector element
  propagate(ecal_sp->volumeCylinder().inner(), ptc);
  propagate(ecal_sp->volumeCylinder().outer(), ptc);
  propagate(hcal_sp->volumeCylinder().inner(), ptc);
  propagate(hcal_sp->volumeCylinder().outer(), ptc);
}

void Simulator::simulateNeutrino(PFParticle& ptc) {
  PDebug::write("Simulating Neutrino \n");
  propagateAllLayers(ptc);
}

void Simulator::smearElectron(PFParticle& ptc) {
  PDebug::write("Smearing Electron");
  auto track = Track(ptc.p3(), ptc.charge(), ptc.path(), 't');
  storeTrack(std::move(track), ptc.id());
  auto ecal_sp = m_detector.ecal();  // ECAL detector element
  propagate(ecal_sp->volumeCylinder().inner(), ptc);
  // PFParticle smeared{ptc}; //this line to match deepcopy on python
  // PDebug::write("Made Smeared{}", smeared)
  // TODO ask COLIN why bother when its not smearedsmeared = copy.deepcopy(ptc)
}

void Simulator::simulateElectron(PFParticle& ptc) {
  /*Simulate an electron corresponding to gen particle ptc.

   Uses the methods detector.electronEnergyResolution
   and detector.electronAcceptance to smear the electron track.
   Later on, the particle flow algorithm will use the tracks
   coming from an electron to reconstruct electrons.

   This method does not simulate an electron energy deposit in the ECAL.*/
  PDebug::write("Simulating Electron");
  auto track = Track(ptc.p3(), ptc.charge(), ptc.path(), 't');
  auto storedtrack = storeTrack(std::move(track), ptc.id());
  auto eres = m_detector.electronEnergyResolution(ptc);
  auto smeared = smearTrack(storedtrack, eres);  // smear it
  if (acceptElectronSmearedTrack(smeared)) {
    storeSmearedTrack(std::move(smeared), storedtrack.id());
  }

  auto ecal_sp = m_detector.ecal();  // ECAL detector element
  propagate(ecal_sp->volumeCylinder().inner(), ptc);
}

void Simulator::smearMuon(PFParticle& ptc) {
  PDebug::write("Smearing Muon");
  auto track = Track(ptc.p3(), ptc.charge(), ptc.path(), 't');
  storeTrack(std::move(track), ptc.id());
  propagateAllLayers(ptc);
}

void Simulator::simulateMuon(PFParticle& ptc) {
  /*Simulate a muon corresponding to gen particle ptc

  Uses the methods detector.muon_energy_resolution
  and detector.muon_acceptance to smear the muon track.
  Later on, the particle flow algorithm will use the tracks
  coming from a muon to reconstruct muons.

  This method does not simulate energy deposits in the calorimeters
  */
  PDebug::write("Simulating Muon");
  propagateAllLayers(ptc);
  auto ptres = m_detector.muonPtResolution(ptc);
  auto track = Track(ptc.p3(), ptc.charge(), ptc.path(), 't');
  auto storedtrack = storeTrack(std::move(track), ptc.id());
  auto smeared = smearTrack(storedtrack, ptres);  // smear it
  if (acceptMuonSmearedTrack(smeared)) {
    storeSmearedTrack(std::move(smeared), storedtrack.id());
  }
}

void Simulator::propagate(const SurfaceCylinder& cylinder, PFParticle& ptc) {
  bool isNeutral = fabs(ptc.charge()) < 0.5;
  if (isNeutral)
    m_propStraight.propagateOne(ptc, cylinder);
  else
    m_propHelix.propagateOne(ptc, cylinder);
}

const Cluster& Simulator::cluster(IdType clusterId) const {
  if (Identifier::isEcal(clusterId))
    return m_ecalClusters.at(clusterId);
  else if (Identifier::isHcal(clusterId))
    return m_hcalClusters.at(clusterId);
  throw std::out_of_range("Cluster not found");
}

PFParticle Simulator::makePFParticle(int pdgid, double charge, const TLorentzVector& tlv,
                                     const TVector3& vertex) const {

  double field = m_detector.field()->getMagnitude();
  auto simParticle = PFParticle(pdgid, charge, tlv, vertex, field, 's');
  return std::move(simParticle);
}

PFParticle Simulator::makePFParticle(int pdgid, double charge, double theta, double phi, double energy,
                                     const TVector3& vertex) const {
  double mass = ParticlePData::particleMass(pdgid);
  double momentum = sqrt(pow(energy, 2) - pow(mass, 2));
  double costheta = cos(theta);
  double sintheta = sin(theta);
  double cosphi = cos(phi);
  double sinphi = sin(phi);
  TLorentzVector p4(momentum * sintheta * cosphi, momentum * sintheta * sinphi, momentum * costheta, energy);
  return makePFParticle(pdgid, charge, p4, vertex);
}

PFParticle& Simulator::storePFParticle(PFParticle&& simParticle, IdType parentId) {
  auto id = simParticle.id();
  m_particles.emplace(id, std::move(simParticle));
  addNode(id, parentId);  // add node to history graph
  return m_particles[id];
}

PFParticle& Simulator::addGunParticle(int pdgid, double charge, double thetamin, double thetamax, double ptmin,
                                      double ptmax, const TVector3& vertex) {
  double theta = rootrandom::Random::uniform(thetamin, thetamax);
  double phi = rootrandom::Random::uniform(-M_PI, M_PI);
  double energy = rootrandom::Random::uniform(ptmin, ptmax);
  double mass = ParticlePData::particleMass(pdgid);
  double costheta = cos(M_PI / 2 - theta);
  double sintheta = sin(M_PI / 2 - theta);
  double cosphi = cos(phi);
  double sinphi = sin(phi);
  double momentum = energy / sintheta;
  energy = sqrt(pow(momentum, 2) + pow(mass, 2));

  TLorentzVector p4(momentum * sintheta * cosphi, momentum * sintheta * sinphi, momentum * costheta, energy);
  auto simParticle = makePFParticle(pdgid, charge, p4, vertex);
  return storePFParticle(std::move(simParticle), 0);
}

Cluster Simulator::makeCluster(const PFParticle& ptc, papas::Layer layer, double fraction, double csize,
                               char subtype) const {
  double energy = ptc.p4().E() * fraction;
  papas::Position clayer = m_detector.calorimeter(layer)->volumeCylinder().innerLayer();
  TVector3 pos = ptc.pathPosition(clayer);
  if (csize == -1.) {  // ie value not provided
    csize = m_detector.calorimeter(layer)->clusterSize(ptc);
  }
  auto cluster = Cluster(energy, pos, csize, Identifier::itemType(layer), subtype);
  return cluster;
}

const Cluster& Simulator::storeEcalCluster(Cluster&& cluster, IdType parentId) {
  IdType id = cluster.id();
  addNode(id, parentId);
  PDebug::write("Made {}", cluster);
  m_ecalClusters.emplace(id, std::move(cluster));
  return m_ecalClusters[id];
}

const Cluster& Simulator::storeHcalCluster(Cluster&& cluster, IdType parentId) {
  IdType id = cluster.id();
  addNode(id, parentId);
  PDebug::write("Made {}", cluster);
  m_hcalClusters.emplace(id, std::move(cluster));
  return m_hcalClusters[id];
}
Cluster Simulator::smearCluster(const Cluster& parent, papas::Layer detectorLayer) {
  // detectorLayer will be used to choose which detector layer is used for energy resolution etc.
  // NB It is not always the same layer as the new smeared cluster
  if (detectorLayer == papas::Layer::kNone)
    detectorLayer = Identifier::layer(parent.id());  // default to same layer as cluster
  std::shared_ptr<const Calorimeter> sp_calorimeter = m_detector.calorimeter(detectorLayer);
  double energyresolution = sp_calorimeter->energyResolution(parent.energy(), parent.eta());
  double response = sp_calorimeter->energyResponse(parent.energy(), parent.eta());
  double energy = parent.energy() * rootrandom::Random::gauss(response, energyresolution);
  // energy = fmax(0., energy);  // energy always positive
  auto cluster = Cluster(energy, parent.position(), parent.size(), Identifier::itemType(parent.id()), 's');
  PDebug::write("Made Smeared{}", cluster);
  return cluster;
}

bool Simulator::acceptSmearedCluster(const Cluster& smearedCluster, papas::Layer detectorLayer,
                                     papas::Layer acceptLayer, bool accept) const {

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
  if (Identifier::layer(parentId) == papas::Layer::kEcal) {
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

Track Simulator::smearTrack(const Track& track, double resolution) const {
  // double ptResolution = m_detector.tracker()->ptResolution(track);
  double scale_factor = rootrandom::Random::gauss(1, resolution);
  auto smeared = Track(track.p3() * scale_factor, track.charge(), track.path(), 's');
  PDebug::write("Made Smeared{}", smeared);
  return smeared;
}

const Track& Simulator::storeSmearedTrack(Track&& smearedtrack, IdType parentid) {
  auto id = smearedtrack.id();
  addNode(id, parentid);
  m_smearedTracks.emplace(smearedtrack.id(), std::move(smearedtrack));
  return m_smearedTracks[id];
}

bool Simulator::acceptSmearedTrack(const Track& smearedTrack, bool accept) const {
  // decide whether the smearedTrack is detected
  if (m_detector.tracker()->acceptance(smearedTrack) || accept) {
    return true;
  } else {
    PDebug::write("Rejected Smeared{}", smearedTrack);
    return false;
  }
}

bool Simulator::acceptElectronSmearedTrack(const Track& smearedTrack, bool accept) const {
  // decide whether the electron smearedTrack is detected
  if (m_detector.electronAcceptance(smearedTrack) || accept) {
    return true;
  } else {
    PDebug::write("Rejected Smeared{}", smearedTrack);
    return false;
  }
}

bool Simulator::acceptMuonSmearedTrack(const Track& smearedTrack, bool accept) const {
  // decide whether the electron smearedTrack is detected
  if (m_detector.muonAcceptance(smearedTrack) || accept) {
    return true;
  } else {
    PDebug::write("Rejected Smeared{}", smearedTrack);
    return false;
  }
}

void Simulator::addNode(IdType newid, const IdType parentid) {
  // add the new node into the set of all nodes
  PFNode node{newid};
  m_history.emplace(newid, std::move(node));
  if (parentid) {
    PFNode& parent = m_history[parentid];
    PFNode& child = m_history[newid];
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
    auto res = bfs.traverseUndirected(m_history[p.first]);
    for (auto r : res)
      std::cout << "  " << r->value() << ": " << Identifier::itemType(r->value()) << std::endl;
  }
}

Ids Simulator::linkedIds(IdType nodeid) const {
  DAG::BFSVisitor<PFNode> bfs;
  Ids foundids;
  auto res = bfs.traverseUndirected(m_history.at(nodeid));
  for (auto r : res) {
    foundids.push_back(r->value());
  }
  return foundids;
}

/*
 Ids Simulator::linkedEcalSmearedClusterIds(long nodeid) const {
 return matchingIds(nodeid,
 Identifier::DataType::kCluster,
 papas::Layer::kEcal,
 papas::SubType::SMEARED,
 papas::enumSource::SIMULATION);
 }

 Ids  Simulator::linkedRawTrackIds(long nodeid) const {
 return matchingIds(nodeid,
 Identifier::DataType::kTrack,
 papas::Layer::kNone,
 papas::SubType::RAW,
 papas::enumSource::SIMULATION);
 }

 Ids Simulator::linkedSmearedTrackIds(long nodeid) const {
 return matchingIds(nodeid,
 Identifier::DataType::kTrack,
 papas::Layer::kNone,
 papas::SubType::SMEARED,
 papas::enumSource::SIMULATION);
 }

 Ids Simulator::linkedParticleIds(long nodeid) const {
 return matchingIds(nodeid,
 Identifier::DataType::kParticle,
 papas::Layer::kNone,
 papas::SubType::RAW,
 papas::enumSource::SIMULATION);
 }

 Ids Simulator::parentParticleIds(long nodeid) const {
 return getMatchingParentIds(nodeid,
 Identifier::DataType::kParticle,
 papas::Layer::kNone,
 papas::SubType::RAW,
 papas::enumSource::SIMULATION);
 }*/

/*
 Ids Simulator::matchingIds(long nodeid, Identifier::DataType datatype, papas::Layer layer, papas::SubType type,
 papas::enumSource source) const
 {
 DAG::BFSVisitor<PFNode> bfs;
 Ids foundids;
 auto res =bfs.traverseUndirected(m_history.at(nodeid));
 for (auto r : res)
 {
 long id=r->value();
 if(Identifier::isUniqueIdMatch(id, datatype, layer, type, source)) {
 foundids.push_back(id);
 }
 }
 return foundids;
 }

 Ids Simulator::getMatchingParentIds(long nodeid, Identifier::DataType datatype, papas::Layer layer, papas::SubType
 type,papas::enumSource source) const
 {
 DAG::BFSVisitor<PFNode> bfs;
 Ids foundids;
 //foundids.reserve(1000); //TODO set sizes sensible.... how
 auto res =bfs.traverseParents(m_history.at(nodeid));
 for (auto r : res)
 {
 long id=r->value();
 if(Identifier::isUniqueIdMatch(id, datatype, layer, type, source)) {
 foundids.push_back(id);
 }
 }
 return foundids;
 }*/

}  // end namespace papas