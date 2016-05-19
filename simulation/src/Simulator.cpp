//
//  Created by Alice Robson on 09/11/15.
//
//
#include <iostream> //TODOAJR remove
#include "Simulator.h"
#include "CMS.h"
#include "Id.h"
#include "Definitions.h"
#include "Cluster.h"
#include "Track.h"
#include "PFParticle.h"
#include "ParticlePData.h"
#include "Path.h"


namespace papas {

//using papas::Layer;

Simulator::Simulator(const Detector& d) :
    m_detector(d),
    m_propStraight(),
    m_propHelix(d.field()->getMagnitude())
{
  //TODO think about sizing
  int isize=1000;
  m_ecalClusters.reserve(isize);
  m_hcalClusters.reserve(isize);
  m_smearedEcalClusters.reserve(isize);
  m_smearedHcalClusters.reserve(isize);
  m_particles.reserve(isize);
  m_tracks.reserve(isize);
  m_smearedTracks.reserve(isize);
  m_nodes.reserve(isize);
}

void  Simulator::simulatePhoton(PFParticle& ptc)
{
  auto ecal_sp=m_detector.ecal();
  
  //find where it meets the Ecal inner cylinder
  // make and smear the cluster
  propagate(ptc, ecal_sp->volumeCylinder().inner());
  Id::Type ecalId = addEcalCluster(ptc);
  addSmearedCluster(ecalId);
}

void Simulator::simulateHadron(PFParticle& ptc) {
  auto ecal_sp = m_detector.ecal();
  auto hcal_sp = m_detector.hcal();
  auto field_sp = m_detector.field();
  double fracEcal = 0.;  // TODO ask Colin

  // make a track
  const Track& track = addTrack(ptc);
  addSmearedTrack(track);

  // find where it meets the inner Ecal cyclinder
  propagate(ptc, ecal_sp->volumeCylinder().inner());
  double pathLength = ecal_sp->material().pathLength(ptc.isElectroMagnetic());

  if (pathLength < std::numeric_limits<double>::max()) {

    /// ecal path length can be infinite in case the ecal
    /// has lambda_I = 0 (fully transparent to hadrons)
    std::shared_ptr<Path> path = ptc.path();
    double timeEcalInner = path->timeAtZ(path->namedPoint(papas::Position::kEcalIn).Z());
    double deltaT = path->deltaT(pathLength);
    double timeDecay = timeEcalInner + deltaT;
    TVector3 pointDecay = path->pointAtTime(timeDecay);
    path->addPoint(papas::Position::kEcalDecay, pointDecay);
    if (ecal_sp->volumeCylinder().Contains(pointDecay)) {
      // TODO reinstate after testingdouble fracEcal = randomgen::randomUniform(0., 0.7).next(); // could also have a
      // static member number generator
      fracEcal = 0.35;
      Id::Type ecalId = addEcalCluster(ptc, track.id(), fracEcal);
      // For now, using the hcal resolution and acceptance for hadronic cluster
      // in the Ecal. That's not a bug!
      addSmearedCluster(ecalId);
    }
  }

  // now find where it reaches into HCAL
  propagate(ptc, hcal_sp->volumeCylinder().inner());
  Id::Type hcalId = addHcalCluster(ptc, ptc.id(), 1 - fracEcal);
  addSmearedCluster(hcalId);
}

void Simulator::simulateNeutrino(PFParticle& ptc) {
  
}

void  Simulator::propagate(PFParticle& ptc, const SurfaceCylinder& cylinder) {
  bool isNeutral = fabs(ptc.charge())<0.5; //TODO ask colin why not zero
  if (isNeutral)
    m_propStraight.propagateOne(ptc, cylinder);
  else
    m_propHelix.propagateOne(ptc, cylinder);
}

const Cluster& Simulator::cluster(Id::Type clusterId) const {
  if (Id::isEcal(clusterId))
    return m_ecalClusters.at(clusterId);
  else if (Id::isHcal(clusterId))
    return m_hcalClusters.at(clusterId);
  
}

PFParticle& Simulator::addParticle(int pdgid, TLorentzVector tlv, TVector3 vertex)
{
  double field = m_detector.field()->getMagnitude();
  Id::Type uniqueid = Id::makeParticleId();
  m_particles.emplace(uniqueid, PFParticle{uniqueid, pdgid, tlv, vertex, field});
  addNode(uniqueid); //add node to history graph
  return m_particles[uniqueid];
}

PFParticle& Simulator::addParticle(int pdgid, double theta, double phi, double energy, TVector3 vertex)
{
  TLorentzVector tlv = makeTLorentzVector(pdgid, theta, phi, energy);
  return addParticle(pdgid, tlv, vertex);
}


TLorentzVector Simulator::makeTLorentzVector(int pdgid, double theta, double phi, double energy)
{
  double mass = ParticlePData::particleMass(pdgid);
  double momentum = sqrt(pow(energy, 2) - pow(mass, 2));
  double costheta = cos(theta);
  double sintheta = sin(theta);
  double cosphi = cos(phi);
  double sinphi = sin(phi);
  TLorentzVector p4(momentum * sintheta * cosphi,
                    momentum * sintheta * sinphi,
                    momentum * costheta,
                    energy);
  /*std::cout << "TLV " << p4.X() << " " << p4.Y() << " " << p4.Z() << " " <<
   p4.Et() << " ";
   std::cout << "energy " << energy << " mom " << momentum << " " << costheta <<
   " " << cosphi <<
   " " << sintheta << " ";*/
  return p4;
}                              


Id::Type Simulator::addEcalCluster(PFParticle& ptc,Id::Type parentid,double fraction, double csize)
{
  Cluster cluster = makeCluster(ptc, parentid, papas::Layer::kEcal, fraction, csize);
  m_ecalClusters.emplace(cluster.id(), std::move(cluster));
  return cluster.id();
}

Id::Type Simulator::addHcalCluster(PFParticle& ptc,Id::Type parentid,double fraction, double csize)
{
  Cluster cluster = makeCluster(ptc, parentid, papas::Layer::kHcal, fraction, csize);
  m_hcalClusters.emplace(cluster.id(), std::move(cluster));
  return cluster.id();
}


  Cluster Simulator::makeCluster(PFParticle& ptc, Id::Type parentid,papas::Layer layer, double fraction, double csize)
{
  if (!parentid) {
    parentid = ptc.id();
  }
  Id::Type clusterid;
  if (layer==papas::Layer::kEcal) {
    clusterid = Id::makeEcalId();
  } else {
    clusterid = Id::makeHcalId();
  }
  double energy = ptc.p4().E() * fraction;
  
  //TODO change string to ENUM
  papas::Position clayer = m_detector.calorimeter(layer)->volumeCylinder().innerLayer();
  TVector3 pos = ptc.pathPosition(clayer); //assume path already set in particle
  
  if (csize == 0) { //or could make the defalt value -1?? check Colin
    csize = m_detector.calorimeter(layer)->clusterSize(ptc);
  }
  
  const Cluster& cluster{energy, pos, csize, clusterid};
  addNode(clusterid, parentid); //a track may be the parent of a cluster
  return cluster; //check this defaults OK
}


Id::Type Simulator::addSmearedCluster(Id::Type parentClusterId)
{
  Cluster smeared=makeSmearedCluster(parentClusterId);
  auto layer = Id::layer(parentClusterId);
  
  if (m_detector.calorimeter(layer)->acceptance(smeared)) {
    addNode(smeared.id(), parentClusterId);
    if(layer ==papas::Layer::kEcal) {
      m_smearedEcalClusters.emplace(smeared.id(), std::move(smeared));
    }
    else
      m_smearedHcalClusters.emplace(smeared.id(), std::move(smeared));
    return smeared.id();
  }
  else {
    //cluster.erase(smeared.id());
    return parentClusterId;
  }
}


Cluster Simulator::makeSmearedCluster(Id::Type parentClusterId) //, double energyresolution )
{
  //create a new id
  auto itemType = Id::ItemType(parentClusterId);
  Id::Type newclusterid = Id::makeId(itemType);
  const Cluster& parent = cluster(parentClusterId);
  auto layer = Id::layer(parentClusterId);
  
  std::shared_ptr<const Calorimeter> sp_calorimeter = m_detector.calorimeter(layer);
  //double energyresolution = sp_calorimeter->energyResolution(parent.energy());
  //double energy = parent.energy() * randomgen::RandNormal(1, energyresolution).next();
  double energy = parent.energy() * 0.95;
  
  Cluster cluster = Cluster{ energy, parent.position(), parent.size(), newclusterid};
  return cluster;
}

const Track& Simulator::addTrack(PFParticle& ptc)
{
  Id::Type trackid = Id::makeTrackId();
  m_tracks.emplace(trackid, Track{ ptc.p3(), ptc.charge(), ptc.path(), trackid});
  addNode(trackid, ptc.id());
  return m_tracks.at(trackid); //check this defaults OK
}


Id::Type Simulator::addSmearedTrack( const Track& track, bool accept) {
  Id::Type smearedTrackId = Id::makeTrackId(papas::SubType::SMEARED);
  //double ptResolution = m_detector.tracker()->ptResolution(track);
  //TODO after testing double scale_factor = randomgen::RandNormal(1, ptResolution).next()
  double scale_factor = 1.1;
  Track smeared = Track{ track.p3() * scale_factor, track.charge(), track.path(), smearedTrackId};
  
  if (m_detector.tracker()->acceptance(smeared) || accept ) {
    addNode(smearedTrackId, track.id());
    m_smearedTracks.emplace(smearedTrackId, std::move(smeared));
  }
  return smearedTrackId;
}

void Simulator::addNode(Id::Type newid, const Id::Type parentid)
{
  //add the new node into the set of all nodes
  m_nodes[newid] = {newid};
  if (parentid) {
    PFNode& parent = m_nodes[parentid];
    PFNode& child = m_nodes[newid];
    parent.addChild(child);
  }
}

std::shared_ptr<const DetectorElement> Simulator::elem(papas::Layer layer) const {
  return m_detector.element(layer);
}

void Simulator::testing()
{
  DAG::BFSVisitor<PFNode> bfs;
  for (auto p : m_particles) {
    std::cout<< "Connected to "<<p.first<< std::endl;
    auto res =bfs.traverseUndirected(m_nodes[p.first]);
    for (auto r : res)
      std::cout<< "  "<<r->value()<<": "<<Id::itemType(r->value()) <<std::endl;
  }
}
/*
Ids Simulator::linkedEcalSmearedClusterIds(long nodeid) const {
  return getMatchingIds(nodeid,
                        Id::DataType::kCluster,
                        papas::Layer::kEcal,
                        papas::SubType::SMEARED,
                        papas::enumSource::SIMULATION);
}

Ids  Simulator::linkedRawTrackIds(long nodeid) const {
  return getMatchingIds(nodeid,
                        Id::DataType::kTrack,
                        papas::Layer::kNone,
                        papas::SubType::RAW,
                        papas::enumSource::SIMULATION);
}

Ids Simulator::linkedSmearedTrackIds(long nodeid) const {
  return getMatchingIds(nodeid,
                        Id::DataType::kTrack,
                        papas::Layer::kNone,
                        papas::SubType::SMEARED,
                        papas::enumSource::SIMULATION);
}

Ids Simulator::linkedParticleIds(long nodeid) const {
  return getMatchingIds(nodeid,
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

Ids Simulator::linkedIds(Id::Type nodeid) const {
  DAG::BFSVisitor<PFNode> bfs;
  Ids foundids;
  foundids.reserve(1000); //TODO how
  auto res = bfs.traverseUndirected(m_nodes.at(nodeid));
  for (auto r : res)
  {
    foundids.push_back(r->value());
  }
  return foundids;
}

  /*
Ids Simulator::getMatchingIds(long nodeid, Id::DataType datatype, papas::Layer layer, papas::SubType type, papas::enumSource source) const
{
  DAG::BFSVisitor<PFNode> bfs;
  Ids foundids;
  //foundids.reserve(1000); //TODO set sizes sensible.... how
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

Ids Simulator::getMatchingParentIds(long nodeid, Id::DataType datatype, papas::Layer layer, papas::SubType type,papas::enumSource source) const
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


} // end namespace papas




