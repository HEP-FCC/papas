//
//  PythiaConnector.h
//  papas
//
//  Created by Alice Robson on 06/07/16.
//
//

#ifndef PythiaConnector_h
#define PythiaConnector_h
#include "datamodel/ParticleCollection.h"
#include "datamodel/EventInfoCollection.h"
#include "utilities/ParticleUtils.h"

// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TLorentzVector.h"

// STL
#include <vector>
#include <iostream>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"
#include "podio/ROOTWriter.h"

#include "DefinitionsCollections.h"
#include "PapasManager.h"
#include "PParticle.h"

//namespace papas {
  
  
  class PapasManager;
  
  
class PythiaConnector {
public:
  PythiaConnector(const char* fname) ;
  void writeParticlesROOT(const char* fname, const papas::Particles& particles) ;
  void processEvent(unsigned int eventNo,  papas::PapasManager& papasManager) ;
  papas::Particles makePapasParticlesFromGeneratedParticles(const fcc::ParticleCollection* ptcs) ;
private:
   podio::EventStore m_store;
   podio::ROOTReader m_reader;
};
  
//}
#endif /* PythiaConnector_h */
