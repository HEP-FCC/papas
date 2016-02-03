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
   : m_detector(d), m_propStraight(), m_propHelix(d.getField()->getMagnitude())
{
   //TODO think about sizing
   m_clusters.reserve(1000);
   m_particles.reserve(1000);
   m_tracks.reserve(1000);
   m_nodes.reserve(1000);

}

void  Simulator::simulatePhoton(SimParticle& ptc)
{
   auto ecal_sp=m_detector.getECAL();
   
   //propagate as far as the ECAL inner cylinder
   Propagate(ptc, ecal_sp->getVolumeCylinder().Inner());
   
   //make a cluster
   const Cluster& ecalclust = addECALCluster(ptc); //add the position into this
   
   //smear the cluster
   addSmearedECALCluster(ecalclust);
}

void  Simulator::simulateHadron(SimParticle& ptc)
{
   auto ecal_sp=m_detector.getECAL();
   auto hcal_sp=m_detector.getHCAL();
   auto field_sp=m_detector.getField();
   double frac_ecal =0.; //TODO ask Colin
   
   //make a track
   const Track& track=addTrack(ptc);
   //TODO addSmearedtrack....
   
   //propagate to the inner ECAL cyclinder
   Propagate(ptc, ecal_sp->getVolumeCylinder().Inner());
   double path_length=ecal_sp->getMaterial().getPathLength(ptc.IsCharged());
   
   //TO MATCH PYTHON$
   //path_length=0.273579537605;

   
   if ( path_length<std::numeric_limits<double>::max()) {
      
      /// ecal path length can be infinite in case the ecal
      /// has lambda_I = 0 (fully transparent to hadrons)
      Path& path =ptc.getPath();
      double time_ecal_inner = path.getTimeAtZ(path.getNamedPoint("_ECALin").Z());
      double deltat = path.getDeltaT(path_length);
      double time_decay = time_ecal_inner + deltat;
      TVector3 point_decay = path.getPointAtTime(time_decay);
      path.addPoint("ecal_decay", point_decay);
      if (ecal_sp->getVolumeCylinder().Contains(point_decay))
         {
         double frac_ecal = fastsim::randomUniform(0., 0.7); // could also have a static member number generator
                                                             //frac_ecal=0.35
         const Cluster& ecalclust = addECALCluster(ptc,track.getID(),frac_ecal);
            //For now, using the hcal resolution and acceptance for hadronic cluster
            //in the ECAL. That's not a bug!
         addSmearedHCALCluster(ecalclust); // TODO ask colin why(cluster, hcal)
                                           //name of call needs to be improved
      }
   }
   
   //now move into HCAL
   Propagate(ptc, hcal_sp->getVolumeCylinder().Inner());
   //TODO ask colin if the parent is the track or the ecalclust
   const Cluster& hcalclust = addHCALCluster(ptc,track.getID(), 1-frac_ecal);
   addSmearedHCALCluster(hcalclust);
   
   
}

void  Simulator::Propagate(SimParticle& ptc, const SurfaceCylinder & sc)
{
   bool is_neutral= fabs(ptc.getCharge())<0.5; //TODO ask colin why not zero
   if (is_neutral)
      m_propStraight.propagateOne(ptc, sc);
   else
      m_propHelix.propagateOne(ptc, sc);
   
}


SimParticle& Simulator::addParticle( int pdgid, TLorentzVector tlv, TVector3 vertex)
{
   double field=m_detector.getField()->getMagnitude();
   long uniqueid=Identifier::makeParticleID(fastsim::enumSource::SIMULATION);
   m_particles.emplace(uniqueid,SimParticle{uniqueid,pdgid,tlv,field,vertex});
   addNode(uniqueid); //add node to graph
   return m_particles[uniqueid];
}


const Cluster& Simulator::addECALCluster(SimParticle& ptc, long parentid, double fraction,
                                         double csize)
{
   if(!parentid)
      parentid=ptc.getID();
   if (csize == 0) { //or could make the defalt value -1?? check Colin
      csize = m_detector.getECAL()->clusterSize(ptc);
   }
   return addCluster(ptc,  parentid, fastsim::enumLayer::ECAL, fraction, csize);
}

const Cluster& Simulator::addHCALCluster(SimParticle& ptc,long parentid,double fraction,
                                         double csize)
{
   if(!parentid)
      parentid=ptc.getID();
   if (csize == 0) { //or could make the defalt value -1?? check Colin
      csize = m_detector.getHCAL()->clusterSize(ptc);
   }
   return addCluster(ptc, parentid, fastsim::enumLayer::HCAL, fraction, csize);
}


const Cluster& Simulator::addCluster(SimParticle& ptc, long parentid,fastsim::enumLayer layer,
                                     double fraction, double csize)
{
   //TODO change string to ENUM
   std::string clyname = m_detector.getElement(layer)->getVolumeCylinder().InnerName();
   long clusterid = Identifier::makeClusterID(layer, fastsim::enumSubtype::RAW);
   double energy = ptc.getP4().E() * fraction;
   TVector3 pos = ptc.getPathPosition(clyname); //assume path already set in particle
   
   const Cluster& cluster = makeCluster(clusterid, energy, pos, csize);
   addNode(cluster.getID(), parentid); //a track may be the parent of a cluster
   return cluster; //check this defaults OK
}


const Cluster& Simulator::makeCluster(long clusterid, double energy,
                                      TVector3 pos, double csize)
{
   ///create cluster and add into the map of clusters
   m_clusters.emplace(clusterid, Cluster{energy, pos, csize, clusterid});
   return m_clusters[clusterid];
}



const Cluster& Simulator::addSmearedECALCluster(const Cluster& parent)
{
   auto ECAL=m_detector.getECAL();
   double eres=ECAL->energyResolution(parent.getEnergy());
   
   const Cluster& smeared=makeSmearedCluster(parent,eres);
   if (ECAL->acceptance(smeared)) {
      addNode(smeared.getID(), parent.getID());
      return smeared;
   }
   else {
      m_clusters.erase(smeared.getID());
      return parent;
   }
}

const Cluster& Simulator::addSmearedHCALCluster(const Cluster& parent)
{
   auto HCAL=m_detector.getHCAL();
   double eres=HCAL->energyResolution(parent.getEnergy());
   
   const Cluster& smeared=makeSmearedCluster(parent,eres);
   if (HCAL->acceptance(smeared)) {
      addNode(smeared.getID(), parent.getID());
      return smeared;
   }
   else {
      m_clusters.erase(smeared.getID());
      return parent;
   }
}


const Cluster& Simulator::makeSmearedCluster(const Cluster& parent,double energyresolution )
{
   //create a new id
   auto layer=Identifier::getLayer(parent.getID());
   long newclusterid = Identifier::makeClusterID(layer,
                                                 fastsim::enumSubtype::SMEARED);
   double energy = parent.getEnergy() * fastsim::randomGaussian(1, energyresolution);
   //TODO temp remove
   //energy=7.353;
   const Cluster& cluster = makeCluster(newclusterid, energy, parent.getPosition(), parent.getSize());
   return cluster;
}

const Track& Simulator::addTrack(SimParticle& ptc)
{
   long trackid = Identifier::makeTrackID();
   const Track& track = makeTrack(trackid, ptc.getP3(), ptc.getCharge(), ptc.getPath());
   addNode(track.getID(), ptc.getID());
   return track; //check this defaults OK
}

const Track& Simulator::makeTrack(long trackid, TVector3 pos, double charge, Path& path)
{
   m_tracks.emplace(trackid, Track{ pos, charge,path ,trackid});
   return m_tracks[trackid];
}


void Simulator::addNode(long newid, const long parentid)
{

   //add the new node into the set of all nodes
   m_nodes[newid] = {newid};

   if (parentid) {
      SimNode& parent = m_nodes[parentid];
      SimNode& child = m_nodes[newid];
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
   
   DAG::BFSVisitor<SimNode> bfs;
   for ( auto p  : m_particles)
   {
      std::cout<< "Connected to "<<p.first<< std::endl;
      auto res =bfs.traverseUndirected(m_nodes[p.first]);
      for (auto r : res)
         std::cout<< "  "<<r->getValue()<<": "<<Identifier::getDataType(r->getValue()) <<": " <<Identifier::getSubtype(r->getValue())<<std::endl;
   }
}

IDs Simulator::getLinkedECALSmearedClusterIDs(long nodeid) {
   return getMatchingIDs(nodeid,
                         fastsim::enumDataType::CLUSTER,
                         fastsim::enumLayer::ECAL,
                         fastsim::enumSubtype::SMEARED,
                         fastsim::enumSource::SIMULATION);
   //return ids;
}

IDs  Simulator::getLinkedRawTrackIDs(long nodeid) {
   return getMatchingIDs(nodeid,
                         fastsim::enumDataType::TRACK,
                         fastsim::enumLayer::NONE,
                         fastsim::enumSubtype::RAW,
                         fastsim::enumSource::SIMULATION);
}
IDs  Simulator::getLinkedSmearedTrackIDs(long nodeid) {
   return getMatchingIDs(nodeid,
                         fastsim::enumDataType::TRACK,
                         fastsim::enumLayer::NONE,
                         fastsim::enumSubtype::SMEARED,
                         fastsim::enumSource::SIMULATION);
}

IDs  Simulator::getLinkedParticleIDs(long nodeid) {
   return getMatchingIDs(nodeid,
                         fastsim::enumDataType::PARTICLE,
                         fastsim::enumLayer::NONE,
                         fastsim::enumSubtype::RAW,
                         fastsim::enumSource::SIMULATION);
}

IDs  Simulator::getParentParticleIDs(long nodeid) {
   return getMatchingParentIDs(nodeid,
                         fastsim::enumDataType::PARTICLE,
                         fastsim::enumLayer::NONE,
                         fastsim::enumSubtype::RAW,
                         fastsim::enumSource::SIMULATION);
}



IDs  Simulator::getLinkedIDs(long nodeid) {
   DAG::BFSVisitor<SimNode> bfs;
   IDs foundids;
   foundids.reserve(1000); //TODO how
   auto res =bfs.traverseUndirected(m_nodes[nodeid]);
   for (auto r : res)
   {
      foundids.push_back(r->getValue());
   }
   return foundids;
}

IDs Simulator::getMatchingIDs(long nodeid, fastsim::enumDataType datatype, fastsim::enumLayer layer, fastsim::enumSubtype type,fastsim::enumSource source)
{
   DAG::BFSVisitor<SimNode> bfs;
   IDs foundids;
   //foundids.reserve(1000); //TODO set sizes sensible.... how
   auto res =bfs.traverseUndirected(m_nodes[nodeid]);
   for (auto r : res)
   {
      long id=r->getValue();
      if(Identifier::isUniqueIDMatch(id, datatype,layer, type,source)) {
         foundids.push_back(id);
      }
   }
   return foundids;
}

IDs Simulator::getMatchingParentIDs(long nodeid, fastsim::enumDataType datatype, fastsim::enumLayer layer, fastsim::enumSubtype type,fastsim::enumSource source)
{
   DAG::BFSVisitor<SimNode> bfs;
   IDs foundids;
   //foundids.reserve(1000); //TODO set sizes sensible.... how
   auto res =bfs.traverseParents(m_nodes[nodeid]);
   for (auto r : res)
   {
      long id=r->getValue();
      if(Identifier::isUniqueIDMatch(id, datatype,layer, type,source)) {
         foundids.push_back(id);
      }
   }
   return foundids;
}




