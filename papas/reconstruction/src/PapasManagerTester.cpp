//
//  Used only for Unit testing

#include "papas/reconstruction/PapasManagerTester.h"

namespace papas {

PapasManagerTester::PapasManagerTester(Detector& detector) : PapasManager(detector) {}

Simulator PapasManagerTester::setSimulator(Particles& particles) {

  // create empty collections that will be passed to simulator to fill
  // the new collection is to be a concrete class owned by the PapasManger
  // and stored in a list of collections.
  // The collection can then be passed to the Simulator and concrete objects
  // stored in the collection
  // create empty collections that will be passed to simulator to fill
  // the new collection is to be a concrete class owned by the PapasManger
  // and stored in a list of collections.
  // The collection can then be passed to the Simulator and concrete objects
  // stored in the collection
  auto& ecalClusters = createClusters();
  auto& hcalClusters = createClusters();
  auto& smearedEcalClusters = createClusters();
  auto& smearedHcalClusters = createClusters();
  auto& tracks = createTracks();
  auto& smearedTracks = createTracks();
  auto& history = createHistory();

  // run the simulator which will fill the above objects
  Simulator simulator(m_event, m_detector, ecalClusters, hcalClusters, smearedEcalClusters, smearedHcalClusters, tracks,
                      smearedTracks, particles, history);
  return simulator;
}

}  // end namespace papas