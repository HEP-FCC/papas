//
//  PapasManager.cpp
//  papas
//
//  Created by Alice Robson on 22/06/16.
//
//

#include "PapasManager.h"
#include "AliceDisplay.h"
#include "Id.h"
#include "PFBlockBuilder.h"

#include "PFReconstructor.h"
#include "Log.h"

namespace papas {
  
PapasManager::PapasManager(Detector& detector) : m_detector(detector), m_simulator(detector, m_history), m_pfEvent(m_simulator) {}


void PapasManager::simulateEvent(Particles&& particles) {
  m_particles=particles;
  for (const auto& ptc : particles) {
    m_history.emplace(ptc.first, PFNode(ptc.first)); ///< insert the raw particles into the history
    m_simulator.SimulateParticle( ptc.second, ptc.first);
  }
  m_pfEvent.mergeClusters();
}

void PapasManager::reconstructEvent() {

  //get the ids of smeared and merged tracks and clusters that are to be reconstruced
  Ids ids = m_pfEvent.mergedElementIds();

  //create the blocks of linked ids
  auto bBuilder = PFBlockBuilder(m_pfEvent, ids);
  m_pfEvent.setBlocks(bBuilder);

  //do the reconstruction of the blocks
  auto pfReconstructor = PFReconstructor(m_pfEvent);
  pfReconstructor.reconstruct();
}
  
void PapasManager::clear() {
  
  m_simulator.clear();
  m_pfEvent.clear();
  m_history.clear();
  m_particles.clear();
  Id::reset();
  
}

void PapasManager::display() {

  PFApp myApp{};
  myApp.display(m_pfEvent, m_detector);
  myApp.run();
}

}  // end namespace papas