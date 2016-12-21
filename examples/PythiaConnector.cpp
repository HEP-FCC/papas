
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
#include "papas/reconstruction/TestPapasManager.h"

#include "datamodel/EventInfoCollection.h"
#include "datamodel/ParticleCollection.h"
#include "datamodel/CaloClusterCollection.h"
#include "utilities/ParticleUtils.h"

#include "papas/utility/PDebug.h"
#include "papas/datatypes/Particle.h"
#include "papas/simulation/Simulator.h"

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

papas::Particles PythiaConnector::makePapasParticlesFromGeneratedParticles(const fcc::ParticleCollection* ptcs) {
  // turns pythia particles into Papas particles and lodges them in the history
  TLorentzVector tlv;
  papas::Particles particles;
  int countp = 0;
  for (const auto& ptc : *ptcs) {
    countp += 1;
    auto p4 = ptc.core().p4;
    tlv.SetXYZM(p4.px, p4.py, p4.pz, p4.mass);
    int pdgid = ptc.core().pdgId;

    //papas::IdType id = papas::Id::makeParticleId();
    auto particle = papas::Particle(pdgid, (double)ptc.core().charge, tlv,  'g');  // make every single one into a particle
    // so as to match python approach (for now)
    // otherwise ids do not align

    if (ptc.core().status == 1) {  // only stable ones

      if (tlv.Pt() > 1e-5 && (abs(pdgid) != 12) && (abs(pdgid) != 14) && (abs(pdgid) != 16)) {

        particles.emplace(particle.id(), std::move(particle));
        papas::PDebug::write("Selected Papas{}", particle);
      }
    }
  }
  // sort(particles.begin(), particles.end(),
  //     [](const papas::Particle& lhs, const papas::Particle& rhs) { return lhs.e() > rhs.e(); });
  return std::move(particles);
}

void PythiaConnector::processEvent(unsigned int eventNo, papas::PapasManager& papasManager) {
  // make a papas particle collection from the next event
  // then run simulate and reconstruct
  m_reader.goToEvent(eventNo);

  const fcc::ParticleCollection* ptcs(nullptr);
  if (m_store.get("GenParticle", ptcs)) {
    papas::Particles papasparticles = makePapasParticlesFromGeneratedParticles(ptcs);
    papasManager.storeParticles(std::move(papasparticles));
    papasManager.simulateEvent();
    papasManager.mergeClusters();
    papasManager.reconstructEvent();
    m_store.clear();
  }

  m_reader.endOfEvent();
}

void PythiaConnector::processEvent(unsigned int eventNo, papas::TestPapasManager& papasManager) {
  // make a papas particle collection from the next event
  // then run simulate and reconstruct
  m_reader.goToEvent(eventNo);
  papasManager.clear();
  const fcc::ParticleCollection* ptcs(nullptr);
  if (m_store.get("GenParticle", ptcs)) {
    papas::Particles papasparticles = makePapasParticlesFromGeneratedParticles(ptcs);
    papasManager.simulate(papasparticles);
    papasManager.mergeClusters("es");
    papasManager.mergeClusters("hs");
    //todo blockbuilder and reconstruct
    //papasManager.testMergeClusters();
    //papasManager.reconstructEvent();
    m_store.clear();
  }
  
  m_reader.endOfEvent();
}

void PythiaConnector::writeParticlesROOT(const char* fname, const papas::Particles& particles) {

  podio::ROOTWriter writer(fname, &m_store);

  unsigned int nevents = 1;
  unsigned int eventno = 0;

  auto& evinfocoll = m_store.create<fcc::EventInfoCollection>("evtinfo");
  auto& pcoll = m_store.create<fcc::ParticleCollection>("GenParticle");

  writer.registerForWrite<fcc::EventInfoCollection>("evtinfo");
  writer.registerForWrite<fcc::ParticleCollection>("GenParticle");

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

void PythiaConnector::writeClustersROOT(const char* fname, const papas::Clusters& clusters) {
  
  podio::ROOTWriter writer(fname, &m_store);
  unsigned int nevents = 1;
  unsigned int eventno = 0;
  auto& evinfocoll = m_store.create<fcc::EventInfoCollection>("evtinfo");
  auto& ccoll = m_store.create<fcc::CaloClusterCollection>("Cluster");
  
  writer.registerForWrite<fcc::EventInfoCollection>("evtinfo");
  writer.registerForWrite<fcc::CaloClusterCollection>("Cluster");
  
  auto evinfo = fcc::EventInfo();  // evinfocoll.create();
  evinfo.number(eventno);
  evinfocoll.push_back(evinfo);
  AddClustersToEDM(clusters, ccoll );
  
  auto checkClusters=ConvertClustersToPapas(ccoll,
                                            0, //size or 0 for merged
                                            papas::Identifier::ItemType::kEcalCluster,
                                            's' );
  
  writer.writeEvent();
  m_store.clearCollections();
  writer.finish();
}

papas::Clusters PythiaConnector::ConvertClustersToPapas(const fcc::CaloClusterCollection& fccClusters, float size, papas::Identifier::ItemType itemtype, char subtype) const {
  papas::Clusters clusters;
  for (const auto& c : fccClusters) {
    const auto position = c.core().position;
    const auto energy = c.core().energy;
    auto cluster = papas::Cluster(energy, TVector3(position.x, position.y, position.z), size, itemtype, subtype);
    clusters.emplace(cluster.id(), std::move(cluster));
  }
  return clusters;
}

void PythiaConnector::AddClustersToEDM(const papas::Clusters& papasClusters, fcc::CaloClusterCollection& fccClusters ) {
  for (const auto& c : papasClusters) {
    auto clust = fccClusters.create();
    clust.core().energy = c.second.energy();
    auto& p3 = clust.core().position;
    p3.x = c.second.position().X();
    p3.y = c.second.position().Y();
    p3.z = c.second.position().Z();
  }

}


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
