//
//  Created by Alice Robson on 09/11/15.
//
//
#include <iostream> //TODOAJR remove
#include "simulator.h"
#include "CMS.h"
#include "identifier.h"
#include "enums.h"

using fastsim::enumLayer;

Simulator::Simulator(const BaseDetector& d)
   : m_detector(d), m_propStraight()
{
   //m_propStraightLine= StraightLinePropagator();
   //m_propHelix;
}

void  Simulator::simulatePhoton(SimParticle& ptc)
{

   const SurfaceCylinder& S = m_detector.getECAL()->getVol().Inner();
   Propagate(ptc, S);
   const Cluster& ecalclust = addECALCluster(ptc); //add the position into this
   smearECALCluster(ecalclust);
}

void  Simulator::Propagate(SimParticle& ptc, const SurfaceCylinder & sc)
{
   
   m_propStraight.propagateOne(ptc, sc);
}

void Simulator::addParticle(const SimParticle& ptc)
{
   long nodeid = ptc.getID();
   m_particles[nodeid] = ptc; //add particle into map of particles
   addNode(nodeid); //add node to graph
}


const Cluster& Simulator::addCluster(SimParticle& ptc, fastsim::enumLayer layer,
                                     double fraction, double csize)
{
   //TODO ENUM?
   std::string clyname = m_detector.getElement(layer)->getVol().InnerName();
   long clusterid = Identifier::makeClusterID(layer, fastsim::enumSubtype::RAW);
   double energy = ptc.getP4().E() * fraction;

   if (csize == 0) { //or could make the defalt value -1?? check Colin
      csize = m_detector.getECAL()->clusterSize(ptc);
   }
   TVector3 pos = ptc.getPathPosition(
                     clyname); //assume path already set in particle

   const Cluster& cluster = makeCluster(clusterid, energy, pos, csize);
   addNode(cluster.getID(), ptc.getID());
   return cluster; //check this defaults OK
}


const Cluster& Simulator::addECALCluster(SimParticle& ptc, double fraction,
      double csize)
{
   return addCluster(ptc, fastsim::enumLayer::ECAL, fraction, csize);
}

const Cluster& Simulator::addHCALCluster(SimParticle& ptc, double fraction,
      double csize)
{
   return addCluster(ptc, fastsim::enumLayer::HCAL, fraction, csize);
}

const Cluster& Simulator::makeCluster(long clusterid, double energy,
                                      TVector3& pos, double csize)
{
   ///create cluster and add into the map of clusters
   m_clusters.emplace(clusterid, Cluster{energy, pos, csize, clusterid});
   return m_clusters[clusterid];
}

const Cluster& Simulator::smearECALCluster(const Cluster& parent)
{
   auto ECAL=m_detector.getECAL();
   double eres=ECAL->energyResolution(parent.getEnergy());
   
   Cluster smeared=makeSmearedCluster(parent,eres);
   if (ECAL->acceptance(smeared))
   {
      return addSmearedCluster(parent, smeared);//make a new cluster and add into the clusterset
   }
   else
      return parent;
}

const Cluster& Simulator::smearHCALCluster(const Cluster& parent)
{
   auto HCAL=m_detector.getHCAL();
   double eres=HCAL->energyResolution(parent.getEnergy());
   
   const Cluster& smeared=makeSmearedCluster(parent,eres);
   if (HCAL->acceptance(smeared))
   {
      return addSmearedCluster(parent, smeared);//make a new cluster and add into the clusterset
   }
   else
      return parent;
}
const Cluster& Simulator::makeSmearedCluster(const Cluster& parent,double energyresolution )
{
   //create a new id
   auto layer=Identifier::getLayer(parent.getID());
   long newclusterid = Identifier::makeClusterID(layer,
                                                 fastsim::enumSubtype::SMEARED);
   double energy = parent.getEnergy() * fastsim::randomGaussian(1, energyresolution);
   TVector3 pos=parent.getPosition();
   const Cluster& smeared{energy,std::move(pos) , parent.getSize(), newclusterid};
   
   return std::move(smeared);
}

const Cluster& Simulator::addSmearedCluster(const Cluster& parent,const Cluster& smeared )
{

   //create a new node and connect into the graph
   long newclusterid=smeared.getID();
   addNode(newclusterid, parent.getID());
   m_clusters.emplace(newclusterid,smeared);
   return m_clusters[newclusterid];
}


void Simulator::addNode(long newid, const long parentid)
{

   //add the new node into the set of all nodes
   m_nodes[newid] = {newid};

   if (parentid) {
      simNode& parent = m_nodes[parentid];
      simNode& child = m_nodes[newid];
      parent.addChild(child);
   }
}

std::shared_ptr<const DetectorElement> Simulator::getElem(
   fastsim::enumLayer layer)
{
   return m_detector.getElement(layer);
}

void Simulator::Experiment()
{
   
   DAG::BFSVisitor<simNode> bfs;
   for ( auto p  : m_particles)
   {
      std::cout<< "Connected to "<<p.first<< std::endl;
      auto res =bfs.traverseUndirected(m_nodes[p.first]);
      for (auto r : res)
         std::cout<< "                "<<r->getValue()<<std::endl;
   }
}
/*
const DetectorElement& Simulator::getElem(fastsim::enumLayer layer)
{
   return m_detector.getElement(layer);
}*/





/*

 Cluster&& Simulator::makeCluster(Particle& ptc, fastsim::enumLayer layer,
 double fraction, double size)
 {
 //adds a cluster in a given detector, with a given fraction of
 //the particle energy.'''
 std::shared_ptr<const DetectorElement> detectorElem = m_detector.getElement(
 layer)   ;

 //propagator(ptc).propagate_one(ptc,
 //detectorElem.volume.inner)

 //AJRTODO this comes next
 //m_propStraightLine.propagate_one(ptc,detectorElem->volume.inner);

 // last field does not appear to be used                                   m_detector.elements['field'].magnitude )
 if (size == 0.) {
 //    size = detectorElem->cluster_size(ptc);
 }
 std::string cylname = detectorElem->getVolume()->InnerName();
 // short term simplification
 TVector3* tvec = new TVector3(.5, .5,
 .5); //AJRTODO must make cluster remove the tvec
 Cluster cluster(0.8,
 *tvec,
 size,
 cylname, ptc);

 PTNode* = graph.addcluster(cluster*, "cluster","smeared","ECAL");



 ///Cluster cluster(ptc.p4().E()*fraction,
 //                 ptc.points[cylname],
 size,
 cylname, ptc);



 //ptc.clusters[cylname] = cluster;
 return std::move(cluster);
 }

 */






