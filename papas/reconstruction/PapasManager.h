#ifndef PapasManager_h
#define PapasManager_h

#include "papas/display/AliceDisplay.h"
#include "papas/reconstruction/PFEvent.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/simulation/Simulator.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include <vector>

namespace papas {
/// High level class to recieve a set of particles, run simulation and then reconstruction
class PapasManager {
public:
  PapasManager(Detector& detector);
  PapasManager(PFEvent&& pfevent);
  void storeParticles(Particles&& particles) { m_particles = std::move(particles); }  // move into PapasManager
  void simulateEvent();
  void mergeClusters();
  void testMergeClusters();
  void reconstructEvent();
  const Particles& reconstructedParticles() { return m_pfEvent.reconstructedParticles(); }
  const Particles& rawParticles() { return m_particles; }
  const PFEvent& pfEvent() { return m_pfEvent; }
  void clear();
  void display(bool plot = false);  ///< relocate?
  void show();
  Simulator& simulator() { return m_simulator; }  // history inside simulator can be changed
  
private:
  
  const Detector& m_detector;
  Simulator m_simulator;
  PFEvent m_pfEvent;
  PapasEvent m_papasEvent;
  Nodes m_history;
  Particles m_particles;
  PFApp m_app;
  std::vector<Clusters*> m_newClusters;
};
}

#endif /* PapasManager_hpp */
