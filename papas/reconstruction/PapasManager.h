//
//  PapasManager.hpp
//  papas
//
//  Created by Alice Robson on 22/06/16.
//
//

#ifndef PapasManager_hpp
#define PapasManager_hpp

#include "papas/display/AliceDisplay.h"
#include "papas/reconstruction/PFEvent.h"
#include "papas/simulation/Simulator.h"

namespace papas {
/// High level class to recieve a set of particles, run simulation and then reconstruction
class PapasManager {
public:
  PapasManager(Detector& detector);
  void storeParticles(Particles&& particles) { m_particles = std::move(particles); }  // move into PapasManager
  void simulateEvent();
  void mergeClusters();
  void reconstructEvent();
  const Particles& reconstructedParticles() { return m_pfEvent.reconstructedParticles(); }
  const Particles& rawParticles() { return m_particles; }
  void clear();
  void display(bool plot = false);  ///< relocate?
  void show();
  Simulator& simulator() { return m_simulator; }  // history inside simulator can be changed

private:
  bool operator()(IdType i, IdType j);
  const Detector& m_detector;
  Simulator m_simulator;
  PFEvent m_pfEvent;
  Nodes m_history;
  Particles m_particles;
  PFApp m_app;
};
}

#endif /* PapasManager_hpp */
