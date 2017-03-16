//
//  r_test.cpp
//  fastsim
//
//  Created by Alice Robson on 25/04/16.
//
//

#include <RInside.h>
#include <stdio.h>

// C++
#include <cstdlib>
#include <iostream>

// ROOT
#include "TApplication.h"
#include "TLorentzVector.h"
#include "TROOT.h"
#include "TVector3.h"

// gtest test
#include "gtest/gtest.h"

#include "PFParticle.h"
#include "Particle.h"
#include "TVector3.h"
#include "Track.h"
#include "datatypes/papas/Cluster.h"
#include "datatypes/papas/Path.h"
#include "detectors/papas/CMS.h"
#include "geometry.h"
#include "material.h"
#include "simulation/papas/Simulator.h"

using namespace papas;
void tryR(int argc, char* argv[]);
void r_density_plot(const std::vector<double>& v, RInside&);

int main(int argc, char* argv[]) {
  tryR(argc, argv);
  return EXIT_SUCCESS;
}

void tryR(int argc, char* argv[]) {

  RInside R(argc, argv);

  // Create CMS detector
  CMS CMSDetector;

  // Create simulator
  Simulator sim = Simulator{CMSDetector};

  // R test of smearing
  Ids smeared_clust_Ids;

  // Photons
  for (int i = 1; i < 10; i++) {
    PFParticle& photon = sim.addParticle(22, M_PI / 2. + 0.025 * i, M_PI / 2. + 0.3 * i, 100);
    sim.simulatePhoton(photon);
    // R test of smearing
    Ids c_Ids = sim.linkedEcalSmearedClusterIds(photon.id());
    smeared_clust_Ids.insert(std::end(smeared_clust_Ids), std::begin(c_Ids), std::end(c_Ids));
  }
  // Hadrons
  for (int i = 1; i < 20; i++) {
    PFParticle& hadron = sim.addParticle(211, M_PI / 2. + 0.5 * i, 0, 40. * (i));
    sim.simulateHadron(hadron);
  }

  // Rtest of smearing
  std::vector<double> w;
  w.reserve(10000);
  const Clusters& clusters = sim.smearedEcalClusters();
  for (auto x : smeared_clust_Ids) {
    w.push_back(clusters.find(x)->second.energy());
  }
  r_density_plot(w, R);

  /* //Check density plot for smeared Photons
  std::vector<double> w2;
  w2.reserve(10000);
  for (auto x :sim.smearedEcalClusters())
  { if (Id::isSmeared(x.second.id()))
  w2.push_back( x.second.energy());
  }
  r_density_plot(w2, R);*/
}

void r_density_plot(const std::vector<double>& v, RInside& R) {

  R["y"] = v;  // assign weights
  std::string txt = "tmpf <- tempfile('curve'); "
                    "pdf(tmpf); "
                    "plot(density(y)); "
                    "dev.off(); "
                    " tmpf ";
  std::string newtmpfile = R.parseEval(txt);  // evaluate assignment
  system(("open " + newtmpfile).c_str());
  // unlink(newtmpfile.c_str());
}
