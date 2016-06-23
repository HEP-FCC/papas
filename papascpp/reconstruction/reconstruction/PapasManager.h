//
//  PapasManager.hpp
//  papas
//
//  Created by Alice Robson on 22/06/16.
//
//

#ifndef PapasManager_hpp
#define PapasManager_hpp

#include "PFEvent.h"
#include "Simulator.h"

namespace papas {
  
class PapasManager {
public:
  PapasManager(Detector& detector);
  void simulateEvent(Particles&& particles);
  void reconstructEvent();
  const Particles& reconstructedParticles() {return m_pfEvent.reconstructedParticles();}
  void clear();
  void display();  ///< relocate?
  
private:
  const Detector& m_detector;
  Simulator m_simulator;
  PFEvent m_pfEvent;
  Nodes m_history;
  Particles m_particles;
};
  
  

}

#endif /* PapasManager_hpp */
