//
//  TestPapasManager.cpp
//  papas
//
//  Created by Alice Robson on 14/12/16.
//
//

#include <stdio.h>
//
//  TestPapasManager.cpp
//  papas
//
//  Created by Alice Robson on 22/06/16.
//
//

#include "papas/reconstruction/TestPapasManager.h"
#include "papas/display/AliceDisplay.h"
#include "papas/graphtools/EventRuler.h"
#include "papas/datatypes/Identifier.h"
#include "papas/reconstruction/MergedClusterBuilder.h"
#include "papas/reconstruction/TestMergedClusterBuilder.h"
#include "papas/utility/PDebug.h"
#include "papas/reconstruction/PFBlockBuilder.h"
#include "papas/reconstruction/PFReconstructor.h"

namespace papas {
  
  TestPapasManager::TestPapasManager(Detector& detector)
  : m_detector(detector), m_papasEvent() {}
  
  /*void TestPapasManager::simulate() {
    // order the particles according to id
    std::vector<IdType> ids;
    for (auto kv : m_particles) {
      ids.push_back(kv.first);
    }
#if WITHSORT
    std::sort(ids.begin(), ids.end(),
              [&](IdType i, IdType j) { return (m_particles.at(i).e() > m_particles.at(j).e()); });
#endif
    for (const auto& id : ids) {
      m_history.emplace(id, std::move(PFNode(id)));  ///< insert the raw particle ids into the history
      m_simulator.simulateParticle(m_particles.at(id), id);
    }
  }*/
  
  
  
  void TestPapasManager::mergeClusters(std::string typeAndSubtype) {
    Ruler ruler;
    auto mergedEClusters = new Clusters(); // will be owned and managed by TestPapasManager
    auto history = new Nodes();
    
    //keep track of what is owned
    m_ownedClusters.push_back(mergedEClusters);
    m_ownedHistory.push_back(history);
    
    auto ecalmerger = TestMergedClusterBuilder(m_papasEvent, typeAndSubtype, ruler, *mergedEClusters, *history);
    m_papasEvent.addCollection(*mergedEClusters);
    m_papasEvent.addCollection(*history);
    
  }
  
  /*void TestPapasManager::blockbuild(std::string ecalTypeAndSubtype, std::string hcalTypeAndSubtype, std::string trackTypeAndSubtype) {
    auto blocks = new Blocks(); //todo change this name
    auto BlockBuilder = BlockBuilder(m_papasEvent, ecalTypeAndSubtype, hcalTypeAndSubtype,trackTypeAndSubtype, *blocks,*history);
    m_papasEvent.addCollection(*blocks);
    m_papasEvent.addHistory(m_history);
    
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
      c->clear();
    m_ownedHistory.clear();
    
    Identifier::reset(); //todo move to papasevent?
    for (auto c : m_ownedClusters)
      c->clear();
    m_ownedClusters.clear();
    
    
  }
  
  
}  // end namespace papas
