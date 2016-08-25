//
//  PythiaConnector.cpp
//  papas
//
//  Created by Alice Robson on 06/07/16.
//
//

#include "PythiaConnector.h"
#include "PFReconstructor.h"
#include "PapasManager.h"

#include "datamodel/EventInfoCollection.h"
#include "datamodel/ParticleCollection.h"
#include "utilities/ParticleUtils.h"

#include "PDebug.h"
#include "PParticle.h"
#include "Simulator.h"

#include <exception>

// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TROOT.h"
#include "TTree.h"

PythiaConnector::PythiaConnector(const char* fname) : m_store(podio::EventStore()), m_reader(podio::ROOTReader()) {

  try {
     m_reader.openFile(fname);
  } catch (std::exception& err) {
    std::cerr << err.what() << ". Quitting." << std::endl;
    exit(1);
  } catch(...) {
    std::cerr << "Error occured: quitting"<< std::endl;
    exit(1);
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
    auto p4 = ptc.Core().P4;
    tlv.SetXYZM(p4.Px, p4.Py, p4.Pz, p4.Mass);
    int pdgid = ptc.Core().Type;

    papas::IdType id = papas::Id::makeParticleId();
    papas::Particle particle{id, pdgid, (double)ptc.Core().Charge, tlv, 1};  // make every single one into a particle
    // so as to match python approach (for now)
    // otherwise ids do not align

    if (ptc.Core().Status == 1) {  // only stable ones

      if (tlv.Pt() > 1e-5 && (abs(pdgid) != 12) && (abs(pdgid) != 14) && (abs(pdgid) != 16)) {

        particles.emplace(id, std::move(particle));
        papas::PDebug::write("Selected Papas{}", particles.at(id));
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
    papasManager.simulateEvent(std::move(papasparticles));
    papasManager.reconstructEvent();
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
  evinfo.Number(eventno);
  evinfocoll.push_back(evinfo);
  for (const auto& p : particles) {
    auto ptc = fcc::Particle();
    ptc.Core().Type = p.second.pdgId();
    auto& p4 = ptc.Core().P4;
    p4.Px = p.second.p4().Px();
    p4.Py = p.second.p4().Py();
    p4.Pz = p.second.p4().Pz();
    p4.Mass = p.second.p4().M();
    ptc.Core().Status = 1;
    ptc.Core().Charge = p.second.charge();
    pcoll.push_back(ptc);
  }
  writer.writeEvent();
  m_store.clearCollections();
  writer.finish();
}
