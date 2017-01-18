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
#include "datamodel/MCParticleCollection.h"
#include "datamodel/GenVertexConst.h"
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

#include "papas/datatypes/DefinitionsCollections.h"
#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Cluster.h"
#include "papas/reconstruction/PapasManager.h"
#include "papas/datatypes/Identifier.h"

class PapasManager;

/// Example class demonstrating reading from and writing to Pythia root files
class PythiaConnector {
public:
  PythiaConnector(const char* fname);
  void writeParticlesROOT(const char* fname, const papas::SimParticles& particles);
  void writeClustersROOT(const char* fname, const papas::Clusters& clusters);
  
  //todo find new home;
  void displayEvent(const papas::PapasManager& papasManager);
  void processEvent(unsigned int eventNo, papas::PapasManager& papasManager);  ///<reads and processes a Pythia event
                                                                               //void processEvent(unsigned int eventNo, PapasManager& papasManager);
  papas::ListParticles makePapasParticlesFromGeneratedParticles(
      const fcc::MCParticleCollection* ptcs);  ///< converts pythia particles into Papas type particles
  papas::Clusters ConvertClustersToPapas(const fcc::CaloClusterCollection& fccClusters, float size, papas::Identifier::ItemType itemtype, char subtype) const;

  void AddClustersToEDM(const papas::Clusters& papasClusters, fcc::CaloClusterCollection& fccClusters );
private:
  podio::EventStore m_store;
  podio::ROOTReader m_reader;
};

#endif /* PythiaConnector_h */
