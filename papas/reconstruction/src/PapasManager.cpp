//
//  PapasManager.cpp
//  papas
//
//  Created by Alice Robson on 22/06/16.
//
//

#include "papas/reconstruction/PapasManager.h"
#include "papas/display/AliceDisplay.h"
#include "papas/graphtools/EventRuler.h"
#include "papas/datatypes/Identifier.h"
#include "papas/reconstruction/MergedClusterBuilder.h"
#include "papas/reconstruction/TestMergedClusterBuilder.h"
#include "papas/utility/PDebug.h"
#include "papas/reconstruction/PFBlockBuilder.h"
#include "papas/reconstruction/PFReconstructor.h"

namespace papas {

PapasManager::PapasManager(Detector& detector)
    : m_detector(detector), m_simulator(detector, m_history), m_pfEvent(m_simulator), m_papasEvent() {}

void PapasManager::simulateEvent() {
  // order the particles according to id
  std::vector<IdType> ids;
  for (auto kv : m_particles) {
    ids.push_back(kv.first);
  }
#if WITHSORT
  bool operator()(IdType i, IdType j);
  std::sort(ids.begin(), ids.end(),
            [&](IdType i, IdType j) { return (m_particles.at(i).e() > m_particles.at(j).e()); });
#endif
  for (const auto& id : ids) {
    m_history.emplace(id, std::move(PFNode(id)));  ///< insert the raw particle ids into the history
    m_simulator.simulateParticle(m_particles.at(id), id);
  }
}

void PapasManager::mergeClusters() {
  Ruler ruler;
  auto ecalmerger = MergedClusterBuilder(m_pfEvent.ecalClusters(), ruler, m_history);
  m_pfEvent.setMergedEcals(ecalmerger.mergedClusters());  // move
  MergedClusterBuilder hcalmerger{m_pfEvent.hcalClusters(), ruler, m_history};
  m_pfEvent.setMergedHcals(hcalmerger.mergedClusters());  // move
}
  
void PapasManager::testMergeClusters() {
  Ruler ruler;
  auto mergedEClusters = new Clusters(); // will be owned and managed by PapasManager
  m_newClusters.push_back(mergedEClusters);
  auto ecalmerger = TestMergedClusterBuilder(m_papasEvent, "es", ruler, *mergedEClusters, m_history);
  m_papasEvent.addCollection(*mergedEClusters);
  m_papasEvent.addCollection(*m_history);
  
}
  

void PapasManager::reconstructEvent() {
  auto pfReconstructor = PFReconstructor(m_pfEvent);
  pfReconstructor.reconstruct();
  // return the blocks and particles to the event
  m_pfEvent.setReconstructedParticles(std::move(pfReconstructor.particles()));
  m_pfEvent.setBlocks(std::move(pfReconstructor.blocks()));
  //std::cout << "rec size" << reconstructedParticles().size();
}

void PapasManager::clear() {
  m_pfEvent.clear();
  m_papasEvent.clear();
  m_history.clear();
  m_particles.clear();
  m_simulator.clear();
  Identifier::reset();
  for (auto c : m_newClusters)
    c->clear();
  m_newClusters.clear();
}

void PapasManager::display(bool jpg) {

  //PFApp myApp{}; // I think this should turn into a PapasManager member

  m_app.display(m_simulator, m_pfEvent, m_particles, m_detector);
  if (jpg) m_app.jpg();
}
void PapasManager::show() {

  // move to PFApp;

  gSystem->ProcessEvents();
}

}  // end namespace papas
