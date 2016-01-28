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

typedef DAG::Node<long> SimNode;
typedef std::unordered_map<long, SimNode> Nodes; ///TODO rename to Nodes
typedef std::unordered_map<long, Cluster> Clusters;
typedef std::unordered_map<long,Track> Tracks;
typedef std::unordered_map<long, SimParticle> Particles;
typedef std::vector<long> IDs;

class Simulator {
   
public:
   Simulator(const BaseDetector&);  //AJRTODO add logger
   void simulate();
   void simulatePhoton(SimParticle& ptc);
   void simulateHadron(SimParticle& ptc);
   
   SimParticle& addParticle(int pdgid, TLorentzVector tlv, TVector3 vertex= TVector3(0., 0.,
                                                                                     0.));
   const Clusters& getClusters() const {return m_clusters;} ;
   void Experiment();
   
   //Clusterset ExportSimulatedClusters() const;
   //Particleset ExportSimulatedParticles() const;

private:
   void Propagate(SimParticle& ptc,const SurfaceCylinder &); //more args needed
   
   const IDs& getECALSmearedClusterIDs(long nodeid);
   const IDs& getRawTrackIDs(long nodeid);
   const IDs& getSmearedTrackIDs(long nodeid);
   const IDs& getParticleIDs (long nodeid);
   const IDs& getConnectedIDs(long nodeid);

   long makeClusterID(fastsim::enumLayer layer  , fastsim::enumSubtype subtype);
   long makeParticleID(fastsim::enumSource source);
   
   
   const Cluster& addECALCluster(SimParticle& ptc,long parentid=0, double fraction = 1.,
                                 double csize = 0.);
   const Cluster& addHCALCluster(SimParticle& ptc, long parentid=0,double fraction = 1.,
                                 double csize = 0.);
   const Cluster& addSmearedECALCluster(const Cluster& parent);
   const Cluster& addSmearedHCALCluster(const Cluster& parent);
   const Cluster& addCluster(SimParticle& ptc, long parentid,fastsim::enumLayer layer,
                            double fraction = 1., double csize = 0.);
   const Cluster& makeCluster(long clusterid, double energy, TVector3 pos,
                              double csize = 0.);
   const Cluster& makeSmearedCluster(const Cluster& parent,double energyresolution );
   
   
   

   const Track& addTrack(SimParticle& ptc);
   const Track& makeTrack(long trackid, TVector3 pos, double charge,const Path& path);
   
   void addNode(const long newid, const long parentid = 0);
   std::shared_ptr<const DetectorElement> getElem(fastsim::enumLayer layer);
   
   const IDs& getMatchingIDs(long nodeid, fastsim::enumDataType datatype, fastsim::enumLayer layer, fastsim::enumSubtype type, fastsim::enumSource source);
   
   Clusters m_clusters;    /// all clusters
   Tracks m_tracks;        /// all tracks
   Particles m_particles;  /// all particles
   
   //this will have identifier for everything that has
   //been simulated and so acts as a lookup table
   Nodes m_nodes;
   
   const BaseDetector& m_detector;
   StraightLinePropagator m_propStraight;
   HelixPropagator m_propHelix;
};

#endif

