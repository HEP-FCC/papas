//
//  TestNewPapasManager.h
//  papas
//
//  Created by Alice Robson on 19/12/16.
//
//

#ifndef TestNewPapasManager_h
#define TestNewPapasManager_h

#include "papas/display/AliceDisplay.h"
#include "papas/reconstruction/PFEvent.h"
#include "papas/datatypes/PapasEvent.h"
//#include "papas/simulation/Simulator.h"
#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/graphtools/DefinitionsNodes.h"
#include <vector>
#include <list>

namespace papas {
  /// High level class to recieve a set of particles, run simulation and then reconstruction
  class TestPapasManager {
  public:
    TestPapasManager(Detector& detector);
    
    void simulate(const Particles& particles);
    void mergeClusters(const std::string& typeAndSubtype);
    void buildBlocks(const std::string& ecalTypeAndSubtype = "em",
                     const std::string& hcalTypeAndSubtype = "hm",
                     const std::string& trackTypeAndSubtype = "ts");
    void reconstruct(const std::string& blockTypeAndSubtype = "br");
    
    const PapasEvent& papasEvent() { return m_papasEvent; }
    void clear();
    
  private:
    Clusters* createClusters();
    Tracks* createTracks();
    Blocks* createBlocks();
    SimParticles* createParticles();
    Nodes* createHistory();
    //bool operator()(IdType i, IdType j);//used for sorting ids
    const Detector& m_detector;
    PapasEvent m_papasEvent;
    
    std::list<Clusters*> m_ownedClusters;
    std::vector<Tracks*> m_ownedTracks;
    std::vector<Blocks*> m_ownedBlocks;
    std::vector<SimParticles*> m_ownedParticles;
    std::vector<Nodes*> m_ownedHistory;
    
    
  };
}



#endif /* TestNewPapasManager_h */
