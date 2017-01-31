#include "papas/reconstruction/PapasManager.h"
#include "papas/datatypes/Identifier.h"
#include "papas/datatypes/PapasEvent.h"
#include "papas/reconstruction/MergedClusterBuilder.h"
#include "papas/reconstruction/PFBlockBuilder.h"
#include "papas/reconstruction/PFBlockSplitter.h"
#include "papas/reconstruction/PFReconstructor.h"
#include "papas/simulation/Simulator.h"
#include "papas/utility/PDebug.h"

namespace papas {

PapasManager::PapasManager(const Detector& detector) : m_detector(detector), m_papasEvent() {}

void PapasManager::simulate(const ListParticles& particles) {

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
  auto& simParticles = createParticles();

  // run the simulator which will fill the above objects
  auto simulator = Simulator(m_papasEvent, particles, m_detector, ecalClusters, hcalClusters, smearedEcalClusters,
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

  }

void PapasManager::mergeClusters(const std::string& typeAndSubtype) {
  PapasEventRuler ruler(m_papasEvent);                ;
  // create collections ready to receive outputs
  auto& mergedClusters = createClusters();
  auto& history = createHistory();
  auto ecalmerger = MergedClusterBuilder(m_papasEvent, typeAndSubtype, ruler, mergedClusters, history);
  // add outputs into papasEvent
  m_papasEvent.addCollection(mergedClusters);
  m_papasEvent.addHistory(history);
}

void PapasManager::buildBlocks(const std::string& ecalTypeAndSubtype, const std::string& hcalTypeAndSubtype,
                               char trackSubtype) {
  // create empty collections to hold the ouputs, the ouput will be added by the algorithm
  auto& blocks = createBlocks();
  auto& history = createHistory();
  auto blockBuilder =
      PFBlockBuilder(m_papasEvent, ecalTypeAndSubtype, hcalTypeAndSubtype, trackSubtype, blocks, history);
  // store a pointer to the ouputs into the papasEvent
  m_papasEvent.addCollection(blocks);
  m_papasEvent.addHistory(history);
  // printHistory(m_papasEvent.history());
}

void PapasManager::simplifyBlocks(char blockSubtype) {
  // create empty collections to hold the ouputs, the ouput will be added by the algorithm
  auto& simplifiedblocks = createBlocks();
  auto& history = createHistory();
  auto blockBuilder = PFBlockSplitter(m_papasEvent, blockSubtype, simplifiedblocks, history);

  // store a pointer to the outputs into the papasEvent
  m_papasEvent.addCollection(simplifiedblocks);
  m_papasEvent.addHistory(history);
}
void PapasManager::mergeHistories() { m_papasEvent.mergeHistories(); }

void PapasManager::reconstruct(char blockSubtype) {
  auto& history = createHistory();
  auto& recParticles = createParticles();

  auto pfReconstructor = PFReconstructor(m_papasEvent, blockSubtype, m_detector, recParticles, history);
  m_papasEvent.addCollection(recParticles);
  m_papasEvent.addHistory(history);
  m_papasEvent.mergeHistories();
  // printHistory(m_papasEvent.history());
}

void PapasManager::clear() {
  Identifier::reset();
  m_papasEvent.clear();
  m_ownedHistory.clear();
  m_ownedClusters.clear();
  m_ownedTracks.clear();
  m_ownedBlocks.clear();
  m_ownedParticles.clear();
}

Clusters& PapasManager::createClusters() {
  // when the Clusters collection is added to the list its address changes
  // we must return the address of the created Clusters collection after it
  // has been added into the list
  m_ownedClusters.emplace_back(Clusters());
  return m_ownedClusters.back();
}

Tracks& PapasManager::createTracks() {
  m_ownedTracks.emplace_back(Tracks());
  return m_ownedTracks.back();
}

Blocks& PapasManager::createBlocks() {
  m_ownedBlocks.emplace_back(Blocks());
  return m_ownedBlocks.back();
}

PFParticles& PapasManager::createParticles() {
  m_ownedParticles.emplace_back(PFParticles());
  return m_ownedParticles.back();
}

Nodes& PapasManager::createHistory() {
  m_ownedHistory.emplace_back(Nodes());
  return m_ownedHistory.back();
}

}  // end namespace papas
