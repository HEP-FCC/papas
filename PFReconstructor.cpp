//
//  PFReconstructor.cpp
//  fastsim
//
//  Created by Alice Robson on 20/04/16.
//
//


#include "PFReconstructor.h"


#include <algorithm>
#include <unordered_map>
#include <vector>


#include "TVector3.h"
#include "TLorentzVector.h"

#include "Identifier.h"
#include "directedacyclicgraph.h"
#include "BlockSplitter.h"
#include "DataTypes.h"
#include "Path.h"
#include "PFEvent.h"
#include "PFBlock.h"
#include "Edge.h"

class PFevent;
class PFBlock;

//TODO simparticles is not right
typedef std::unordered_map<longID, SimParticle> Particles;
class PFReconstructor {
  
public:
  PFReconstructor();// TODO  detector, logger):
  void reconstruct(PFEvent& pfEvent);
  
  
private:
  void reconstructBlock(PFBlock& block);
  Blocks simplifyBlock(PFBlock& block);
  void reconstructHcal(const PFBlock& block, longID hcalID);
  void insertParticle(const PFBlock& block, SimParticle&& particle);
  SimParticle reconstructCluster(const Cluster& cluster, fastsim::enumLayer layer);
  SimParticle reconstructTrack(const Track& track);
  
  
  PFEvent & m_pfEvent;
  Blocks m_blocks;
  Nodes m_historyNodes;
  Particles m_particles;
  bool m_hasHistory;
  IDs m_unused;
  //IDs m_locked; // or vector of bools?
  std::unordered_map<longID , bool> m_locked;
  
};

void PFReconstructor::reconstruct(PFEvent& pfEvent) {
  m_blocks = pfEvent.blocks();
  
  //history nodes will be used to connect reconstructed particles into the history
  //its optional at the moment
  m_historyNodes = pfEvent.historyNodes();
  m_hasHistory = pfEvent.historyNodes().size() == 0;
  
  
  //TODO sort m_blocks
  
  
  // simplify the blocks by editing the links so that each track will end up linked to at most one hcal
  // then recalculate the blocks
  for (auto& block : m_blocks) {
    //std::cout "block: "<< block.second.short_name();
    Blocks newBlocks = simplifyBlock(block.second);
    if (newBlocks.size() >0) {
      m_blocks.insert(newBlocks.begin(), newBlocks.end());
    }
  }
  
  for (auto& block : m_blocks) {
    if (block.second.isActive()) { //when blocks are split the original gets deactivated
      reconstructBlock(block.second);
      
      //TODO move this bit to reconstructBlock
      for (auto& id : block.second.elementIDs()) {
        //if (std::find(m_locked.begin(), m_locked.end(), id) != m_locked.end()) { //TODO check for more efficient approach
        if (m_locked.find(id) != m_locked.end()) {
          m_unused.push_back(id);
        }
      }
    }
  }
  if (m_unused.size() > 0) {
    //TODO ouput warning message
  }
}


/*def _sorted_block_keys(self) :
 #sort blocks (1) by number of elements (2) by mix of ecal, hcal , tracks (the shortname will look like "H1T2" for a block
 #with one cluster and two tracks)
 return sorted(self.blocks.keys(), key=lambda k: (len(self.blocks[k].element_uniqueids), self.blocks[k].short_name()),reverse =True)
 */

Blocks PFReconstructor::simplifyBlock(PFBlock & block) {
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
  IDs ids=block.elementIDs();
  
  if (ids.size()<=1 ) {    //no links to remove
    return  splitBlocks;
  }
  
  /* work out any links that need to be removed
   - for tracks unink all hcals except the closest hcal
   - for ecals unlink hcals */
  Edges toUnlink; //TODO think about copy
  std::vector<long long> linkedEdgeKeys;
  bool firstHCAL;
  double firstDist = -1;
  
  
  for (auto id : ids) {
    if (Identifier::isTrack(id)) {
      linkedEdgeKeys = block.linkedEdgeKeys(id,Edge::EdgeType::kHcalTrack ) ;//TODO ensurealready sorted from small to large distance
      if (linkedEdgeKeys.size()>0) {
        firstHCAL = true;
        for (auto elem : linkedEdgeKeys) {
          if (firstHCAL) {
            firstDist = block.Edge(elem).distance();
            firstHCAL = false;
          }
          else if (block.Edge(elem).distance()==firstDist) { //link this track to the closest HCALs (could be more than one at zero distance)
            toUnlink[elem] = block.Edge(elem);
          }
        }
      }
    }
    else if (Identifier::isEcal(id)) {
      // this is now handled  elsewhere in  Ruler::distance and so could be removed
      // remove all ecal-hcal links. ecal linked to hcal give rise to a photon anyway.
      linkedEdgeKeys = block.linkedEdgeKeys(id,Edge::EdgeType::kEcalHcal ) ;//"ecal_hcal")
      for (auto  elem : linkedEdgeKeys) {
        toUnlink[elem] = block.Edge(elem);
        
      }
    }
  }
  
  //if there is something to unlink then use the BlockSplitter
  
  if (toUnlink.size()>0) {
    splitBlocks = BlockSplitter(toUnlink, block, m_historyNodes).blocks();
  }
  return splitBlocks;
}

void PFReconstructor::reconstructBlock(PFBlock& block) {
  /// see class description for summary of reconstruction approach
  
  IDs ids = block.elementIDs();
  for (auto id : ids) {
    m_locked[id] = false;
  }
  /*self.debugprint = False
   if (self.debugprint  and len(block.element_uniqueids)> 4):
   print  block*/
  
  
  if (ids.size() == 1 ) { //#TODO WARNING!!! LOTS OF MISSING CASES
    longID id = ids[0];
    if (Identifier::isEcal(id)) {
      insertParticle(block, reconstructCluster(m_pfEvent.ECALCluster(id),fastsim::enumLayer::ECAL));
    }
    else if(Identifier::isHcal(id)) {
      insertParticle(block, reconstructCluster(m_pfEvent.HCALCluster(id),fastsim::enumLayer::HCAL));
    }
    else if(Identifier::isTrack(id)) {
      insertParticle(block, reconstructTrack(m_pfEvent.track(id)));
    }
    else {// ask Colin about energy balance - what happened to the associated clusters that one would expect?
          //TODO
    }
  }
  else {
    for (auto id : ids ) {
      if (Identifier::isHcal(id)) {
        reconstructHcal(block, id);
      }
      for (auto id : ids ) {
        if (Identifier::isTrack(id) && ! m_locked[id]) {
          /* unused tracks, so not linked to HCAL
           # reconstructing charged hadrons.
           # ELECTRONS TO BE DEALT WITH.*/
          insertParticle(block, reconstructTrack(m_pfEvent.track(id)));
          for (auto idlink : block.linkedIDs(id,Edge::EdgeType::kEcalTrack) ) {
            //TODO ask colin what happened to possible photons here:
            //TODO add in extra photonsbut decide where they should go?
            m_locked[idlink] = true;
          }
        }
      }
    }
  }
}

void  PFReconstructor::insertParticle(const PFBlock& block, SimParticle&& newparticle) {
  /* The new particle will be inserted into the history_nodes (if present).
   A new node for the particle will be created if needed.
   It will have as its parents the block and all the elements of the block.
   '''
   #Note that although it may be possible to specify more closely that the particle comes from
   #some parts of the block, there are frequently ambiguities and so for now the particle is
   #linked to everything in the block*/
  //if (newparticle) :
  longID newid = newparticle.uniqueID();
  m_particles[newid] = newparticle;
  
  //check if history nodes exists
  if (!m_hasHistory)
    return;
  
  //find the node for the block
  PFNode blockNode = m_historyNodes[block.uniqueID()];
  
  //find or make a node for the particle
  if (m_historyNodes.find(newid) == m_historyNodes.end()) {
    m_historyNodes[newid] = PFNode(newid);
  }
  PFNode particleNode = m_historyNodes[newid];
  //link particle to the block
  blockNode.addChild( particleNode);
  //link particle to block elements
  for (auto element_id : block.elementIDs()) {
    m_historyNodes[element_id].addChild(particleNode);
  }
}

double PFReconstructor::neutralHadronEnergyResolution(const Cluster& hcal) const {
  /*WARNING CMS SPECIFIC!

http://cmslxr.fnal.gov/source/RecoParticleFlow/PFProducer/src/PFAlgo.cc#3350
*/
energy = max(hcal.energy, 1.)
stoch, const = 1.02, 0.065
if abs(hcal.position.Eta())>1.48:
stoch, const = 1.2, 0.028
resol = math.sqrt(stoch**2/energy + const**2)
return resol

def nsigma_hcal(self, cluster):
'''WARNING CMS SPECIFIC!

http://cmslxr.fnal.gov/source/RecoParticleFlow/PFProducer/src/PFAlgo.cc#3365
'''

return 1. + math.exp(-cluster.energy/100.)



def reconstruct_hcal(self, block, hcalid):
'''
block: element ids and edges
hcalid: id of the hcal being processed her

has hcal and has a track
-> add up all connected tracks, turn each track into a charged hadron
-> add up all ecal energies
-> if track energies is greater than hcal energy then turn the missing energies into an ecal (photon)
NB this links the photon to the hcal rather than the ecals
-> if track energies are less than hcal then make a neutral hadron with rest of hcal energy and turn all ecals into photons
has hcal but no track (nb by design there will be no attached ecals because hcal ecal links have been removed)
-> make a neutral hadron
has hcals
-> each hcal is treated using rules above
'''


# hcal used to make ecal_in has a couple of possible issues
tracks = []
ecals = []
hcal =block.pfevent.hcal_clusters[hcalid]

assert(len(block.linked_ids(hcalid, "hcal_hcal"))==0  )
trackids =    block.sort_distance_energy(hcalid, block.linked_ids(hcalid, "hcal_track") )
for trackid in  trackids:
tracks.append(block.pfevent.tracks[trackid])
for ecalid in block.linked_ids(trackid, "ecal_track"):
# the ecals get all grouped together for all tracks in the block
# Maybe we want to link ecals to their closest track etc?
# this might help with history work
# ask colin.
if not self.locked[ecalid]:
ecals.append(block.pfevent.ecal_clusters[ecalid])
self.locked[ecalid]  = True
# hcal should be the only remaining linked hcal cluster (closest one)
#thcals = [th for th in elem.linked if th.layer=='hcal_in']
#assert(thcals[0]==hcal)
self.log.info( hcal )
self.log.info( '\tT {tracks}'.format(tracks=tracks) )
self.log.info( '\tE {ecals}'.format(ecals=ecals) )
hcal_energy = hcal.energy
if len(tracks):
ecal_energy = sum(ecal.energy for ecal in ecals)
track_energy = sum(track.energy for track in tracks)
for track in tracks:
#make a charged hadron
self.insert_particle(block, self.reconstruct_track( track))

delta_e_rel = (hcal_energy + ecal_energy) / track_energy - 1.
# WARNING
# calo_eres = self.detector.elements['hcal'].energy_resolution(track_energy)
calo_eres = self.neutral_hadron_energy_resolution(hcal)
self.log.info( 'dE/p, res = {derel}, {res} '.format(
                                                    derel = delta_e_rel,
                                                    res = calo_eres ))
if delta_e_rel > self.nsigma_hcal(hcal) * calo_eres: # approx means hcal energy + ecal energies > track energies

excess = delta_e_rel * track_energy # energy in excess of track energies
#print( 'excess = {excess:5.2f}, ecal_E = {ecal_e:5.2f}, diff = {diff:5.2f}'.format(
#    excess=excess, ecal_e = ecal_energy, diff=excess-ecal_energy))
if excess <= ecal_energy: # approx means hcal energy > track energies
# Make a photon from the ecal energy
# We make only one photon using only the combined ecal energies
self.insert_particle(block, self.reconstruct_cluster(hcal, 'ecal_in',excess))

else: # approx means that hcal energy>track energies so we must have a neutral hadron
#excess-ecal_energy is approximately hcal energy  - track energies
self.insert_particle(block, self.reconstruct_cluster(hcal, 'hcal_in',
                                                     excess-ecal_energy))
if ecal_energy:
#make a photon from the remaining ecal energies
#again history is confusingbecause hcal is used to provide direction
#be better to make several smaller photons one per ecal?
self.insert_particle(block, self.reconstruct_cluster(hcal, 'ecal_in',
                                                     ecal_energy))

else: # case whether there are no tracks make a neutral hadron for each hcal
# note that hcal-ecal links have been removed so hcal should only be linked to
# other hcals

self.insert_particle(block,  self.reconstruct_cluster(hcal, 'hcal_in'))

self.locked[hcalid] = True


def reconstruct_cluster(self, cluster, layer, energy = None, vertex = None):
'''construct a photon if it is an ecal
construct a neutral hadron if it is an hcal
'''
if vertex is None:
vertex = TVector3()
pdg_id = None
if layer=='ecal_in':
pdg_id = 22 #photon
elif layer=='hcal_in':
pdg_id = 130 #K0
else:
raise ValueError('layer must be equal to ecal_in or hcal_in')
assert(pdg_id)
mass, charge = particle_data[pdg_id]
if energy is None:
energy = cluster.energy
if energy < mass:
return None
if (mass==0):
momentum= energy #avoid sqrt for zero mass
else:
momentum = math.sqrt(energy**2 - mass**2)
p3 = cluster.position.Unit() * momentum
p4 = TLorentzVector(p3.Px(), p3.Py(), p3.Pz(), energy) #mass is not accurate here
particle = Reconstructed_Particle(p4, vertex, charge, pdg_id)
path = StraightLine(p4, vertex)
path.points[layer] = cluster.position #alice: this may be a bit strange because we can make a photon with a path where the point is actually that of the hcal?
# nb this only is problem if the cluster and the assigned layer are different
particle.set_path(path)
particle.clusters[layer] = cluster  # not sure about this either when hcal is used to make an ecal cluster?
self.locked[cluster.uniqueid] = True #just OK but not nice if hcal used to make ecal.
if self.debugprint:
print "made particle from cluster ",pdg_id,  cluster, particle
return particle

def reconstruct_track(self, track, clusters = None): # cluster argument does not ever seem to be used at present
'''construct a charged hadron from the track
'''
vertex = track.path.points['vertex']
pdg_id = 211 * track.charge
mass, charge = particle_data[pdg_id]
p4 = TLorentzVector()
p4.SetVectM(track.p3, mass)
particle = Reconstructed_Particle(p4, vertex, charge, pdg_id)
particle.set_path(track.path)
particle.clusters = clusters
self.locked[track.uniqueid] = True
if self.debugprint:
print "made particle from track ", pdg_id, track, particle
return particle


def __str__(self):
theStr = ['New Rec Particles:']
theStr.extend( map(str, self.particles))
theStr.append('Unused:')
if len(self.unused)==0:
theStr.append('None')
else:
theStr.extend( map(str, self.unused))
return '\n'.join( theStr )
