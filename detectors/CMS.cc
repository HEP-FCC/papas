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
#include "pfobjects/pfobjects.h"


ECAL::ECAL(const std::string& name, const VolumeCylinder& volume,
           const  Material& material , double eta_crack,
           double emin, const std::vector<double>& eres) :
BaseECAL(name,   volume,  material), m_eta_crack(eta_crack), m_emin(emin),
m_eres(eres)
{}

ECAL::ECAL(const std::string& name, const VolumeCylinder&& volume,
           const Material&& material, double eta_crack,
           double emin, const std::vector<double>&& eres) :
BaseECAL(name,   volume,  material), m_eta_crack(eta_crack), m_emin(emin),
m_eres(eres)
{}


//will need to be rewritten for different detectors
/**
 Cluster_size as a function of the type of particle
 @param ptc particle
 @return size of resulting cluster
 */
double ECAL::cluster_size(const Particle& ptc) const
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
bool ECAL::acceptance(const Cluster& cluster) const
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


double ECAL::energy_resolution(double energy) const
{
   double stoch = m_eres[0] / sqrt(energy);
   double noise = m_eres[1] / energy;
   double constant = m_eres[2];
   return sqrt(pow(stoch, 2) + pow(noise, 2) + pow(constant, 2));
}


CMS::CMS() : BaseDetector()
{
   std::string name = "ECAL";
   m_detectorElements["ECAL"] = std::shared_ptr<const DetectorElement> {new ECAL(name,
                                                                                 VolumeCylinder(name, 1.55, 2.1, 1.30, 2),
                                                                                 Material(name, 8.9e-3, 0.275),
                                                                                 0.15, // eta_crack
                                                                                 0.4, //emin
                                                                                 std::vector<double> {.073, .1, .005})
   }; //eres
   
   //TODOAJR add in  tracker etc below
   
   
   ///NONe e
   name = "HCAL";
   m_detectorElements["HCAL"] = std::shared_ptr<const DetectorElement> {new HCAL(name,
                                                                                 VolumeCylinder(name, 2.9, 3.6, 1.9, 2.6),
                                                                                 Material(name, 0.0, 0.175),
                                                                                 std::vector<double> {1.1, 0., 0.})
   };
}

HCAL::HCAL(const std::string& name, const VolumeCylinder& volume,
           const  Material& material , const std::vector<double>& eres) :
BaseHCAL(name,   volume,  material),m_eres(eres)
{}

HCAL::HCAL(const std::string& name, const VolumeCylinder&& volume,
           const Material&& material, const std::vector<double>&& eres) :
BaseHCAL(name,   volume,  material),m_eres(eres)
{}


//will need to be rewritten for different detectors
/**
 Cluster_size as a function of the type of particle
 @param ptc particle
 @return size of resulting cluster
 */
double HCAL::cluster_size(const Particle& ptc) const
{
   return 0.2;
}

/* Decides whether a cluster will be seen by a detector
 @param cluster the cluster to be analysed
 @return true is cluster is detected, false it if is too small to be seen
 */
bool HCAL::acceptance(const Cluster& cluster) const
{
   double energy = cluster.getEnergy();
   double eta = fabs(cluster.getEta());
   //AJRTOCHECK should this be more parameterized et eta_crack etc
   
   if (eta < 3. )
      return energy>4.;
   else if (eta < 5.)
      return energy>7.;
   else
      return false;
   
}


double HCAL::energy_resolution(double energy) const
{
   return m_eres[0]/ sqrt( energy );
}


//def space_resolution(self, ptc):
//pass



/*
 
 HCAL::HCAL():
 DetectorElement("HCAL", "HCAL",2.9, 3.6, 1.9, 2.6,"HCAL", 0., 0.17), //AJRTODO was (.... .... , None, 0.17))
 m_eres (3,0.)
 {
 
 m_eres[0]=1.1;
 m_eres[1]=0.;
 m_eres[2]= 0.;
 }
 
 double HCAL::energy_resolution( double energy){
 return m_eres[0]/ sqrt( energy ) ;
 }
 
 double cluster_size(const  Particle& ptc){
 return 0.2;}
 
 
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

