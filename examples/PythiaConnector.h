//
//  PythiaConnector.h
//  papas
//
//  Created by Alice Robson on 06/07/16.
//
//

#ifndef PythiaConnector_h
#define PythiaConnector_h
#include "datamodel/EventInfoCollection.h"
#include "datamodel/ParticleCollection.h"
#include "utilities/ParticleUtils.h"

// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TROOT.h"
#include "TTree.h"

// STL
#include <iostream>
#include <vector>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"
#include "podio/ROOTWriter.h"

#include "DefinitionsCollections.h"
#include "PParticle.h"
#include "PapasManager.h"

class PapasManager;

/// Example class demonstrating reading from and writing to Pythia root files
class PythiaConnector {
public:
  PythiaConnector(const char* fname);
  void writeParticlesROOT(const char* fname, const papas::Particles& particles);
  void processEvent(unsigned int eventNo, papas::PapasManager& papasManager);  ///<reads and processes a Pythia event
  papas::Particles makePapasParticlesFromGeneratedParticles(
      const fcc::ParticleCollection* ptcs);  ///< converts pythia particles into Papas type particles

private:
  podio::EventStore m_store;
  podio::ROOTReader m_reader;
};

#endif /* PythiaConnector_h */
