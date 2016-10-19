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
#include "papas/datatypes/Id.h"
#include "papas/reconstruction/MergedClusterBuilder.h"
#include "papas/utility/PDebug.h"
#include "papas/reconstruction/PFBlockBuilder.h"
#include "papas/reconstruction/PFReconstructor.h"

namespace papas {

PapasManager::PapasManager(Detector& detector)
    : m_detector(detector), m_simulator(detector, m_history), m_pfEvent(m_simulator) {}

void PapasManager::simulateEvent() {
  // order the particles according to id
  std::vector<Id::Type> ids;
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
}

void PapasManager::mergeClusters() {
  EventRuler ruler{m_pfEvent};
  MergedClusterBuilder ecalmerger{m_pfEvent.ecalClusters(), ruler, m_history};
  m_pfEvent.setMergedEcals(ecalmerger.mergedClusters());  // move
  MergedClusterBuilder hcalmerger{m_pfEvent.hcalClusters(), ruler, m_history};
  m_pfEvent.setMergedHcals(hcalmerger.mergedClusters());  // move
}

void PapasManager::reconstructEvent() {

  auto pfReconstructor = PFReconstructor(m_pfEvent);
  pfReconstructor.reconstruct();
  // return the blocks and particles to the event
  m_pfEvent.setReconstructedParticles(std::move(pfReconstructor.particles()));
  m_pfEvent.setBlocks(std::move(pfReconstructor.blocks()));
}

void PapasManager::clear() {
  m_pfEvent.clear();
  m_history.clear();
  m_particles.clear();
  m_simulator.clear();
  Id::reset();
}

void PapasManager::display(bool jpg) {

  // PFApp myApp{}; // I think this should turn into a PapasManager member

  m_app.display(m_simulator, m_pfEvent, m_particles, m_detector);
  if (jpg) m_app.jpg();
}
void PapasManager::show() {

  // move to PFApp;

  gSystem->ProcessEvents();
}

}  // end namespace papas
