//
//  TestPapasManager.h
//  papas
//
//  Created by Alice Robson on 14/12/16.
//
//

#ifndef TestPapasManager_h
#define TestPapasManager_h

namespace papas {
  /// High level class to coordinate PapasEvent run
  class TestPapasManager {
  public:
    TestPapasManager(Detector& detector);
    
    void simulate(const& particles);
    void mergeClusters(std::string typeAndSubtype);
    void buildBlocks(std::string blocksTypeAndSubtype);
    void reconstruct(std::string ecalsTypeAndSubtype,
                     std::string hcalsTypeAndSubtype,
                     std::string tracksTypeAndSubtype);
    
    const PapasEvent& papasEvent() const { return m_papasEvent; }
    
    void clear();
    
    
  private:
    const Detector& m_detector;
    PapasEvent m_papasEvent;
    std::vector<Clusters*> m_ownedClusters;
    std::vector<Nodes*> m_ownedHistorys;
  };
  
  
  
  StatusCode PapasClusterMerger::execute() {
    
    auto ruler = papas::Ruler(); //no need to store
    
    //input data
    auto papasEvent = m_papasevent.get();
    
    //output data  - collections are made outside of papas (handled by Gaudi)
    auto mergedclusters = m_mergedclusterspapas.createAndPut(); //gaudi handle
    auto history = m_newhistory.createAndPut; //gaudi handle
    
    //pass inputs via papasEvent, outputs separately
    //clustertypesubtype set via PapasClusterMerger constructor parameter arguments
    papas::TestMergedClusterBuilder(papasEvent, m_clustertypesubtype, ruler, *mergedclusters, *history);
    
    auto outpapasEvent = m_outPapasEvent.createAndPut(); //gaudi handle
    outpapasEvent.addEvent(papasEvent); // optional
    outPapasEvent.addCollection(mergedclusters);
    //history (make optional)
    outpapasEvent.mergeHistory(papasEvent.history, *history); // copies from papasEvent history and updates history
    outpapasEvent.addHistory(*history);
    return StatusCode::SUCCESS;
    
  }
  
  
  StatusCode PapasReconstructor::execute() {
    
    //input data
    auto papasEvent = m_papasevent.get();
  
    //outputs
    auto history = m_history.createAndPut; //gaudi handle
    auto blocks = m_blocks.createAndPut; //gaudi handle
    auto recparticles = m_particles.createAndPut; //gaudi handle
    auto outpapasEvent = m_outPapasEvent.createAndPut(); //gaudi handle
    
    //pass inputs via papasEvent, outputs separately
    //ecaltype set via PapasReconstructor constructor parameters eg 'em' etc
    papas::Reconstruction(papasEvent, m_ecaltypesubtype, m_hcaltypesubtype, m_tracktypesubtype, *blocks, *recparticles, *history);
    
    outPapasEvent.addEvent(papasEvent);
    outPapasEvent.addCollection(recparticles);
    outpapasEvent.mergeHistory(papasEvent.history, *history); // copies from papasEvent history and updates history
    
    return StatusCode::SUCCESS;
    
  }
  
  
  /*StatusCode PapasReconstructor::execute() {
    
    //input data
    auto eclusters = m_eclusterspapas.get();
    auto hclusters = m_hclusterspapas.get();
    auto tracks = m_trackspapas.get();
    
    auto papasEvent papas::PapasEvent();
    papasEvent.addCollection(eclusters); //add input clusters to papasEvent
    papasEvent.addCollection(hclusters);
    papasEvent.addCollection(tracks);
    //output data  - collections are made outside of papas (handled by Gaudi)
    auto recparticles = m_particlespapas.createAndPut(); //gaudi handle
    auto history = m_newhistory.createAndPut; //gaudi handle
    
    //pass inputs via papasEvent, outputs separately
    auto etype=Papas::Identifier::typeAndSubType(eclusters->begin().first;
                                                 ...
                                                 papas::Reconstruction(papasEvent, m_ecaltypesubtype, m_hcaltypesubtype, m_tracktypesubtype, *blocks, *recparticles, *history);
                                                 
                                                 return StatusCode::SUCCESS;
                                                 
                                                 }*/

  
  

}

StorePapasEventComponent


}

#endif /* TestPapasManager_h */
