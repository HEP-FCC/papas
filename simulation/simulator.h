//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  SIMULATOR_H
#define SIMULATOR_H
#include <string>
#include <unordered_map>
#include <map>
#include "enums.h"
#include "detector.h"
#include "datatypes.h"
#include "directedacyclicgraph.h"

#include "identifier.h"
#include "propagator.h"

class Particle;

typedef DAG::Node<long> simNode;
typedef std::unordered_map<long, simNode> Nodes; ///TODO rename to Nodes
typedef std::unordered_map<long, Cluster> Clusters;
//typedef std::unordered_set<long,Track> Tracks;
typedef std::unordered_map<long, Particle> Particles;

class Simulator {
public:
   Simulator(const BaseDetector&);  //AJRTODO add logger
   void simulate();
   void simulatePhoton(SimParticle& ptc);
   const Cluster& addECALCluster(SimParticle& ptc, double fraction = 1.,
                                 double csize = 0.);
   const Cluster& addHCALCluster(SimParticle& ptc, double fraction = 1.,
                                 double csize = 0.);
   const Cluster& smearECALCluster(const Cluster& parent);
   const Cluster& smearHCALCluster(const Cluster& parent);

   void addParticle(const SimParticle& ptc);
   const Clusters& getClusters() const {return m_clusters;} ;
   void Experiment();
   
   //Clusterset ExportSimulatedClusters() const;
   //Particleset ExportSimulatedParticles() const;

private:
   void Propagate(SimParticle& ptc,const SurfaceCylinder &); //more args needed
   long makeClusterID(fastsim::enumLayer layer  , fastsim::enumSubtype subtype); //move to private
   long makeParticleID(fastsim::enumSource source);
   const Cluster& addCluster(SimParticle& ptc, fastsim::enumLayer layer,
                             double fraction = 1., double csize = 0.);
   const Cluster& makeCluster(long clusterid, double energy, TVector3& pos,
                              double csize = 0.);
   const Cluster& addSmearedCluster(const Cluster& parent, const Cluster& smeared);
   const Cluster& makeSmearedCluster(const Cluster& parent,double energyresolution );
   void addNode(const long newid, const long parentid = 0);
   std::shared_ptr<const DetectorElement> getElem(fastsim::enumLayer layer);
   
   const BaseDetector& m_detector;
   // all clusters
   Clusters m_clusters;
   //etc Trackset m_Tracks
   //All particles
   Particles m_particles;
   //this will have identifier for everything that has
   //been simulated and so acts as a lookup table
   Nodes m_nodes;
   StraightLinePropagator m_propStraight;
   //HelixPropagator m_propHelix;
};




#endif






///Simulator
/*class LongHash
{
public:
   size_t operator()(const long l) const
   {
      return std::hash<long>()(l);
   }
};*/

/*
 class DAG
 {

 MergeClusters(layer)
 //find all nodes of type smearedcluster in a detectorElement
 //find any pairs that are too close
 // create new merged cluster and add in some edges to original clusters
 SmearClusters(layer)
 // find nodes of type cluster in a detectorElement
 // smear each cluster saving as new node with pointer to the SmearedCluster
 // add in link from cluster to smearedcluster
 SmearTracks
 //find nodes of type Tracker
 //smear each track and save as new node with pointer to SmearedTrack
 PTNode* AddParticleNode(PFParticle*,label="particle", type=pdgid)
 PTNode* AddClusterNode(Cluster*, label="cluster", type="merged/smeared/raw")
 PTNode* AddTrackNode(Track*, label="track", type="smeared/raw")
 PTNodeset GetEndClusters(layer)
 // returns list of end cluster nodes
 PTNodeset GetSmearedTracks()
 //returns list of smeared track nodes
 }





 Class ReconstructPolyTree: public PolyTree





 Class Reconstructor

 ReconstuctPolyTree graph

 FindDistances
 FindClosest
 MakeEdges
 AddClustersFromPolyTree
 AddTracksFromPolyTree
 BuildLinks
 MakeParticles
 Reconstruct
 Simplify

 SimulatePhoton
 PTNode* pt_particle= makeParticle;
 PTNode* pt_cluster_eval= makeCluster
 pt_graph.addEdgeParentChild(pt_particle, pt_cluster);


 MakeCluster(...)
 new cluster
 return makeNode(cluster)
 #endif


 Propogator* Simulator::propagator(Particle& ptc){
 bool is_neutral = abs(ptc.q())<0.5;
 //if (is_neutral)
 return prop_straight;
 //else return prop_helix;
 //return prop_straight if is_neutral else self.prop_helix
 }

 Simulator::propagate(ptc){
 //propagate the particle to all detector cylinders'''
 Propagator(ptc).propagate([ptc], m_detector.cylinders(),
 m_detector.elements['field'].magnitude)






 from heppy_fcc.fastsim.propagator import StraightLinePropagator, HelixPropagatorom heppy_fcc.fastsim.pfobjects import Cluster, SmearedCluster, SmearedTrack
 from heppy_fcc.fastsim.pfobjects import Particle as PFSimParticle

 from pfalgo.sequence import PFSequence
 import random
 import sys
 import copy
 import shelve

 from ROOT import TVector3

 def pfsimparticle(ptc):
 '''Create a PFSimParticle from a particle.
 The PFSimParticle will have the same p4, vertex, charge, pdg ID.
 '''
 tp4 = ptc.p4()
 vertex = TVector3()
 charge = ptc.q()
 pid = ptc.pdgid()
 return PFSimParticle(tp4, vertex, charge, pid)

 class Simulator(object):

 def __init__(self, detector, logger=None):
 self.verbose = True
 self.detector = detector
 if logger is None:
 import logging
 logging.basicConfig(level='ERROR')
 logger = logging.getLogger('Simulator')
 self.logger = logger
 self.prop_helix = HelixPropagator()
 self.prop_straight = StraightLinePropagator()

 def write_ptcs(self, dbname):
 db = shelve.open(dbname)
 db['ptcs'] = self.ptcs
 db.close()

 def reset(self):
 self.particles = None
 self.ptcs = None
 Cluster.max_energy = 0.
 SmearedCluster.max_energy = 0.

 def propagator(self, ptc):
 is_neutral = abs(ptc.q())<0.5
 return self.prop_straight if is_neutral else self.prop_helix

 def propagate(self, ptc):
 '''propagate the particle to all detector cylinders'''
 self.propagator(ptc).propagate([ptc], self.detector.cylinders(),
 self.detector.elements['field'].magnitude)

 def make_cluster(self, ptc, detname, fraction=1., size=None):
 '''adds a cluster in a given detector, with a given fraction of
 the particle energy.'''
 detector = self.detector.elements[detname]
 self.propagator(ptc).propagate_one(ptc,
 detector.volume.inner,
 self.detector.elements['field'].magnitude )
 if size is None:
 size = detector.cluster_size(ptc)
 cylname = detector.volume.inner.name
 cluster =  Cluster(ptc.p4().E()*fraction,
 ptc.points[cylname],
 size,
 cylname, ptc)
 ptc.clusters[cylname] = cluster
 return cluster

 def smear_cluster(self, cluster, detector, accept=False):
 '''Returns a copy of self with a smeared energy.
 If accept is False (default), returns None if the smeared
 cluster is not in the detector acceptance. '''
 eres = detector.energy_resolution(cluster.energy)
 energy = cluster.energy * random.gauss(1, eres)
 smeared_cluster = SmearedCluster( cluster,
 energy,
 cluster.position,
 cluster.size(),
 cluster.layer,
 cluster.particle )
 # smeared_cluster.set_energy(energy)
 if detector.acceptance(smeared_cluster) or accept:
 return smeared_cluster
 else:
 return None

 def smear_track(self, track, detector, accept=False):
 #TODO smearing depends on particle type!
 ptres = detector.pt_resolution(track)
 scale_factor = random.gauss(1, ptres)
 smeared_track = SmearedTrack(track,
 track.p3 * scale_factor,
 track.charge,
 track.path)
 if detector.acceptance(smeared_track) or accept:
 return smeared_track
 else:
 return None

 def simulate_photon(self, ptc):
 detname = 'ecal'
 ecal = self.detector.elements[detname]
 self.prop_straight.propagate_one(ptc,
 ecal.volume.inner)

 cluster = self.make_cluster(ptc, detname)
 smeared = self.smear_cluster(cluster, ecal)
 if smeared:
 ptc.clusters_smeared[smeared.layer] = smeared


 def simulate_electron(self, ptc):
 ecal = self.detector.elements['ecal']
 self.prop_helix.propagate_one(ptc,
 ecal.volume.inner,
 self.detector.elements['field'].magnitude )
 cluster = self.make_cluster(ptc, 'ecal')
 smeared_cluster = self.smear_cluster(cluster, ecal)
 if smeared_cluster:
 ptc.clusters_smeared[smeared_cluster.layer] = smeared_cluster
 smeared_track = self.smear_track(ptc.track,
 self.detector.elements['tracker'])
 if smeared_track:
 ptc.track_smeared = smeared_track


 def simulate_neutrino(self, ptc):
 self.propagate(ptc)

 def simulate_hadron(self, ptc):
 ecal = self.detector.elements['ecal']
 hcal = self.detector.elements['hcal']
 frac_ecal = 0.
 self.propagator(ptc).propagate_one(ptc,
 ecal.volume.inner,
 self.detector.elements['field'].magnitude)
 path_length = ecal.material.path_length(ptc)
 if path_length<sys.float_info.max:
 # ecal path length can be infinite in case the ecal
 # has lambda_I = 0 (fully transparent to hadrons)
 time_ecal_inner = ptc.path.time_at_z(ptc.points['ecal_in'].Z())
 deltat = ptc.path.deltat(path_length)
 time_decay = time_ecal_inner + deltat
 point_decay = ptc.path.point_at_time(time_decay)
 ptc.points['ecal_decay'] = point_decay
 if ecal.volume.contains(point_decay):
 frac_ecal = random.uniform(0., 0.7)
 cluster = self.make_cluster(ptc, 'ecal', frac_ecal)
 # For now, using the hcal resolution and acceptance
 # for hadronic cluster
 # in the ECAL. That's not a bug!
 smeared = self.smear_cluster(cluster, hcal)
 if smeared:
 ptc.clusters_smeared[smeared.layer] = smeared
 cluster = self.make_cluster(ptc, 'hcal', 1-frac_ecal)
 smeared = self.smear_cluster(cluster, hcal)
 if smeared:
 ptc.clusters_smeared[smeared.layer] = smeared
 if ptc.q()!=0:
 smeared_track = self.smear_track(ptc.track,
 self.detector.elements['tracker'])
 if smeared_track:
 ptc.track_smeared = smeared_track

 def simulate_muon(self, ptc):
 self.propagate(ptc)
 smeared_track = self.smear_track(ptc.track,
 self.detector.elements['tracker'])
 if smeared_track:
 ptc.track_smeared = smeared_track

 def smear_muon(self, ptc):
 self.propagate(ptc)
 smeared = copy.deepcopy(ptc)
 return smeared

 def smear_electron(self, ptc):
 ecal = self.detector.elements['ecal']
 self.prop_helix.propagate_one(ptc,
 ecal.volume.inner,
 self.detector.elements['field'].magnitude )
 smeared = copy.deepcopy(ptc)
 return smeared

 def simulate(self, ptcs):
 self.reset()
 self.ptcs = []
 smeared = []
 for gen_ptc in ptcs:
 ptc = pfsimparticle(gen_ptc)
 if ptc.pdgid() == 22:
 self.simulate_photon(ptc)
 elif abs(ptc.pdgid()) == 11:
 smeared_ptc = self.smear_electron(ptc)
 smeared.append(smeared_ptc)
 # self.simulate_electron(ptc)
 elif abs(ptc.pdgid()) == 13:
 smeared_ptc = self.smear_muon(ptc)
 smeared.append(smeared_ptc)
 # self.simulate_muon(ptc)
 elif abs(ptc.pdgid()) in [12,14,16]:
 self.simulate_neutrino(ptc)
 elif abs(ptc.pdgid()) > 100: #TODO make sure this is ok
 if ptc.q() and ptc.pt()<0.2:
 # to avoid numerical problems in propagation
 continue
 self.simulate_hadron(ptc)
 self.ptcs.append(ptc)
 self.pfsequence = PFSequence(self.ptcs, self.detector, self.logger)
 self.particles = copy.copy(self.pfsequence.pfreco.particles)
 self.particles.extend(smeared)

 if __name__ == '__main__':

 import math
 import logging
 from vectors import Point
 from detectors.CMS import cms
 from detectors.perfect import perfect
 from toyevents import monojet, particle
 from heppy_fcc.display.core import Display
 from heppy_fcc.display.geometry import GDetector
 from heppy_fcc.display.pfobjects import GTrajectories

 display_on = True
 detector = cms

 logging.basicConfig(level='INFO')
 logger = logging.getLogger('Simulator')
 logger.addHandler( logging.StreamHandler(sys.stdout) )

 for i in range(1):
 if not i%100:
 print i
 simulator = Simulator(detector, logger)
 # particles = monojet([211, -211, 130, 22, 22, 22], math.pi/2., math.pi/2., 2, 50)
 particles = [
 # particle(211, math.pi/2., math.pi/2., 100),
 #particle(211, math.pi/2 + 0.5, 0., 40.),
 # particle(130, math.pi/2., math.pi/2.+0., 100.),
 particle(22, math.pi/2., math.pi/2.+0.0, 10.)
 ]
 simulator.simulate(particles)

 if display_on:
 display = Display(['xy', 'yz',
 'ECAL_thetaphi',
 'HCAL_thetaphi'
 ])
 gdetector = GDetector(detector)
 display.register(gdetector, 0)
 gtrajectories = GTrajectories(simulator.ptcs)
 display.register(gtrajectories,1)
 display.draw()
 print "end"
 */