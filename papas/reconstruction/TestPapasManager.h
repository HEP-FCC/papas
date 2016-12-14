//
//  TestPapasManager.h
//  papas
//
//  Created by Alice Robson on 14/12/16.
//
//

#ifndef TestPapasManager_h
#define TestPapasManager_h
#include "papas/display/AliceDisplay.h"
#include "papas/reconstruction/PFEvent.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/simulation/Simulator.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include <vector>

namespace papas {
  /// High level class to recieve a set of particles, run simulation and then reconstruction
  class TestPapasManager {
  public:
    TestPapasManager(Detector& detector);
    
    //todo void simulate();
    void mergeClusters(std::string typeAndSubtype);
    void buildBlocks(std::string ecalTypeAndSubtype = "em",
                     std::string hcalTypeAndSubtype = "hm",
                     std::string trackTypeAndSubtype = "ts");
    void reconstruct(std::string blockTypeAndSubtype = "br");
    
    const PapasEvent& papasEvent() { return m_papasEvent; }
    void clear();
    
  private:
    const Detector& m_detector;
    PapasEvent m_papasEvent;
    
    std::vector<Clusters*> m_ownedClusters;
    std::vector<Nodes*> m_ownedHistory;
  };
}

#endif /* TestPapasManager_h */
