//
//  TestSimulator.cpp
//  papas
//
//  Created by Alice Robson on 15/12/16.
//
//


//
//  Created by Alice Robson on 09/11/15.
//
//
#include "papas/simulation/TestSimulator.h"

#include <iostream>  //TODOAJR remove

#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/Identifier.h"
#include "papas/utility/PDebug.h"
#include "papas/datatypes/ParticlePData.h"
#include "papas/datatypes/Path.h"
#include "papas/utility/Random.h"
#include "papas/datatypes/SimParticle.h"
#include "papas/datatypes/Track.h"


namespace papas {
  
  TestSimulator::TestSimulator(const Particles& particles, const Detector& detector,
            Clusters& ecalClusters,
            Clusters& hcalClusters,
            Clusters& smearedEcalClusters,
            Clusters& smearedHcalClusters,
            Tracks& tracks,
            Tracks& smearedTracks,
            SimParticles& simParticles,
            Nodes& history)
  :  m_detector(detector),
  m_ecalClusters(ecalClusters),
  m_hcalClusters(hcalClusters),
  m_smearedEcalClusters(smearedEcalClusters),
  m_smearedHcalClusters(smearedHcalClusters),
  m_tracks(tracks),
  m_smearedTracks(smearedTracks),
  m_particles(simParticles),
  m_history(history),
  m_propHelix(detector.field()->getMagnitude()) {
  
    for (auto p : particles)
    {
      simulateParticle(p.second);
    }
  }
  
  
  void TestSimulator::simulateParticle(const Particle& ptc) {
    int pdgid = ptc.pdgId();
    SimParticle simParticle = makeSimParticle(pdgid, ptc.charge(), ptc.p4(), TVector3{0, 0, 0});
    SimParticle& storedParticle = storeSimParticle(std::move(simParticle), 0);
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
  
  void TestSimulator::simulatePhoton(SimParticle& ptc) {
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
  
  void TestSimulator::simulateHadron(SimParticle& ptc) {
    PDebug::write("Simulating Hadron");
    auto ecal_sp = m_detector.ecal();
    auto hcal_sp = m_detector.hcal();
    auto field_sp = m_detector.field();
    double fracEcal = 0.;  // TODO ask Colin
    
    // make a track if it is charged
    if (ptc.charge() != 0) {
      auto track = Track(ptc.p3(), ptc.charge(), ptc.path(), 't');
      auto storedtrack = storeTrack(std::move(track), ptc.id());
      auto smeared = smearTrack(storedtrack);  // smear it
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
        fracEcal = randomgen::RandUniform(0., 0.7).next();
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
  
  void TestSimulator::propagateAllLayers(SimParticle& ptc) {
    auto ecal_sp = m_detector.ecal();  // ECAL detector element
    auto hcal_sp = m_detector.hcal();  // HCAL detector element
    propagate(ecal_sp->volumeCylinder().inner(), ptc);
    propagate(ecal_sp->volumeCylinder().outer(), ptc);
    propagate(hcal_sp->volumeCylinder().inner(), ptc);
    propagate(hcal_sp->volumeCylinder().outer(), ptc);
  }
  
  void TestSimulator::simulateNeutrino(SimParticle& ptc) {
    PDebug::write("Simulating Neutrino \n");
    propagateAllLayers(ptc);
  }
  
  void TestSimulator::smearElectron(SimParticle& ptc) {
    PDebug::write("Smearing Electron");
    auto track = Track(ptc.p3(), ptc.charge(), ptc.path(), 't');
    storeTrack(std::move(track), ptc.id());
    auto ecal_sp = m_detector.ecal();  // ECAL detector element
    propagate(ecal_sp->volumeCylinder().inner(), ptc);
    // SimParticle smeared{ptc}; //this line to match deepcopy on python
    // PDebug::write("Made Smeared{}", smeared)
    // TODO ask COLIN why bother when its not smearedsmeared = copy.deepcopy(ptc)
  }
  
  void TestSimulator::smearMuon(SimParticle& ptc) {
    PDebug::write("Smearing Muon");
    auto track = Track(ptc.p3(), ptc.charge(), ptc.path(), 't');
    storeTrack(std::move(track), ptc.id());
    propagateAllLayers(ptc);
  }
  
  void TestSimulator::propagate(const SurfaceCylinder& cylinder, SimParticle& ptc) {
    bool isNeutral = fabs(ptc.charge()) < 0.5;
    if (isNeutral)
      m_propStraight.propagateOne(ptc, cylinder);
    else
      m_propHelix.propagateOne(ptc, cylinder);
  }
  
  const Cluster& TestSimulator::cluster(IdType clusterId) const {
    if (Identifier::isEcal(clusterId))
      return m_ecalClusters.at(clusterId);
    else if (Identifier::isHcal(clusterId))
      return m_hcalClusters.at(clusterId);
    throw std::out_of_range("Cluster not found");
  }
  
  SimParticle TestSimulator::makeSimParticle(int pdgid, double charge, const TLorentzVector& tlv,
                                         const TVector3& vertex) const {
    
    double field = m_detector.field()->getMagnitude();
    auto simParticle=SimParticle( pdgid, charge, tlv, vertex, field, 's');
    return std::move(simParticle);
  }
  
  SimParticle TestSimulator::makeSimParticle(int pdgid, double charge, double theta, double phi, double energy,
                                         const TVector3& vertex) const {
    double mass = ParticlePData::particleMass(pdgid);
    double momentum = sqrt(pow(energy, 2) - pow(mass, 2));
    double costheta = cos(theta);
    double sintheta = sin(theta);
    double cosphi = cos(phi);
    double sinphi = sin(phi);
    TLorentzVector p4(momentum * sintheta * cosphi, momentum * sintheta * sinphi, momentum * costheta, energy);
    return makeSimParticle(pdgid, charge, p4, vertex);
  }
  
  SimParticle& TestSimulator::storeSimParticle(SimParticle&& simParticle, IdType parentId) {
    auto id = simParticle.id();
    m_particles.emplace(id, std::move(simParticle));
    addNode(id, parentId);  // add node to history graph
    return m_particles[id];
  }
  
  SimParticle& TestSimulator::addGunParticle(int pdgid, double charge, double thetamin, double thetamax, double ptmin,
                                         double ptmax, const TVector3& vertex) {
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
  
  Cluster TestSimulator::makeCluster(const SimParticle& ptc, papas::Layer layer, double fraction, double csize, char subtype) const {
    double energy = ptc.p4().E() * fraction;
    papas::Position clayer = m_detector.calorimeter(layer)->volumeCylinder().innerLayer();
    TVector3 pos = ptc.pathPosition(clayer);
    if (csize == -1.) {  // ie value not provided
      csize = m_detector.calorimeter(layer)->clusterSize(ptc);
    }
    auto cluster= Cluster(energy, pos, csize, Identifier::itemType(layer), subtype);
    return cluster;
  }
  
  const Cluster& TestSimulator::storeEcalCluster(Cluster&& cluster, IdType parentId) {
    IdType id = cluster.id();
    addNode(id, parentId);
    PDebug::write("Made {}", cluster);
    m_ecalClusters.emplace(id, std::move(cluster));
    return m_ecalClusters[id];
  }
  
  const Cluster& TestSimulator::storeHcalCluster(Cluster&& cluster, IdType parentId) {
    IdType id = cluster.id();
    addNode(id, parentId);
    PDebug::write("Made {}", cluster);
    m_hcalClusters.emplace(id, std::move(cluster));
    return m_hcalClusters[id];
  }
  Cluster TestSimulator::smearCluster(const Cluster& parent, papas::Layer detectorLayer) {
    // detectorLayer will be used to choose which detector layer is used for energy resolution etc.
    // NB It is not always the same layer as the new smeared cluster
    if (detectorLayer == papas::Layer::kNone) detectorLayer = Identifier::layer(parent.id());  // default to same layer as cluster
    std::shared_ptr<const Calorimeter> sp_calorimeter = m_detector.calorimeter(detectorLayer);
    double energyresolution = sp_calorimeter->energyResolution(parent.energy(), parent.eta());
    double response = sp_calorimeter->energyResponse(parent.energy(), parent.eta());
    double energy = parent.energy() * randomgen::RandNormal(response, energyresolution).next();
    energy = fmax(0., energy); // energy always positive
    auto cluster = Cluster(energy, parent.position(), parent.size(), Identifier::itemType(parent.id()), 's');
    PDebug::write("Made Smeared{}", cluster);
    return cluster;
  }
  
  bool TestSimulator::acceptSmearedCluster(const Cluster& smearedCluster, papas::Layer detectorLayer,
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
  
  const Cluster& TestSimulator::storeSmearedCluster(Cluster&& smearedCluster, IdType parentId) {
    
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
  
  const Track& TestSimulator::storeTrack(Track&& track, IdType parentid) {
    IdType id = track.id();
    m_tracks.emplace(id, std::move(track));
    addNode(id, parentid);
    PDebug::write("Made {}", track);
    return m_tracks.at(id);
  }
  
  Track TestSimulator::smearTrack(const Track& track) const {
    double ptResolution = m_detector.tracker()->ptResolution(track);
    double scale_factor = randomgen::RandNormal(1, ptResolution).next();
    auto smeared = Track(track.p3() * scale_factor, track.charge(), track.path(),'s');
    PDebug::write("Made Smeared{}", smeared);
    return smeared;
  }
  
  const Track& TestSimulator::storeSmearedTrack(Track&& smearedtrack, IdType parentid) {
    auto id = smearedtrack.id();
    addNode(id, parentid);
    m_smearedTracks.emplace(smearedtrack.id(), std::move(smearedtrack));
    return m_smearedTracks[id];
  }
  
  bool TestSimulator::acceptSmearedTrack(const Track& smearedtrack, bool accept) const {
    // decide whether the smearedTrack is detected
    if (m_detector.tracker()->acceptance(smearedtrack) || accept) {
      return true;
    } else {
      PDebug::write("Rejected Smeared{}", smearedtrack);
      return false;
    }
  }
  
  void TestSimulator::addNode(IdType newid, const IdType parentid) {
    // add the new node into the set of all nodes
    PFNode node{newid};
    m_history.emplace(newid, std::move(node));
    if (parentid) {
      PFNode& parent = m_history[parentid];
      PFNode& child = m_history[newid];
      parent.addChild(child);
    }
  }
  
  void TestSimulator::clear() {
    m_ecalClusters.clear();
    m_hcalClusters.clear();
    m_smearedEcalClusters.clear();
    m_smearedHcalClusters.clear();
    m_tracks.clear();
    m_smearedTracks.clear();
    m_particles.clear();
  }
  
  std::shared_ptr<const DetectorElement> TestSimulator::elem(papas::Layer layer) const { return m_detector.element(layer); }
  
  void TestSimulator::testing() {
    DAG::BFSVisitor<PFNode> bfs;
    for (auto p : m_particles) {
      std::cout << "Connected to " << p.first << std::endl;
      auto res = bfs.traverseUndirected(m_history[p.first]);
      for (auto r : res)
        std::cout << "  " << r->value() << ": " << Identifier::itemType(r->value()) << std::endl;
    }
  }
  
  Ids TestSimulator::linkedIds(IdType nodeid) const {
    DAG::BFSVisitor<PFNode> bfs;
    Ids foundids;
    foundids.reserve(1000);  // TODO how
    auto res = bfs.traverseUndirected(m_history.at(nodeid));
    for (auto r : res) {
      foundids.push_back(r->value());
    }
    return foundids;
  }
  
  /*
   Ids TestSimulator::linkedEcalSmearedClusterIds(long nodeid) const {
   return matchingIds(nodeid,
   Identifier::DataType::kCluster,
   papas::Layer::kEcal,
   papas::SubType::SMEARED,
   papas::enumSource::SIMULATION);
   }
   
   Ids  TestSimulator::linkedRawTrackIds(long nodeid) const {
   return matchingIds(nodeid,
   Identifier::DataType::kTrack,
   papas::Layer::kNone,
   papas::SubType::RAW,
   papas::enumSource::SIMULATION);
   }
   
   Ids TestSimulator::linkedSmearedTrackIds(long nodeid) const {
   return matchingIds(nodeid,
   Identifier::DataType::kTrack,
   papas::Layer::kNone,
   papas::SubType::SMEARED,
   papas::enumSource::SIMULATION);
   }
   
   Ids TestSimulator::linkedParticleIds(long nodeid) const {
   return matchingIds(nodeid,
   Identifier::DataType::kParticle,
   papas::Layer::kNone,
   papas::SubType::RAW,
   papas::enumSource::SIMULATION);
   }
   
   Ids TestSimulator::parentParticleIds(long nodeid) const {
   return getMatchingParentIds(nodeid,
   Identifier::DataType::kParticle,
   papas::Layer::kNone,
   papas::SubType::RAW,
   papas::enumSource::SIMULATION);
   }*/
  
  /*
   Ids TestSimulator::matchingIds(long nodeid, Identifier::DataType datatype, papas::Layer layer, papas::SubType type,
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
   
   Ids TestSimulator::getMatchingParentIds(long nodeid, Identifier::DataType datatype, papas::Layer layer, papas::SubType
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

