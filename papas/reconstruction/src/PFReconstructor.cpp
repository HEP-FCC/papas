//
//  PFReconstructor.cpp
//  fastsim
//
//  Created by Alice Robson on 20/04/16.
//
//

#include "papas/reconstruction/PFReconstructor.h"

#include <algorithm>
#include <math.h>
#include <vector>

#include "TLorentzVector.h"
#include "papas/datatypes/Cluster.h"
#include "papas/datatypes/HistoryHelper.h"
#include "papas/datatypes/ParticlePData.h"
#include "papas/datatypes/Path.h"
#include "papas/datatypes/SimParticle.h"
#include "papas/datatypes/Track.h"
#include "papas/graphtools/Edge.h"
#include "papas/reconstruction/BlockSplitter.h"
#include "papas/reconstruction/PFBlock.h"
#include "papas/reconstruction/PFBlockBuilder.h"
#include "papas/reconstruction/PFEvent.h"
#include "papas/utility/PDebug.h"

namespace papas {

PFReconstructor::PFReconstructor(const PapasEvent& papasEvent, char blockSubtype, SimParticles& particles, Nodes& history)
    : m_papasEvent(papasEvent), m_particles(particles),  m_history(history) {

  
  const auto& blocks = m_papasEvent.blocks(blockSubtype);
  auto blockids = m_papasEvent.collectionIds<Blocks>(blocks);
#if WITHSORT
  std::sort(blockids.begin(), blockids.end());
#endif
  
  
  for (auto bid : blockids) {
    const PFBlock& block = blocks.at(bid);
    PDebug::write("Processing {}", block);
    reconstructBlock(block);
    
  }
  if (m_unused.size() > 0) {
    PDebug::write("unused elements ");
    for (auto u : m_unused)
      PDebug::write("{},", u);
    // TODO warning message
  }

  // TODO sort m_blocks

 /* // simplify the blocks by editing the links
  // each track will end up linked to at most one hcal

  // sort the blocks by id to ensure match with python
  std::vector<IdType> blockids;
  std::vector<IdType> newblockids;

  auto bBuilder = PFBlockBuilder(m_papasEvent, "em", "hm", 's', m_blocks, m_history);
  m_blocks = bBuilder.blocks();

  for (const auto& b : m_blocks) {
    blockids.push_back(b.first);
  }
#if WITHSORT
  std::sort(blockids.begin(), blockids.end());
#endif
  // go through each block and see if it can be simplified
  // in some cases it will end up being split into smaller blocks
  // Note that the old block will be marked as disactivated
  for (auto bid : blockids) {
    Blocks newBlocks = simplifyBlock(bid);
    if (newBlocks.size() > 0) {
      for (auto& b : newBlocks) {
        IdType id = b.first;
        m_blocks.emplace(id, std::move(b.second));
        newblockids.push_back(b.first);
      }
    }
  }
  blockids.insert(std::end(blockids), std::begin(newblockids), std::end(newblockids));

  for (auto bid : blockids) {
    PFBlock& block = m_blocks.at(bid);
    if (block.isActive()) {  // when blocks are split the original gets deactivated
      PDebug::write("Processing {}", block);
      reconstructBlock(block);
    }
  }
  if (m_unused.size() > 0) {
    PDebug::write("unused elements ");
    for (auto u : m_unused)
      PDebug::write("{},", u);
    // TODO warning message
  }*/
}

#if 0
Blocks PFReconstructor::simplifyBlock(IdType id) {
  /* Block: a block which contains list of element ids and set of edges that connect them
   history_nodes: optional dictionary of Nodes with element identifiers in each node

   returns None or a dictionary of new split blocks

   The goal is to remove, if needed, some links from the block so that each track links to
   at most one hcal within a block. In some cases this may separate a block into smaller
   blocks (splitblocks). The BlockSplitter is used to return the new smaller blocks.
   If history_nodes are provided then the history will be updated. Split blocks will
   have the tracks and cluster elements as parents, and also the original block as a parent
   */
  Blocks splitBlocks;
  auto& block = m_blocks.at(id);
  Ids ids = block.elementIds();

  if (ids.size() <= 1) {  // if block is just one element therer are no links to remove
    return splitBlocks;
  }

  /* work out any links that need to be removed
   - for tracks unink all hcals except the closest hcal
   - for ecals unlink hcals */
  Edges toUnlink;  // TODO think about copy
  std::vector<Edge::EdgeKey> linkedEdgeKeys;
  bool firstHCAL;
  double minDist = -1;
  for (auto id : ids) {
    if (Identifier::isTrack(id)) {
      linkedEdgeKeys = block.linkedEdgeKeys(id, Edge::EdgeType::kHcalTrack);
      if (linkedEdgeKeys.size() > 0) {
        firstHCAL = true;
        // find minimum distance
        for (auto elem : linkedEdgeKeys) {
          if (firstHCAL) {
            minDist = block.findEdge(elem).distance();
            firstHCAL = false;
          } else {
            minDist = fmin(minDist, block.findEdge(elem).distance());
          }
        }
        // unlink anything that is greater than minimum distance
        for (auto elem : linkedEdgeKeys) {
          if (block.findEdge(elem).distance() > minDist) {  // (could be more than one at zero distance)
            toUnlink[elem] = block.findEdge(elem);          // should toUnlink be list of keys rather than edges
          }
        }
      }
    } else if (Identifier::isEcal(id)) {
      // this is now handled  elsewhere in  Ruler::distance and so could be removed
      // remove all ecal-hcal links. ecal linked to hcal give rise to a photon anyway.
      linkedEdgeKeys = block.linkedEdgeKeys(id, Edge::EdgeType::kEcalHcal);  //"ecal_hcal")
      for (auto elem : linkedEdgeKeys) {
        toUnlink[elem] = block.findEdge(elem);
      }
    }
  }

  // if there is something to unlink then use the BlockSplitter
  if (toUnlink.size() > 0) {
    splitBlocks = BlockSplitter(toUnlink, block, m_history).blocks();
  }
  return splitBlocks;  // moves
}
#endif

void PFReconstructor::reconstructBlock(const PFBlock& block) {
  /// see class description for summary of reconstruction approach

  Ids ids = block.elementIds();
#if WITHSORT
  std::sort(ids.begin(), ids.end());
#endif
  for (auto id : ids) {
    m_locked[id] = false;
  }

  reconstructMuons(block);
  reconstructElectrons(block);
  // keeping only the elements that have not been used so far
  Ids uids;
  for (auto id : ids) {
    if (!m_locked[id]) uids.push_back(id);
  }
  if (uids.size() == 1) {  //#TODO WARNING!!! LOTS OF MISSING CASES
    IdType id = *uids.begin();
    auto parentIds = Ids{block.id(), id};
    if (Identifier::isEcal(id)) {
      reconstructCluster(m_papasEvent.cluster(id), papas::Layer::kEcal, parentIds);
    } else if (Identifier::isHcal(id)) {
      reconstructCluster(m_papasEvent.cluster(id), papas::Layer::kHcal, parentIds);
    } else if (Identifier::isTrack(id)) {
      reconstructTrack(m_papasEvent.track(id), 211, parentIds);
    } else {  // ask Colin about energy balance - what happened to the associated clusters that one would expect?
              // TODO
    }
  } else {
    for (auto id : uids) {
      if (Identifier::isHcal(id)) {
        reconstructHcal(block, id);
      }
    }
    for (auto id : ids) {
      if (Identifier::isTrack(id) && !m_locked[id]) {
        /* unused tracks, so not linked to HCAL
         # reconstructing charged hadrons.
         # ELECTRONS TO BE DEALT WITH.*/
        auto parentIds = Ids{block.id(), id};
        reconstructTrack(m_papasEvent.track(id), 211, parentIds);
        for (auto idlink : block.linkedIds(id, Edge::EdgeType::kEcalTrack)) {
          // TODO ask colin what happened to possible photons here:
          // TODO add in extra photons but decide where they should go?
          m_locked[idlink] = true;
        }
      }
    }
  }
  for (auto& id : ids) {
    if (!m_locked[id]) {
      m_unused.push_back(id);
    }
  }
}

void PFReconstructor::reconstructMuons(const PFBlock& block) {
  /// Reconstruct muons in block.'''
  Ids ids = block.elementIds();
#if WITHSORT
  std::sort(ids.begin(), ids.end());
#endif
  for (auto id : ids) {
    if (Identifier::isTrack(id) && isFromParticle(id, "ps", 13)) {

      auto parentIds = Ids{block.id(), id};
      reconstructTrack(m_papasEvent.track(id), 13, parentIds);
    }
  }
}

void PFReconstructor::reconstructElectrons(const PFBlock& block) {
  /*Reconstruct electrons in block.*/
  Ids ids = block.elementIds();
#if WITHSORT
  std::sort(ids.begin(), ids.end());
#endif

  /* the simulator does not simulate electron energy deposits in ecal.
  # therefore, one should not lock the ecal clusters linked to the
  # electron track as these clusters are coming from other particles.*/

  for (auto id : ids) {
    if (Identifier::isTrack(id) && isFromParticle(id, "ps", 11)) {

      auto parentIds = Ids{block.id(), id};
      reconstructTrack(m_papasEvent.track(id), 13, parentIds);
    }
  }
}

/*void PFReconstructor::insertParticle(const PFBlock& block, SimParticle&& newparticle) {
 
  IdType newid = newparticle.id();
  m_particles.emplace(newid, std::move(newparticle));


  // find the node for the block
  PFNode& blockNode = findOrMakeNode(block.id());
  PFNode& particleNode = findOrMakeNode(newid);
  blockNode.addChild(particleNode);
  // link particle to block elements
  for (auto element_id : block.elementIds()) {
    m_historyNodes[element_id].addChild(particleNode);
  }
}*/
  
  
  PFNode& PFReconstructor::findOrMakeNode(IdType id) const {
    if (m_history.find(id)==m_history.end()){
      auto newnode = PFNode(id);
      m_history.emplace(id, newnode);
    }
    return m_history.at(id);
  }

void PFReconstructor::insertParticle(const Ids& parentIds, SimParticle& newparticle) {
  /* The new particle will be inserted into the history_nodes (if present).
   A new node for the particle will be created if needed.
   It will have as its parents the block and all the elements of the block.
   '''
   #Note that although it may be possible to specify more closely that the particle comes from
   #some parts of the block, there are frequently ambiguities and so for now the particle is
   #linked to everything in the block*/
  // if (newparticle) :
  IdType newid = newparticle.id();
  m_particles.emplace(newid, std::move(newparticle));

  PFNode particleNode = findOrMakeNode(newid);
  // add in parental history
  for (auto elementId : parentIds) {
    auto& parentNode =findOrMakeNode(elementId);
    parentNode.addChild(particleNode);
  }
}

bool PFReconstructor::isFromParticle(IdType id, std::string typeAndSubtype, int pdgid) const {
  /*returns: True if object unique_id comes, directly or indirectly,
from a particle of type type_and_subtype, with this absolute pdgid.
*/
  
  auto historyHelper = HistoryHelper(m_papasEvent);
  
  auto parentIds = historyHelper.linkedIds(id, typeAndSubtype, DAG::enumVisitType::PARENTS);
  
  

  bool isFromPdgId = false;
  for (auto pid : parentIds) {
    
    if (abs(m_papasEvent.particle(pid).pdgId()) == abs(pdgid)) isFromPdgId = true;
  }
  return isFromPdgId;
}

double PFReconstructor::neutralHadronEnergyResolution(const Cluster& hcal) const {
  /*WARNING CMS SPECIFIC!
   //http://cmslxr.fnal.gov/source/RecoParticleFlow/PFProducer/src/PFAlgo.cc#3350
   */
  double energy = fmax(hcal.energy(), 1.);
  double stoch = 1.02;
  double kconst = 0.065;
  if (fabs(hcal.position().Eta()) > 1.48) {
    stoch = 1.2;
    kconst = 0.028;
  }
  double resol = sqrt(pow(stoch, 2) / energy + pow(kconst, 2));
  return resol;
}

double PFReconstructor::nsigmaHcal(const Cluster& cluster) const {
  /*'WARNING CMS SPECIFIC!

   //http://cmslxr.fnal.gov/source/RecoParticleFlow/PFProducer/src/PFAlgo.cc#3365
   '''*/
  return 1. + exp(-cluster.energy() / 100.);
}

void PFReconstructor::reconstructHcal(const PFBlock& block, IdType hcalId) {
  /*
   block: element ids and edges
   hcalid: id of the hcal being processed her

   has hcal and has a track
   -> add up all connected tracks, turn each track into a charged hadron
   -> add up all ecal energies
   -> if track energies is greater than hcal energy then turn the missing energies into an ecal (photon)
   NB this links the photon to the hcal rather than the ecals
   -> if track energies are less than hcal then make a neutral hadron with rest of hcal energy and turn all ecals into
   photons
   has hcal but no track (nb by design there will be no attached ecals because hcal ecal links have been removed)
   -> make a neutral hadron
   has hcals
   -> each hcal is treated using rules above
   */

  // hcal used to make ecal_in has a couple of possible issues
  // m_pfEvent.HCALCluster(hcalId);

  // TODO assert(len(block.linked_ids(hcalid, "hcal_hcal"))==0  )
  // TODO sorting Ids trackids =    block.sort_distance_energy(hcalid, block.linked_ids(hcalid, "hcal_track") )

  Ids ecalIds;
  Ids trackIds = block.linkedIds(hcalId, Edge::EdgeType::kHcalTrack);
#if WITHSORT
  std::sort(trackIds.begin(), trackIds.end());
#endif
  for (auto trackId : trackIds) {
    for (auto ecalId : block.linkedIds(trackId, Edge::EdgeType::kEcalTrack)) {
      /*the ecals get all grouped together for all tracks in the block
       # Maybe we want to link ecals to their closest track etc?
       # this might help with history work
       # ask colin.*/
      if (!m_locked[ecalId]) {
        ecalIds.push_back(ecalId);
        m_locked[ecalId] = true;
      }
    }
  }
#if WITHSORT
  std::sort(trackIds.begin(), trackIds.end());
  std::sort(ecalIds.begin(), ecalIds.end());
#endif
  // hcal should be the only remaining linked hcal cluster (closest one)
  const Cluster& hcal = m_papasEvent.cluster(hcalId);
  double hcalEnergy = hcal.energy();
  double ecalEnergy = 0.;
  double trackEnergy = 0.;

  if (!trackIds.empty()) {
    for (auto id : trackIds) {
      const Track& track = m_papasEvent.track(id);
      auto parentIds = Ids{block.id(), id};

      auto ecalLinks = block.linkedIds(id, Edge::kEcalTrack);
      parentIds.insert(parentIds.end(), ecalLinks.begin(), ecalLinks.end());

      auto hcalLinks = block.linkedIds(id, Edge::kHcalTrack);
      parentIds.insert(parentIds.end(), hcalLinks.begin(), hcalLinks.end());
      reconstructTrack(track, 211, parentIds);
      trackEnergy += track.energy();
    }
    for (auto id : ecalIds) {
      ecalEnergy += m_papasEvent.cluster(id).energy();
    }
    double deltaERel = (hcalEnergy + ecalEnergy) / trackEnergy - 1.;
    double caloERes = neutralHadronEnergyResolution(hcal);
    /*self.log.info( 'dE/p, res = {derel}, {res} '.format(
     derel = delta_e_rel,
     res = calo_eres ))*/
    if (deltaERel > nsigmaHcal(hcal) * caloERes) {  //# approx means hcal energy + ecal energies > track energies

      double excess = deltaERel * trackEnergy;  // energy in excess of track energies
      // print( 'excess = {excess:5.2f}, ecal_E = {ecal_e:5.2f}, diff = {diff:5.2f}'.format(
      //   excess=excess, ecal_e = ecal_energy, diff=excess-ecal_energy))
      if (excess <= ecalEnergy) { /* # approx means hcal energy > track energies
                                   # Make a photon from the ecal energy
                                   # We make only one photon using only the combined ecal energies*/
        auto parentIds = ecalIds;
        parentIds.push_back(block.id());
        reconstructCluster(hcal, papas::Layer::kEcal, parentIds, excess);
      }

      else {  // approx means that hcal energy>track energies so we must have a neutral hadron
              // excess-ecal_energy is approximately hcal energy  - track energies
        auto parentIds = Ids{block.id(), hcalId};
        reconstructCluster(hcal, papas::Layer::kHcal, parentIds, excess - ecalEnergy);
        if (ecalEnergy) {
          // make a photon from the remaining ecal energies
          // again history is confusingbecause hcal is used to provide direction
          // be better to make several smaller photons one per ecal?
          auto parentIds = ecalIds;
          parentIds.push_back(block.id());
          reconstructCluster(hcal, papas::Layer::kEcal, parentIds, ecalEnergy);
        }
      }
    }
  } else {  // # case whether there are no tracks make a neutral hadron for each hcal
            //# note that hcal-ecal links have been removed so hcal should only be linked to
            //# other hcals
    auto parentIds = Ids{block.id(), hcalId};
    reconstructCluster(hcal, papas::Layer::kHcal, parentIds);
  }
  m_locked[hcalId] = true;
}

void PFReconstructor::reconstructCluster(const Cluster& cluster, papas::Layer layer, Ids parentIds, double energy,
                                         const TVector3& vertex) {
  // construct a photon if it is an ecal
  // construct a neutral hadron if it is an hcal
  int pdgId = 0;
  if (energy < 0) {
    energy = cluster.energy();
  }
  // double charge = ParticlePData::particleCharge(pdgId);
  if (layer == papas::Layer::kEcal) {
    pdgId = 22;  // photon
  } else if (layer == papas::Layer::kHcal) {
    pdgId = 130;  // K0
  } else {
    // TODO raise ValueError('layer must be equal to ecal_in or hcal_in')
  }
  // assert(pdg_id)
  double mass = ParticlePData::particleMass(pdgId);

  if (energy < mass)  // no particle
    return;

  double momentum;
  if (mass == 0) {
    momentum = energy;
  }  //#avoid sqrt for zero mass
  else {
    momentum = sqrt(pow(energy, 2) - pow(mass, 2));
  }
  TVector3 p3 = cluster.position().Unit() * momentum;
  TLorentzVector p4 = TLorentzVector(p3.Px(), p3.Py(), p3.Pz(), energy);  // mass is not accurate here

  // IdType newid = Identifier::makeRecParticleId();
  auto particle = SimParticle(pdgId, 0., p4, vertex, 0, 'r');
  // TODO discuss with Colin
  particle.path()->addPoint(papas::Position::kEcalIn, cluster.position());
  if (layer == papas::Layer::kHcal) {  // alice not sure
    particle.path()->addPoint(papas::Position::kHcalIn, cluster.position());
  }

  // alice: Colin this may be a bit strange
  // because we can make a photon with a
  // path where the point is actually that
  // of the hcal?
  // nb this only is problem if the cluster and the assigned layer are different
  // particle.setPath(path);
  // particle.clusters[layer] = cluster  # not sure about this either when hcal is used to make an ecal cluster?
  m_locked[cluster.id()] = true;  // alice : just OK but not nice if hcal used to make ecal.
  // TODO make more flexible and able to detect what type of cluster
  PDebug::write("Made Reconstructed{} from Merged{}", particle, cluster);
  insertParticle(parentIds, particle);
  // return particle;
}

/*SimParticle PFReconstructor::reconstructTrack(const Track& track) {
  // , Clusters = None): cluster argument does not ever seem to be used at present

  int pdgId = 211 * track.charge();
  TLorentzVector p4 = TLorentzVector();
  p4.SetVectM(track.p3(), ParticlePData::particleMass(pdgId));
  auto particle= SimParticle(pdgId, track.charge(), p4, track, 'r');

  m_locked[track.id()] = true;
  PDebug::write("Made Reconstructed{} from Smeared{}", particle, track);
  // std::cout << "made particle pdgid: " << particle.pdgId() << " from track: " << track;  // TODO << particle;
  return particle;
}*/

void PFReconstructor::reconstructTrack(const Track& track, int pdgId, Ids parentIds) {
  /*construct a charged hadron/electron/muon from the track
  */
  if (m_locked[track.id()]) return;
  // vertex = track.path().points()['vertex'];
  pdgId = pdgId * track.charge();
  // mass, charge = particle_data[pdgid]
  TLorentzVector p4 = TLorentzVector();
  p4.SetVectM(track.p3(), ParticlePData::particleMass(pdgId));
  // particle = Particle(p4, vertex, charge, pdgId, 'r');
  auto particle = SimParticle(pdgId, track.charge(), p4, track, 'r');
  //#todo fix this so it picks up smeared track points (need to propagagte smeared track)
  // particle.set_path(track.path)
  m_locked[track.id()] = true;
  PDebug::write("Made Reconstructed{} from Smeared{}", particle, track);
  insertParticle(parentIds, particle);
  // return particle;
}
}  // end namespace papas