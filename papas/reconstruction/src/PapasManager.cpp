#include "papas/reconstruction/PapasManager.h"

#include "papas/graphtools/EventRuler.h"
#include "papas/reconstruction/BuildPFBlocks.h"
#include "papas/reconstruction/MergeClusters.h"
#include "papas/reconstruction/PFReconstructor.h"
#include "papas/reconstruction/SimplifyPFBlocks.h"
#include "papas/simulation/Simulator.h"

namespace papas {

PapasManager::PapasManager(const Detector& detector) : m_detector(detector), m_history(), m_event(m_history) {}

void PapasManager::simulate(Particles& particles) {
  // create empty collections that will be passed to simulator to fill
  // the new collection is to be a concrete class owned by the PapasManger
  // and stored in a list of collections.
  // The collection can then be passed to the Simulator and concrete objects
  // stored in the collection
  // Note: Normally The empty particles passed in here should have been created by the PapasManager
  auto& ecalClusters = createClusters();
  auto& hcalClusters = createClusters();
  auto& smearedEcalClusters = createClusters();
  auto& smearedHcalClusters = createClusters();
  auto& tracks = createTracks();
  auto& smearedTracks = createTracks();
     
  // run the simulator which will fill the above objects
  Simulator simulator(m_event, m_detector, ecalClusters, hcalClusters, smearedEcalClusters, smearedHcalClusters, tracks,
                      smearedTracks, particles, m_history);

  // store the addresses of the filled collections to the Event
  m_event.addCollectionToFolder(ecalClusters);
  m_event.addCollectionToFolder(hcalClusters);
  m_event.addCollectionToFolder(smearedEcalClusters);
  m_event.addCollectionToFolder(smearedHcalClusters);
  m_event.addCollectionToFolder(tracks);
  m_event.addCollectionToFolder(smearedTracks);
  // NB can only add the particle collection once the particles are completed (eg paths added in)
  // this is because they are stored here as const objects
  m_event.addCollectionToFolder(particles);
}

void PapasManager::mergeClusters(const std::string& typeAndSubtype) {
  EventRuler ruler(m_event);
  // create collections ready to receive outputs
  auto& mergedClusters = createClusters();
  papas::mergeClusters(m_event, typeAndSubtype, ruler, mergedClusters, m_history);
  // add outputs into event
  m_event.addCollectionToFolder(mergedClusters);
}

void PapasManager::buildBlocks(const char ecalSubtype, char hcalSubtype, char trackSubtype) {
  // create empty collections to hold the ouputs, the ouput will be added by the algorithm
  auto& blocks = createBlocks();
  buildPFBlocks(m_event, ecalSubtype, hcalSubtype, trackSubtype, blocks, m_history);
  // store a pointer to the ouputs into the event
  m_event.addCollectionToFolder(blocks);
}

void PapasManager::simplifyBlocks(char blockSubtype) {
  // create empty collections to hold the ouputs, the ouput will be added by the algorithm
  auto& simplifiedblocks = createBlocks();
  simplifyPFBlocks(m_event, blockSubtype, simplifiedblocks, m_history);
  // store a pointer to the outputs into the event
  m_event.addCollectionToFolder(simplifiedblocks);
}

void PapasManager::reconstruct(char blockSubtype) {
  auto& recParticles = createParticles();
  PFReconstructor pfReconstructor(m_event, blockSubtype, m_detector, recParticles, m_history);
  m_event.addCollectionToFolder(recParticles);
}

void PapasManager::clear() {
  m_event.clear();
  m_history.clear();
  m_ownedClustersList.clear();
  m_ownedTracksList.clear();
  m_ownedBlocksList.clear();
  m_ownedParticlesList.clear();
}

Clusters& PapasManager::createClusters() {
  // when the Clusters collection is added to the list its address changes
  // we must return the address of the created Clusters collection after it
  // has been added into the list
  m_ownedClustersList.emplace_back(Clusters());
  return m_ownedClustersList.back();
}

Tracks& PapasManager::createTracks() {
  m_ownedTracksList.emplace_back(Tracks());
  return m_ownedTracksList.back();
}

Blocks& PapasManager::createBlocks() {
  m_ownedBlocksList.emplace_back(Blocks());
  return m_ownedBlocksList.back();
}

Particles& PapasManager::createParticles() {
  m_ownedParticlesList.emplace_back(Particles());
  return m_ownedParticlesList.back();
}

}  // end namespace papas
