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
    Clusters& createClusters();
    Tracks& createTracks();
    Blocks& createBlocks();
    SimParticles& createParticles();
    Nodes& createHistory();
    //bool operator()(IdType i, IdType j);//used for sorting ids
    const Detector& m_detector;
    PapasEvent m_papasEvent;
    
    std::list<Clusters> m_ownedClusters;
    std::vector<Tracks> m_ownedTracks;
    std::vector<Blocks> m_ownedBlocks;
    std::vector<SimParticles> m_ownedParticles;
    std::vector<Nodes> m_ownedHistory;
    
    Clusters m_ecalClusters  ;
    Clusters m_hcalClusters ;
    Clusters m_smearedEcalClusters  ;
    Clusters m_smearedHcalClusters ;
    Tracks m_tracks ;
    Tracks m_smearedTracks ;
    SimParticles m_simParticles ;
    Nodes  m_history;
    
    Clusters m_mergedEClusters;
    Nodes m_history2;
    
  };
}

#endif /* TestPapasManager_h */
