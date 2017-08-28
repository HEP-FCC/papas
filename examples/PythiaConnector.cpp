//
//  PythiaConnector.cpp
//  papas
//
//  Created by Alice Robson on 06/07/16.
//
//

#include "PythiaConnector.h"
#include "papas/reconstruction/PFReconstructor.h"
#include "papas/reconstruction/PapasManager.h"
#include "papas/reconstruction/PapasManager.h"

//#include "datamodel/CaloClusterCollection.h"
#include "datamodel/EventInfoCollection.h"
#include "datamodel/ParticleCollection.h"
#include "utilities/ParticleUtils.h"

#include "papas/datatypes/Helix.h"
#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Path.h"
#include "papas/detectors/Detector.h"
#include "papas/detectors/Field.h"
#include "papas/display/PFApp.h"
#include "papas/simulation/Simulator.h"
#include "papas/utility/Logger.h"
#include "papas/utility/PDebug.h"

#include <exception>
#include <string>
#include <sys/stat.h>

// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TROOT.h"
#include "TTree.h"

PythiaConnector::PythiaConnector(const char* fname) : m_store(podio::EventStore()), m_reader(podio::ROOTReader()) {

  // check file exists before attempting to open (this is supposed to be a fast way to check)
  if (access(fname, F_OK) != -1) {
    m_reader.openFile(fname);
  } else {
    throw "File not found";
  }
  m_store.setReader(&m_reader);
}

void PythiaConnector::makePapasParticlesFromGeneratedParticles(const fcc::MCParticleCollection* ptcs,
                                                               papas::Particles& particles,
                                                               const papas::Detector& detector) {
  // turns pythia particles into Papas particles and lodges them in the history
  TLorentzVector tlv;
  int countp = 0;

  // Sort particles in order of decreasing energy
  std::list<fcc::ConstMCParticle> sortPtcs;
  for (const auto& p : *ptcs) {
    sortPtcs.push_back(p);
  }
  sortPtcs.sort([](const fcc::ConstMCParticle& a, const fcc::ConstMCParticle& b) {
    auto p4 = a.p4();
    TLorentzVector tlv;
    tlv.SetXYZM(p4.px, p4.py, p4.pz, p4.mass);
    TLorentzVector tlv2;
    p4 = b.p4();
    tlv2.SetXYZM(p4.px, p4.py, p4.pz, p4.mass);
    return tlv.E() > tlv2.E();
  });

  for (const auto& ptc : sortPtcs) {
    countp += 1;
    auto p4 = ptc.core().p4;
    tlv.SetXYZM(p4.px, p4.py, p4.pz, p4.mass);
    int pdgid = ptc.core().pdgId;
    TVector3 startVertex = TVector3(0, 0, 0);
    if (ptc.startVertex().isAvailable()) {
      startVertex = TVector3(ptc.startVertex().x() * 1e-3, ptc.startVertex().y() * 1e-3, ptc.startVertex().z() * 1e-3);
    }
    if (ptc.core().status == 1) {  // only stable ones

      if (tlv.Pt() > 1e-5 && (abs(pdgid) != 12) && (abs(pdgid) != 14) && (abs(pdgid) != 16)) {
        papas::Particle particle(pdgid, (double)ptc.core().charge, tlv, particles.size(), 's', startVertex,
                                 ptc.core().status);
        // set the particles papas path (allows particles to be const when passed to simulator)
        std::shared_ptr<papas::Path> ppath;
        if (fabs(particle.charge()) < 0.5) {
          ppath = std::make_shared<papas::Path>(particle.p4(), particle.startVertex(), particle.charge());
        } else {
          ppath = std::make_shared<papas::Helix>(particle.p4(), particle.startVertex(), particle.charge(),
                                                 detector.field()->getMagnitude());
        }
        particle.setPath(ppath);
        particles.emplace(particle.id(), particle);
        papas::PDebug::write("Made {}", particle);
      }
    }
  }
}

void PythiaConnector::processEvent(unsigned int eventNo, papas::PapasManager& papasManager) {
  // make a papas particle collection from the next event
  // then run simulate and reconstruct
  m_reader.goToEvent(eventNo);
  papasManager.setEventNo(eventNo);
  const fcc::MCParticleCollection* ptcs;
  if (m_store.get("GenParticle", ptcs)) {
    try {
      papasManager.clear();
      papas::Particles& genParticles = papasManager.createParticles();
      makePapasParticlesFromGeneratedParticles(ptcs, genParticles, papasManager.detector());
      papasManager.addParticles(genParticles);
      papasManager.simulate('s');
      papasManager.mergeClusters("es");
      papasManager.mergeClusters("hs");
      papasManager.buildBlocks('m', 'm', 's');
      papasManager.simplifyBlocks('r');
      papasManager.reconstruct('s');
    } catch (std::string message) {
      std::string outstring = string_format("An error occurred and event was discarsed. Event no: %i : %s", eventNo, message);
      PAPASLOG_ERROR(outstring);    }
    m_store.clear();
  }
  m_reader.endOfEvent();
}

void PythiaConnector::processEvent(unsigned int eventNo, std::shared_ptr<papas::PapasManager> papasManager) {
  // make a papas particle collection from the next event
  // then run simulate and reconstruct
  m_reader.goToEvent(eventNo);
  papasManager->setEventNo(eventNo);
  const fcc::MCParticleCollection* ptcs;
  if (m_store.get("GenParticle", ptcs)) {
    try {
      papasManager->clear();
      papas::Particles& genParticles = papasManager->createParticles();
      makePapasParticlesFromGeneratedParticles(ptcs, genParticles, papasManager->detector());
      papasManager->addParticles(genParticles);
      papasManager->simulate('s');
      papasManager->mergeClusters("es");
      papasManager->mergeClusters("hs");
      papasManager->buildBlocks('m', 'm', 's');
      papasManager->simplifyBlocks('r');
      papasManager->reconstruct('s');
    } catch (std::string message) {
      std::string outstring = string_format("An error occurred and event was discarsed. Event no: %i : %s", eventNo, message);
      PAPASLOG_ERROR(outstring);
    }
    m_store.clear();
  }
  m_reader.endOfEvent();
}




void PythiaConnector::displayEvent(const papas::PapasManager& papasManager) {
  papas::PFApp myApp{};  // I think this should turn into a PapasManager member
  myApp.display(papasManager.event(), papasManager.detector());
  // gSystem->ProcessEvents();
}

void PythiaConnector::writeParticlesROOT(const char* fname, const papas::Particles& particles) {

  podio::ROOTWriter writer(fname, &m_store);

  unsigned int nevents = 10;
  unsigned int eventno = 0;

  auto& evinfocoll = m_store.create<fcc::EventInfoCollection>("evtinfo");
  auto& pcoll = m_store.create<fcc::ParticleCollection>("GenParticle");

  writer.registerForWrite("evtinfo");
  writer.registerForWrite("GenParticle");

  auto evinfo = fcc::EventInfo();  // evinfocoll.create();
  evinfo.number(eventno);
  evinfocoll.push_back(evinfo);
  for (const auto& p : particles) {
    auto ptc = fcc::Particle();
    ptc.core().pdgId = p.second.pdgId();
    auto& p4 = ptc.core().p4;
    p4.px = p.second.p4().Px();
    p4.py = p.second.p4().Py();
    p4.pz = p.second.p4().Pz();
    p4.mass = p.second.p4().M();
    ptc.core().status = 1;
    ptc.core().charge = p.second.charge();
    pcoll.push_back(ptc);
  }
  writer.writeEvent();
  m_store.clearCollections();
  writer.finish();
}

/*
void PythiaConnector::writeClustersROOT(const char* fname, const papas::Clusters& clusters) {

  podio::ROOTWriter writer(fname, &m_store);
  unsigned int nevents = 1;
  unsigned int eventno = 0;
  auto& evinfocoll = m_store.create<fcc::EventInfoCollection>("evtinfo");
  auto& ccoll = m_store.create<fcc::CaloClusterCollection>("Cluster");

  writer.registerForWrite("evtinfo");
  writer.registerForWrite("Cluster");

  auto evinfo = fcc::EventInfo();  // evinfocoll.create();
  evinfo.number(eventno);
  evinfocoll.push_back(evinfo);
  AddClustersToEDM(clusters, ccoll);

  auto checkClusters = ConvertClustersToPapas(ccoll,
                                              0,  // size or 0 for merged
                                              papas::IdCoder::ItemType::kEcalCluster,
                                              's');

  writer.writeEvent();
  m_store.clearCollections();
  writer.finish();
}

papas::Clusters PythiaConnector::ConvertClustersToPapas(const fcc::CaloClusterCollection& fccClusters,
                                                        float size,
                                                        papas::IdCoder::ItemType itemtype,
                                                        char subtype) const {
  papas::Clusters clusters;
  for (const auto& c : fccClusters) {
    const auto position = c.core().position;
    const auto energy = c.core().energy;
    papas::Cluster cluster(energy, TVector3(position.x, position.y, position.z), size, clusters.size(), itemtype,
                           subtype);
    clusters.emplace(cluster.id(), std::move(cluster));
  }
  return clusters;
}

void PythiaConnector::AddClustersToEDM(const papas::Clusters& papasClusters, fcc::CaloClusterCollection& fccClusters) {
  for (const auto& c : papasClusters) {
    auto clust = fccClusters.create();
    clust.core().energy = c.second.energy();
    auto& p3 = clust.core().position;
    p3.x = c.second.position().X();
    p3.y = c.second.position().Y();
    p3.z = c.second.position().Z();
  }
}
*/
/*void PythiaConnector::readClustersROOT(unsigned int eventNo, papas::PapasManager& papasManager) {

  const fcc::ParticleCollection* ptcs(nullptr);
  if (m_store.get("GenParticle", ptcs)) {
    papas::Particles papasparticles = makePapasClustersFromCaloClusts(ptcs);
    papasManager.storeParticles(std::move(papasparticles));
    papasManager.simulateEvent();
    papasManager.mergeClusters();
    papasManager.reconstructEvent();
    m_store.clear();
  }

  m_reader.endOfEvent();
}*/
