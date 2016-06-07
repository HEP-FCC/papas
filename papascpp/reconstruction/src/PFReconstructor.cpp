//
//  PFReconstructor.cpp
//  fastsim
//
//  Created by Alice Robson on 20/04/16.
//
//

#include "BlockSplitter.h"
#include "Cluster.h"
#include "Edge.h"
#include "Log.h"
#include "PFBlock.h"
#include "PFEvent.h"
#include "PFParticle.h"
#include "PFReconstructor.h"
#include "ParticlePData.h"
#include "Path.h"
#include "TLorentzVector.h"
#include "pTrack.h"
#include <algorithm>
#include <math.h>
#include <vector>

namespace papas {

PFReconstructor::PFReconstructor(PFEvent& pfEvent)
    : m_pfEvent(pfEvent),
      m_historyNodes(pfEvent.historyNodes()),
      m_blocks(pfEvent.blocks()),
      m_hasHistory(pfEvent.historyNodes().size() == 0) {}

void PFReconstructor::reconstruct() {
  // TODO sort m_blocks

  // simplify the blocks by editing the links
  // each track will end up linked to at most one hcal

  for (auto& block : m_blocks) {
    // std::cout << "Reconstrblock: " << block.second.shortName()<<std::endl;
    Blocks newBlocks = simplifyBlock(block.second);
    if (newBlocks.size() > 0) {
      for (auto& b : newBlocks)
        PDebug::write("Made {}", b.second);
      m_blocks.insert(newBlocks.begin(), newBlocks.end());
    }
  }

  for (auto& block : m_blocks) {
    if (block.second.isActive()) {  // when blocks are split the original gets deactivated
      std::cout << "Reconstructing block: " << block.second.shortName() << std::endl;

      reconstructBlock(block.second);
    }
  }
  if (m_unused.size() > 0) {
    std::cout << "unused elements";
    // TODO ouput warning message
  }
}

/*def _sorted_block_keys(self) :
 #sort blocks (1) by number of elements (2) by mix of ecal, hcal , tracks (the shortname will look like "H1T2" for a
 block
 #with one cluster and two tracks)
 return sorted(self.blocks.keys(), key=lambda k: (len(self.blocks[k].element_uniqueids),
 self.blocks[k].short_name()),reverse =True)
 */

Blocks PFReconstructor::simplifyBlock(PFBlock& block) {
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
  Ids ids = block.elementIds();

  if (ids.size() <= 1) {  // no links to remove
    return splitBlocks;
  }

  /* work out any links that need to be removed
   - for tracks unink all hcals except the closest hcal
   - for ecals unlink hcals */
  Edges toUnlink;  // TODO think about copy
  std::vector<Edge::EdgeKey> linkedEdgeKeys;
  bool firstHCAL;
  double firstDist = -1;

  for (auto id : ids) {
    if (Id::isTrack(id)) {
      linkedEdgeKeys = block.linkedEdgeKeys(
          id, Edge::EdgeType::kHcalTrack);  // TODO ensurealready sorted from small to large distance
      if (linkedEdgeKeys.size() > 0) {
        firstHCAL = true;
        for (auto elem : linkedEdgeKeys) {
          if (firstHCAL) {
            firstDist = block.Edge(elem).distance();
            firstHCAL = false;
          } else if (block.Edge(elem).distance() ==
                     firstDist) {  // link this track to the closest HCALs (could be more than one at zero distance)
            toUnlink[elem] = block.Edge(elem);
          }
        }
      }
    } else if (Id::isEcal(id)) {
      // this is now handled  elsewhere in  Ruler::distance and so could be removed
      // remove all ecal-hcal links. ecal linked to hcal give rise to a photon anyway.
      linkedEdgeKeys = block.linkedEdgeKeys(id, Edge::EdgeType::kEcalHcal);  //"ecal_hcal")
      for (auto elem : linkedEdgeKeys) {
        toUnlink[elem] = block.Edge(elem);
      }
    }
  }

  // if there is something to unlink then use the BlockSplitter

  if (toUnlink.size() > 0) {
    splitBlocks = BlockSplitter(toUnlink, block, m_historyNodes).blocks();
  }
  return splitBlocks;
}

void PFReconstructor::reconstructBlock(const PFBlock& block) {
  /// see class description for summary of reconstruction approach

  Ids ids = block.elementIds();
  for (auto id : ids) {
    m_locked[id] = false;
  }
  /*self.debugprint = False
   if (self.debugprint  and len(block.element_uniqueids)> 4):
   print  block*/

  if (ids.size() == 1) {  //#TODO WARNING!!! LOTS OF MISSING CASES
    Id::Type id = ids[0];
    if (Id::isEcal(id)) {
      insertParticle(block, reconstructCluster(m_pfEvent.ECALCluster(id), papas::Layer::kEcal));
    } else if (Id::isHcal(id)) {
      insertParticle(block, reconstructCluster(m_pfEvent.HCALCluster(id), papas::Layer::kHcal));
    } else if (Id::isTrack(id)) {
      insertParticle(block, reconstructTrack(m_pfEvent.track(id)));
    } else {  // ask Colin about energy balance - what happened to the associated clusters that one would expect?
              // TODO
    }
  } else {
    for (auto id : ids) {
      if (Id::isHcal(id)) {
        reconstructHcal(block, id);
      }
    }
    for (auto id : ids) {
      if (Id::isTrack(id) && !m_locked[id]) {
        /* unused tracks, so not linked to HCAL
         # reconstructing charged hadrons.
         # ELECTRONS TO BE DEALT WITH.*/
        insertParticle(block, reconstructTrack(m_pfEvent.track(id)));
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

void PFReconstructor::insertParticle(const PFBlock& block, PFParticle&& newparticle) {
  /* The new particle will be inserted into the history_nodes (if present).
   A new node for the particle will be created if needed.
   It will have as its parents the block and all the elements of the block.
   '''
   #Note that although it may be possible to specify more closely that the particle comes from
   #some parts of the block, there are frequently ambiguities and so for now the particle is
   #linked to everything in the block*/
  // if (newparticle) :
  Id::Type newid = newparticle.id();
  m_particles.emplace(newid, std::move(newparticle));

  // check if history nodes exists
  if (!m_hasHistory) return;

  // find the node for the block
  PFNode blockNode = m_historyNodes[block.uniqueId()];

  // find or make a node for the particle
  if (m_historyNodes.find(newid) == m_historyNodes.end()) {
    m_historyNodes[newid] = PFNode(newid);
  }
  PFNode particleNode = m_historyNodes[newid];
  // link particle to the block
  blockNode.addChild(particleNode);
  // link particle to block elements
  for (auto element_id : block.elementIds()) {
    m_historyNodes[element_id].addChild(particleNode);
  }
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

void PFReconstructor::reconstructHcal(const PFBlock& block, Id::Type hcalId) {
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
  /*# hcal should be the only remaining linked hcal cluster (closest one)
   #thcals = [th for th in elem.linked if th.layer=='hcal_in']
   #assert(thcals[0]==hcal)
   self.log.info( hcal )
   self.log.info( '\tT {tracks}'.format(tracks=tracks) )
   self.log.info( '\tE {ecals}'.format(ecals=ecals) )*/
  const Cluster& hcal = m_pfEvent.hcalClusters().at(hcalId);  // avoid copy
  double hcalEnergy = hcal.energy();
  double ecalEnergy = 0.;
  double trackEnergy = 0.;

  if (!trackIds.empty()) {
    for (auto id : trackIds) {
      const Track& track = m_pfEvent.tracks().at(id);
      insertParticle(block, reconstructTrack(track));
      trackEnergy += track.energy();
      for (auto id : ecalIds) {
        ecalEnergy += m_pfEvent.ECALCluster(id).energy();
      }
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
        insertParticle(block, reconstructCluster(hcal, papas::Layer::kEcal, excess));
      }

      else {  // approx means that hcal energy>track energies so we must have a neutral hadron
              // excess-ecal_energy is approximately hcal energy  - track energies
        insertParticle(block, reconstructCluster(hcal, papas::Layer::kHcal, excess - ecalEnergy));
        if (ecalEnergy) {
          // make a photon from the remaining ecal energies
          // again history is confusingbecause hcal is used to provide direction
          // be better to make several smaller photons one per ecal?
          insertParticle(block, reconstructCluster(hcal, papas::Layer::kEcal, ecalEnergy));
        }
      }
    }
  } else {  // # case whether there are no tracks make a neutral hadron for each hcal
            //# note that hcal-ecal links have been removed so hcal should only be linked to
            //# other hcals

    insertParticle(block, reconstructCluster(hcal, papas::Layer::kHcal));
  }
  m_locked[hcalId] = true;
}

PFParticle
PFReconstructor::reconstructCluster(const Cluster& cluster, papas::Layer layer, double energy, TVector3 vertex) {
  // construct a photon if it is an ecal
  // construct a neutral hadron if it is an hcal
  int pdgId = -1;
  if (energy < 0) {
    energy = cluster.energy();
  }
  if (layer == papas::Layer::kEcal) {
    pdgId = 22;  // photon
  } else if (layer == papas::Layer::kHcal) {
    pdgId = 130;  // K0
  } else {
    // TODO raise ValueError('layer must be equal to ecal_in or hcal_in')
  }
  // assert(pdg_id)
  double mass = ParticlePData::particleMass(pdgId);
  // double charge = ParticlePData::particleCharge(pdgId);
  if (energy < mass)  // null particle
    return PFParticle();

  double momentum;
  if (mass == 0) {
    momentum = energy;
  }  //#avoid sqrt for zero mass
  else {
    momentum = sqrt(pow(energy, 2) - pow(mass, 2));
  }
  TVector3 p3 = cluster.position().Unit() * momentum;
  TLorentzVector p4 = TLorentzVector(p3.Px(), p3.Py(), p3.Pz(), energy);  // mass is not accurate here

  Id::Type newid = Id::makeRecParticleId();
  // TODO check field and charge match?????
  PFParticle particle{newid, pdgId, p4, vertex};
  // TODO figure out if this is needed
  // Path path{p4, vertex}; //default is strightline path

  // TODO make addpoint use layer also??
  particle.path()->addPoint(papas::Position::kEcalIn, cluster.position());  // alice: Colin this may be a bit strange
                                                                            // because we can make a photon with a
                                                                            // path where the point is actually that
                                                                            // of the hcal?
  // nb this only is problem if the cluster and the assigned layer are different
  // particle.setPath(path);
  // particle.clusters[layer] = cluster  # not sure about this either when hcal is used to make an ecal cluster?
  m_locked[cluster.id()] = true;  // alice : just OK but not nice if hcal used to make ecal.
  // if self.debugprint:
  PDebug::write("Made Reconstructed{} from Smeared{}", particle, cluster);

  std::cout << "made particle pdgid: " << pdgId << " from cluster: " << cluster;  // TODO << particle;
  return particle;
}

PFParticle PFReconstructor::reconstructTrack(
    const Track& track) {  // Cclusters = None): # cluster argument does not ever seem to be used at present
                           /*construct a charged hadron from the track
                            */
  
  Id::Type newid = Id::makeRecParticleId();
  int pdgId = 211 * track.charge();
  TLorentzVector p4 = TLorentzVector();
  p4.SetVectM(track.p3(), ParticlePData::particleMass(pdgId));
  PFParticle particle{newid, pdgId, p4, track};
  // particle.setPath(track.path());
  // particle.clusters = clusters
  m_locked[track.id()] = true;
  PDebug::write("Made Reconstructed{} from Smeared{}", particle, track);
  std::cout << "made particle pdgid: " << particle.pdgId() << " from track: " << track;  // TODO << particle;
  return particle;
}
}  // end namespace papas

/*
 def __str__(self):
 theStr = ['New Rec Particles:']
 theStr.extend( map(str, self.particles))
 theStr.append('Unused:')
 if len(self.unused)==0:
 theStr.append('None')
 else:
 theStr.extend( map(str, self.unused))
 return '\n'.join( theStr )
 */