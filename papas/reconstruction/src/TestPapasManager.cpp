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

  auto ecalClusters = new Clusters();
  auto hcalClusters = new Clusters();
  auto smearedEcalClusters = new  Clusters();
  auto smearedHcalClusters =new  Clusters();
  auto tracks =new  Tracks();
  auto smearedTracks =new  Tracks();
  auto simParticles = new SimParticles();
  auto history =new  Nodes();

  // keep track of what is owned
  /*m_ownedClusters.emplace_back();
  auto & ecalClusters = m_ownedClusters.at(m_ownedClusters.size()-1);
  m_ownedClusters.emplace_back();
  auto & smearedEcalClusters = m_ownedClusters.at(m_ownedClusters.size()-1);
  std::cout <<m_ownedClusters.size() <<std::endl;
  m_ownedClusters.emplace_back();
  auto & hcalClusters = m_ownedClusters.at(m_ownedClusters.size()-1);
  m_ownedClusters.emplace_back();
  auto & smearedHcalClusters = m_ownedClusters.at(m_ownedClusters.size()-1);

  m_ownedTracks.emplace_back();
  auto & tracks = m_ownedTracks.at(m_ownedTracks.size()-1);
  m_ownedTracks.emplace_back();
  auto & smearedTracks = m_ownedTracks.at(m_ownedTracks.size()-1);
  m_ownedParticles.emplace_back();
  auto & simParticles = m_ownedParticles.at(m_ownedParticles.size()-1);

  m_ownedHistory.emplace_back();
  auto & history = m_ownedHistory.at(m_ownedHistory.size()-1);*/

  /*m_ownedHistory.push_back(std::move(history));
  m_ownedTracks.push_back(std::move(tracks));
  m_ownedTracks.push_back(std::move(smearedTracks));
  m_ownedParticles.push_back(std::move(simParticles));
  m_ownedHistory.push_back(std::move(history));*/

  auto simulator = TestSimulator(particles, m_detector, *ecalClusters,
                              *hcalClusters,  *smearedEcalClusters,  *smearedHcalClusters,
                              *tracks,  *smearedTracks,  *simParticles,  *history);
  m_papasEvent.addCollection( *ecalClusters);
  m_papasEvent.addCollection( *hcalClusters);
  m_papasEvent.addCollection( *smearedEcalClusters);
  m_papasEvent.addCollection ( *smearedHcalClusters);
  
 /* auto simulator = TestSimulator(particles, m_detector, m_ecalClusters,
                                 m_hcalClusters,  m_smearedEcalClusters,  m_smearedHcalClusters,
                                 m_tracks,  m_smearedTracks,  m_simParticles,  m_history);*/


  /*m_papasEvent.addCollection( m_ecalClusters);
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

/*void TestPapasManager::mergeClusters(std::string typeAndSubtype) {
  Ruler ruler;
  auto mergedEClusters = new Clusters(); // will be owned and managed by TestPapasManager
  auto history = new Nodes();

  //keep track of what is owned
  m_ownedClusters.push_back(mergedEClusters);
  m_ownedHistory.push_back(history);

  auto ecalmerger = TestMergedClusterBuilder(m_papasEvent, typeAndSubtype, ruler, *mergedEClusters, *history);
  m_papasEvent.addCollection(*mergedEClusters);
  m_papasEvent.addCollection(*history);

}*/

void TestPapasManager::mergeClusters(const std::string& typeAndSubtype) {
  Ruler ruler;
  auto mergedEClusters = new Clusters();  // will be owned and managed by TestPapasManager
  auto history = new Nodes();

  // keep track of what is owned
  //m_ownedClusters.push_back(std::move(m_mergedEClusters));
  //m_ownedHistory.push_back(std::move(m_history2));

  auto ecalmerger = TestMergedClusterBuilder(m_papasEvent, typeAndSubtype, ruler, *mergedEClusters, *history);
  m_papasEvent.addCollection(*mergedEClusters);
  //m_papasEvent.addHistory(m_history2);
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
}

}  // end namespace papas