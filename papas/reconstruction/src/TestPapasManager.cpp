//
//  TestPapasManager.cpp
//  papas
//
//  Created by Alice Robson on 14/12/16.
//
//


#include "papas/datatypes/Identifier.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/display/AliceDisplay.h"
#include "papas/graphtools/EventRuler.h"
#include "papas/simulation/TestSimulator.h"
#include "papas/reconstruction/MergedClusterBuilder.h"
#include "papas/reconstruction/PFBlockBuilder.h"
#include "papas/reconstruction/PFReconstructor.h"
#include "papas/reconstruction/TestMergedClusterBuilder.h"
#include "papas/reconstruction/TestPapasManager.h"
#include "papas/utility/PDebug.h"

namespace papas {

TestPapasManager::TestPapasManager(Detector& detector) : m_detector(detector), m_papasEvent() {}

void TestPapasManager::simulate(const Particles& particles) {

  auto& ecalClusters = createClusters();
  auto& hcalClusters = createClusters();
  auto& smearedEcalClusters = createClusters();
  auto& smearedHcalClusters = createClusters( );
  auto& tracks =createTracks();
  auto& smearedTracks =createTracks();
  auto& history = createHistory();
  auto& simParticles = createParticles();
  
  auto simulator = TestSimulator(particles, m_detector, ecalClusters,
                              hcalClusters,  smearedEcalClusters,  smearedHcalClusters,
                              tracks,  smearedTracks,  simParticles,  history);
  m_papasEvent.addCollection( ecalClusters);
  m_papasEvent.addCollection( hcalClusters);
  m_papasEvent.addCollection( smearedEcalClusters);
  m_papasEvent.addCollection ( smearedHcalClusters);
  m_papasEvent.addCollection ( tracks);
  m_papasEvent.addCollection ( smearedTracks);
  m_papasEvent.addCollection ( simParticles);
  m_papasEvent.addHistory ( history);

  /*auto simulator = TestSimulator(particles, m_detector, m_ecalClusters,
                                 m_hcalClusters,  m_smearedEcalClusters,  m_smearedHcalClusters,
                                 m_tracks,  m_smearedTracks,  m_simParticles,  m_history);


  m_papasEvent.addCollection( m_ecalClusters);
  m_papasEvent.addCollection( m_hcalClusters);
  m_papasEvent.addCollection( m_smearedEcalClusters);
  m_papasEvent.addCollection ( m_smearedHcalClusters);*/
  // m_papasEvent.addCollection( m_tracks);
  //m_papasEvent.addCollection( m_smearedTracks);
  //m_papasEvent.addHistory( m_history);

  // TODO need to make several changes incl rename SimParticles and possibly particles
  // also make incoming particles free of Uniquedid to match python changes
  // order the particles according to id
  /*std::vector<IdType> ids;
  for (auto kv : particles) {
    ids.push_back(kv.first);
  }
#if WITHSORT
  bool operator()(IdType i, IdType j);
  std::sort(ids.begin(), ids.end(),
            [&](IdType i, IdType j) { return (m_particles.at(i).e() > m_particles.at(j).e()); });
#endif

  for (const auto& id : ids) {
    //m_history.emplace(id, std::move(PFNode(id)));  ///< insert the raw particle ids into the history
    m_simulator.simulateParticle(m_particles.at(id), id);
  }*/
}


void TestPapasManager::mergeClusters(const std::string& typeAndSubtype) {
  Ruler ruler;
  auto& mergedClusters = createClusters();
  auto& history = createHistory();
  auto ecalmerger = TestMergedClusterBuilder(m_papasEvent, typeAndSubtype, ruler, mergedClusters, history);
  m_papasEvent.addCollection( mergedClusters);
  m_papasEvent.addHistory(history);
}

/*void TestPapasManager::blockBuild(const std::string& ecalTypeAndSubtype, const std::string& hcalTypeAndSubtype, const
std::string& trackTypeAndSubtype) {
  auto blocks = Blocks();
  auto history =  Nodes();
  m_ownedBlocks.push_back(std::move(blocks));
  m_ownedHistory.push_back(std::move(history));

  auto BlockBuilder = BlockBuilder(m_papasEvent, ecalTypeAndSubtype, hcalTypeAndSubtype, trackTypeAndSubtype,
blocks,history);
  m_papasEvent.addCollection(blocks);
  m_papasEvent.addHistory(history);

}


void TestPapasManager::reconstruct(std::string blockTypeAndSubtype) {
  auto recParticles = new SimParticles(); //todo change this name
  auto pfReconstructor = PFReconstructor(m_papasEvent, blockTypeAndSubtype, *recparticles);
  pfReconstructor.reconstruct();
  m_papasEvent.addCollection(*recparticles);

}*/

void TestPapasManager::clear() {

  m_papasEvent.clear();

  for (auto c : m_ownedHistory)
    c.clear();
  m_ownedHistory.clear();

  Identifier::reset();  // todo move to papasevent?
  for (auto c : m_ownedClusters)
    c.clear();
  m_ownedClusters.clear();
  
  for (auto c : m_ownedParticles)
    c.clear();
  m_ownedParticles.clear();

}
  
  Clusters& TestPapasManager::createClusters() {
    auto clusters = Clusters();
    m_ownedClusters.emplace_back(std::move(clusters));
    return m_ownedClusters.back();
  }
  
  Tracks& TestPapasManager::createTracks() {
    m_ownedTracks.emplace_back(Tracks());
    return m_ownedTracks.back();
  }
Blocks& TestPapasManager::createBlocks() {
    m_ownedBlocks.emplace_back(Blocks());
    return m_ownedBlocks.back();
  }
  SimParticles& TestPapasManager::createParticles() {
    m_ownedParticles.emplace_back(SimParticles());
    return m_ownedParticles.back();
  }
  Nodes& TestPapasManager::createHistory() {
    m_ownedHistory.emplace_back(Nodes());
    return m_ownedHistory.back();
  }

}  // end namespace papas