//
//  ConvertEDMPapas.cpp
//  papas
//
//  Created by Alice Robson on 06/12/16.
//
//

#include <stdio.h>
#include "ConvertEDMPapas.h"


#include "PythiaConnector.h"
#include "papas/reconstruction/PFReconstructor.h"
#include "papas/reconstruction/PapasManager.h"

#include "datamodel/EventInfoCollection.h"
#include "datamodel/ParticleCollection.h"
#include "datamodel/CaloClusterCollection.h"
#include "utilities/ParticleUtils.h"

#include "papas/utility/PDebug.h"
#include "papas/datatypes/Particle.h"
#include "papas/simulation/Simulator.h"

#include <exception>
#include <string>
#include <sys/stat.h>

// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TROOT.h"
#include "TTree.h"


/*papas::Clusters ConvertClustersToPapas(fcc::CaloClusterCollection* fccClusters, float size, papas::Identifier::ItemType itemtype, char subtype) {
    papas::Clusters clusters;
    for (const auto& c : *fccClusters) {
      const auto position = c.core().position;
      const auto energy = c.core().energy;
      auto cluster = papas::Cluster(energy, TVector3(position.x, position.y, position.z), size, itemtype, subtype);
      clusters.emplace(cluster.id(), std::move(cluster));
    }
    return clusters;
  }
  
  
void AddClustersToEDM(const papas::Clusters& papasClusters, fcc::CaloClusterCollection* fccClusters ) {
  for (const auto& c : papasClusters) {
    auto clust = fccClusters->create();
    clust.core().energy = c.second.energy();
    auto& p3 = clust.core().position;
    p3.x = c.second.position().X();
    p3.y = c.second.position().Y();
    p3.z = c.second.position().Z();
  }
}*/


