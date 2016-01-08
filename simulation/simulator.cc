#include "simulator.h"
#include "detectors/CMS.h"

#include <iostream> //TODOAJR remove


/*
Simulator::Simulator(BaseDetector& d)
   : m_detector(d)
{
   //m_propStraightLine= StraightLinePropagator();
   //m_propHelix;
}
//
//  Created by Alice Robson on 09/11/15.
//
//

Cluster&& Simulator::makeCluster(Particle& ptc, const  std::string& detname,
                                 double fraction, double size)
{
   //adds a cluster in a given detector, with a given fraction of
   //the particle energy.'''
   std::shared_ptr<const DetectorElement> detectorElem = m_detector.getElement(
            detname)   ;
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


void  Simulator::simulatePhoton(Particle& ptc)
{
   const std::string detname("ECAL");
   //AJRTODO check that it is found OK
   std::shared_ptr<const DetectorElement>  ecal = m_detector.getElement(
            detname)   ;
   //m_propStraight.propagate_one(ptc,  ecal.volume.inner);

   Cluster cluster = makeCluster(ptc, detname);
   std::cout << "HELP??";
} */