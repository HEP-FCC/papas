#include "papas/datatypes/Identifier.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/reconstruction/PFBlockBuilder.h"
#include "papas/reconstruction/PFReconstructor.h"
#include "papas/reconstruction/TestMergedClusterBuilder.h"
#include "papas/reconstruction/TestPapasManager.h"
#include "papas/simulation/TestSimulator.h"
#include "papas/utility/PDebug.h"

namespace papas {

TestPapasManager::TestPapasManager(Detector& detector) : m_detector(detector), m_papasEvent() {}

void TestPapasManager::simulate(const Particles& particles) {

  // create empty collections that will be passed to simulator to fill
  auto& ecalClusters = createClusters();
  auto& hcalClusters = createClusters();
  auto& smearedEcalClusters = createClusters();
  auto& smearedHcalClusters = createClusters();
  auto& tracks = createTracks();
  auto& smearedTracks = createTracks();
  auto& history = createHistory();
  auto& simParticles = createParticles();

  // run the simulator which will fill the above objects
  auto simulator = TestSimulator(particles, m_detector, ecalClusters, hcalClusters, smearedEcalClusters,
                                 smearedHcalClusters, tracks, smearedTracks, simParticles, history);

  // store the addresses of the filled collections to the PapasEvent
  m_papasEvent.addCollection(ecalClusters);
  m_papasEvent.addCollection(hcalClusters);
  m_papasEvent.addCollection(smearedEcalClusters);
  m_papasEvent.addCollection(smearedHcalClusters);
  m_papasEvent.addCollection(tracks);
  m_papasEvent.addCollection(smearedTracks);
  m_papasEvent.addCollection(simParticles);
  m_papasEvent.addHistory(history);

  /*// TODO  reinstate sorting (but some work on generated particles is needed first)
  std::vector<IdType> ids;
  for (auto kv : particles) {
    ids.push_back(kv.first);
  }
#if WITHSORT
  bool operator()(IdType i, IdType j);
  std::sort(ids.begin(), ids.end(),
            [&](IdType i, IdType j) { return (m_particles.at(i).e() > m_particles.at(j).e()); });
#endif
}*/
}

void TestPapasManager::mergeClusters(const std::string& typeAndSubtype) {
  Ruler ruler;
  // create collections ready to receive outputs
  auto& mergedClusters = createClusters();
  auto& history = createHistory();
  auto ecalmerger = TestMergedClusterBuilder(m_papasEvent, typeAndSubtype, ruler, mergedClusters, history);
  // add outputs into papasEvent
  m_papasEvent.addCollection(mergedClusters);
  m_papasEvent.addHistory(history);
}

/*TODO
 void TestPapasManager::buildBlocks(const std::string& ecalTypeAndSubtype, const std::string& hcalTypeAndSubtype,  const
std::string& trackTypeAndSubtype) {
  //create empty collections to hold the ouputs, the ouput will be added by the algorithm
  auto& blocks = createBlocks();
  auto& history = createHistory();
  auto blockBuilder = PFBlockBuilder(m_papasEvent, ecalTypeAndSubtype, hcalTypeAndSubtype, trackTypeAndSubtype,
blocks, history);
  //store a pointer to the ouputs into the papasEvent
  m_papasEvent.addCollection(blocks);
  m_papasEvent.addHistory(history);

}

 //TODO
void TestPapasManager::reconstruct(std::string blockTypeAndSubtype) {
  auto& history = createHistory();
  auto& recParticles = createParticles();

  auto pfReconstructor = PFReconstructor(m_papasEvent, blockTypeAndSubtype, recparticles, history);
  pfReconstructor.reconstruct();
  m_papasEvent.addCollection(recparticles);

}*/

void TestPapasManager::clear() {
  Identifier::reset();
  m_papasEvent.clear();
  m_ownedHistory.clear();
  m_ownedClusters.clear();
  m_ownedTracks.clear();
  m_ownedBlocks.clear();
  m_ownedParticles.clear();
}

Clusters& TestPapasManager::createClusters() {
  // when the Clusters collection is added to the list its address changes
  // we must return the address of the created Clusters collection after it
  // has been added into the list
  m_ownedClusters.emplace_back(Clusters());
  return m_ownedClusters.back();
}

Tracks& TestPapasManager::createTracks() {
  m_ownedTracks.emplace_back(Tracks());
  return m_ownedTracks.back();
}

Blocks& TestPapasManager::createBlocks() {
  m_ownedBlocks.emplace_back(Blocks());
  return m_ownedBlocks.back();
}

SimParticles& TestPapasManager::createParticles() {
  m_ownedParticles.emplace_back(SimParticles());
  return m_ownedParticles.back();
}

Nodes& TestPapasManager::createHistory() {
  m_ownedHistory.emplace_back(Nodes());
  return m_ownedHistory.back();
}

}  // end namespace papas
