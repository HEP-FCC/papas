//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @file CMS.cc
 * @brief Implementation of the CMS detector
 */
#include <cmath>
#include <vector>
#include "CMS.h"
#include "material.h"
#include "geometry.h"
#include "datatypes.h"

/*
 CMSECAL::CMSECAL(fastsim::enumLayer layer, const VolumeCylinder& volume,
 const  Material& material , double eta_crack,
 double emin, const std::vector<double>& eres) :
 DetectorElement(layer,   volume,  material), m_eta_crack(eta_crack), m_emin(emin),
 m_eres(eres)
 {}*/

CMSECAL::CMSECAL(fastsim::enumLayer layer, const VolumeCylinder&& volume,
           const Material&& material, double eta_crack,
           double emin, const std::vector<double>&& eres) :
   ECAL(layer,   volume,  material), m_eta_crack(eta_crack),
   m_emin(emin),
   m_eres(eres)
{}


//will need to be rewritten for different detectors
/**
 Cluster_size as a function of the type of particle
 @param ptc particle
 @return size of resulting cluster
 */
double CMSECAL::clusterSize(const Particle& ptc) const
{
   //simplified version TODOAJR
   int pdgid = 22 ; //= abs(ptc->pdgid()) //AJRTODO implement particle
   if (pdgid == 22 | pdgid == 11)
      return 0.04;
   else
      return 0.07;
}

/* Decides whether a cluster will be seen by a detector
 @param cluster the cluster to be analysed
 @return true is cluster is detected, false it if is too small to be seen
 */
bool CMSECAL::acceptance(const Cluster& cluster) const
{
   double energy = cluster.getEnergy();
   double eta = fabs(cluster.getEta());
   if (eta < m_eta_crack)
      return energy > m_emin;
   else if (eta < 3.)
      return (energy > m_emin &  cluster.getPt() > 0.5);
   else
      return false;
}


double CMSECAL::energyResolution(double energy) const
{
   double stoch = m_eres[0] / sqrt(energy);
   double noise = m_eres[1] / energy;
   double constant = m_eres[2];
   return sqrt(pow(stoch, 2) + pow(noise, 2) + pow(constant, 2));
}


CMS::CMS() : BaseDetector()
{
   //ECAL detector Element
   fastsim::enumLayer layer = fastsim::enumLayer::ECAL;
   m_ECAL = std::shared_ptr<const ECAL> {
      new CMSECAL(layer,
      VolumeCylinder(fastsim::to_str(layer), 1.55, 2.1, 1.30, 2),
      Material(layer, 8.9e-3, 0.275),
      0.15, // eta_crack
      0.4, //emin
      std::vector<double> {.073, .1, .005}
              )
   };

   //HCAL detector element
   layer = fastsim::enumLayer::HCAL;
   m_HCAL = std::shared_ptr<const HCAL> {
      new CMSHCAL(layer,
      VolumeCylinder(fastsim::to_str(layer), 2.9, 3.6, 1.9, 2.6),
      Material(layer, 0.0, 0.175),
      std::vector<double> {1.1, 0., 0.})
   };
   
   //Tracker detector element
   layer = fastsim::enumLayer::TRACKER;
   m_Tracker = std::shared_ptr<const Tracker> {
   new CMSTracker(layer,
                  VolumeCylinder(fastsim::to_str(layer), 1.29, 1.99))
   };
   
   //Field detector element
   layer = fastsim::enumLayer::FIELD;
   m_Field = std::shared_ptr<const Field> {
      new CMSField(layer,
                   VolumeCylinder(fastsim::to_str(layer), 2.9, 3.6),3.8)
   };

   //TODO is a concrete object approach needed?

   /*m_detectorElements.emplace(layer,std::move(ECAL(layer,
    VolumeCylinder(fastsim::to_str(layer), 1.55, 2.1, 1.30, 2),
    Material(layer, 8.9e-3, 0.275),
    0.15, // eta_crack
    0.4, //emin
    std::vector<double> {.073, .1, .005})))*/


   //TODOAJR add in  tracker etc below




}

CMSHCAL::CMSHCAL(fastsim::enumLayer layer, const VolumeCylinder& volume,
           const  Material& material , const std::vector<double>& eres) :
   HCAL(layer,   volume,  material), m_eres(eres)
{}

CMSHCAL::CMSHCAL(fastsim::enumLayer layer, const VolumeCylinder&& volume,
           const Material&& material, const std::vector<double>&& eres) :
   HCAL(layer,   volume,  material), m_eres(eres)
{}


//will need to be rewritten for different detectors
/**
 Cluster_size as a function of the type of particle
 @param ptc particle
 @return size of resulting cluster
 */
double CMSHCAL::clusterSize(const Particle& ptc) const
{
   //TODO
   return 0.2;
}

/* Decides whether a cluster will be seen by a detector
 @param cluster the cluster to be analysed
 @return true is cluster is detected, false it if is too small to be seen
 */
bool CMSHCAL::acceptance(const Cluster& cluster) const
{
   double energy = cluster.getEnergy();
   double eta = fabs(cluster.getEta());

   //AJRTOCHECK should this be more parameterized et eta_crack etc
   if (eta < 3.)
      return energy > 4.;
   else if (eta < 5.)
      return energy > 7.;
   else
      return false;

}

double CMSHCAL::energyResolution(double energy) const
{
   return m_eres[0] / sqrt(energy);
}


CMSTracker::CMSTracker(fastsim::enumLayer layer, const VolumeCylinder& volume) :
Tracker(layer,   volume,  Material(layer,0,0))
{}

CMSTracker::CMSTracker(fastsim::enumLayer layer, const VolumeCylinder&& volume) :
Tracker(layer,   volume,  Material(layer,0,0)){}

bool CMSTracker::acceptance(const Track& track) const
{
   double pt = track.getPt();
   double eta = fabs(track.getEta());
   if (eta < 2.5 and pt>0.5)
      return true; //TODO random.uniform(0,1)<1. ; //# CMS without tracker material effects
   else
      return false;
   
}

double CMSTracker::getPtResolution(const Track& track) const
{
   double pt = track.getPt();
   //TODO
   return 5e-3;
}


CMSField::CMSField(fastsim::enumLayer layer, const VolumeCylinder& volume,double magnitude) :
Field(layer,   volume,  Material(layer,0,0)),m_magnitude(magnitude)
{}

CMSField::CMSField(fastsim::enumLayer layer, const VolumeCylinder&& volume,double magnitude) :
Field(layer,   volume,  Material(layer,0,0)),m_magnitude(magnitude)
{}


//def space_resolution(self, ptc):
//pass



/*


 Tracker::Tracker()
 :
 DetectorElement("tracker", "tracker", 1.29, 1.99,"void", 0, 0)
 {}


 DField::DField(double magnitude):
 DetectorElement("tracker","field",  2.9, 3.6,"void",0.,0.),
 m_magnitude(magnitude)
 {
 }
 */



/*  // m_elements["tracker"]= new Tracker();
 string name="ECAL";
 std::vector<double> cms_eres{.073,.1,.005};
 VolumeCylinder vol(name, 1.55, 2.1, 1.30, 2);
 Material mat(name,8.9e-3, 0.275);
 m_elements["ecal"]= new ECAL(name, vol,mat,0.15, 0.4,cms_eres);
 */

//m_elements["hcal"]= new HCAL();
//m_elements["field"]= new DField(3.8);

//}


/*bool Tracker::acceptance(const Track& track){
 {
 double pt = track->pt;
 double eta = abs(track->p3.Eta());
 if (eta < 2.5&  pt>0.5)
 return true; //AJRTODO  ame this =random.uniform(0,1)<1.; //# CMS without tracker material effects
 else
 return false;
 }

 double Tracker::pt_resolution(const Track& track){
 # TODO: depends on the field
 pt = track.pt;
 return 5e-3;
 }






 bool ECAL::acceptance(self, cluster):
 energy = cluster.energy
 eta = abs(cluster.position.Eta())
 if eta < self.eta_crack:
 return energy>self.emin
 elif eta < 3.:
 return energy>self.emin and cluster.pt>0.5
 else:
 return False

 def ECAL::space_resolution(self, ptc):
 pass
 */

