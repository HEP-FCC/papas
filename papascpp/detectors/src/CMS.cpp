//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @file CMS.cc
 * @brief Implementation of the CMS detector
 */
#include "CMS.h"
#include "Cluster.h"
#include "BaseParticle.h"
#include "pTrack.h"
#include "geometry.h"
#include "material.h"
#include "Path.h"
#include "random.h"
#include <cmath>
#include <vector>

namespace papas {

CMS::CMS() : Detector() {
  // ECAL detector Element
  m_ecal = std::shared_ptr<const class Calorimeter>{
      new CMSECAL(VolumeCylinder(Layer::kEcal, 1.55, 2.1, 1.30, 2),
                  Material(8.9e-3, 0.275),
                  1.5,                         // eta_crack
                  std::vector<double>{0.3, 1},  // emin barrel and endcap
                  std::vector<std::vector<double>>{{.073, .1, .005}, {.213, .224, .005}})}; //barrel and endcap

  // HCAL detector element
  m_hcal = std::shared_ptr<const class Calorimeter>{
      new CMSHCAL(VolumeCylinder(Layer::kHcal, 2.9, 3.6, 1.9, 2.6),
                  Material(0.0, 0.17),
                  1.3, //eta crack
                  std::vector<std::vector<double>>{{1.25829, 0., 0.175950}, {1.32242e-06, 6.99123, 2.70281e-01}},
                  std::vector<std::vector<double>>{{1.03430, 5.23646, -2.03400}, {1.06742, 9.41242, -2.75191}})};

  // Tracker detector element
  m_tracker = std::shared_ptr<const Tracker>{new CMSTracker(VolumeCylinder(Layer::kTracker, 1.29, 1.99))};

  // Field detector element
  m_field = std::shared_ptr<const Field>{new CMSField(VolumeCylinder(Layer::kField, 2.9, 3.6), 3.8)};
}
  
  
/*
 CMSECAL::CMSECAL(papas::Layer layer, const VolumeCylinder& volume,
 const  Material& material , double eta_crack,
 double emin, const std::vector<double>& eres) :
 DetectorElement(layer,   volume,  material), m_eta_crack(eta_crack), m_emin(emin),
 m_eres(eres)
 {}*/

CMSECAL::CMSECAL(const VolumeCylinder&& volume, const Material&& material, double eta_crack,
                 std::vector<double> emin, const std::vector<std::vector<double>> eres)
  : Calorimeter(Layer::kEcal, volume, material), m_etaCrack(eta_crack), m_emin(emin), m_eres(eres) {}

// will need to be rewritten for different detectors
/**
 Cluster_size as a function of the type of particle
 @param ptc particle
 @return size of resulting cluster
 */
double CMSECAL::clusterSize(const Particle& ptc) const {

  int pdgid = abs(ptc.pdgId());
  if (pdgid == 22 | pdgid == 11)
    return 0.04;
  else
    return 0.07;
}


bool CMSECAL::acceptance(const Cluster& cluster) const {
  double energy = cluster.energy();
  double eta = fabs(cluster.eta());
  if (eta < m_etaCrack)
    return energy > m_emin[kBarrel];
  else if (eta < 2.93)
    return (energy > m_emin[kEndCap] & cluster.pt() > 0.2);
  else
    return false;
}

double CMSECAL::energyResolution(double energy, double eta) const {
  int location = kBarrel;
  if (fabs(eta)>1.479 && fabs(eta)<3.0 )
    location = kEndCap; //endcap
  double stoch = m_eres[location][0] / sqrt(energy);
  double noise = m_eres[location][1] / energy;
  double constant = m_eres[location][2];
  return sqrt(pow(stoch, 2) + pow(noise, 2) + pow(constant, 2));
}
  
double CMSECAL::energyResponse(double energy, double  eta) const  {
  (void)energy; //suppress warning messages for unused parameters;
  (void)eta;
  return 1;
}



CMSHCAL::CMSHCAL(const VolumeCylinder& volume, const Material& material, double etacrack,
                 std::vector<std::vector<double>> eres, std::vector<std::vector<double>> eresp)
  : Calorimeter(Layer::kHcal, volume, material), m_etaCrack(etacrack), m_eres(eres), m_eresp(eresp){}

CMSHCAL::CMSHCAL(const VolumeCylinder&& volume, const Material&& material, double etacrack,
                 std::vector<std::vector<double>> eres, std::vector<std::vector<double>> eresp)
    : Calorimeter(Layer::kHcal, volume, material),m_etaCrack(etacrack),  m_eres(eres), m_eresp(eresp) {}

// will need to be rewritten for different detectors
/**
 Cluster_size as a function of the type of particle
 @param ptc particle
 @return size of resulting cluster
 */
double CMSHCAL::clusterSize(const Particle& ptc) const {
  (void)ptc ;//suppress warning messages for unused parameters;
  return 0.2;
}

/* Decides whether a cluster will be seen by a detector
 @param cluster the cluster to be analysed
 @return true is cluster is detected, false it if is too small to be seen
 */
bool CMSHCAL::acceptance(const Cluster& cluster) const {
  double energy = cluster.energy();
  double eta = fabs(cluster.eta());

  bool accept=false;
  randomgen::RandUniform rUniform{0,1};
  if (eta < m_etaCrack) {
    if (energy>1.)
      accept= rUniform.next()<(1/(1 + exp((energy-1.93816)/(-1.75330))));
  }
  else if (eta < 3.) {
    if (energy>1.1) {
      if (energy<10.)
        accept= rUniform.next()<(1.05634-1.66943e-01*energy+1.05997e-02*(pow(energy,2)));
      else
        accept= rUniform.next()<(8.09522e-01/(1+exp((energy-9.90855)/-5.30366)));
    }
  }
  else if (eta < 5. && energy>7)
    accept=true;
  
  return accept;
}

double CMSHCAL::energyResolution(double energy, double eta) const {
  int part = kBarrel;
  if (fabs(eta)>m_etaCrack)
    part = kEndCap;
  double stoch = m_eres[part][0] / sqrt(energy);
  double  noise = m_eres[part][1] / energy;
  double  constant = m_eres[part][2];
  return sqrt( pow(stoch,2) + pow(noise, 2) + pow(constant, 2));
}
  
  
double CMSHCAL::energyResponse(double energy, double eta) const{
    int part = kBarrel;
    if (fabs(eta)>m_etaCrack)
      part = kEndCap;
    //using fermi-dirac function : [0]/(1 + exp( (energy-[1]) /[2] ))
    return m_eresp[part][0]/(1+exp((energy-m_eresp[part][1])/m_eresp[part][2]));
}
  
CMSTracker::CMSTracker(const VolumeCylinder& volume) :
  Tracker(Layer::kTracker, volume, Material(0, 0)) {}

CMSTracker::CMSTracker(const VolumeCylinder&& volume):
    Tracker(Layer::kTracker,volume, Material(0, 0)) {}

bool CMSTracker::acceptance(const Track& track) const {
  double pt = track.pt();
  double eta = fabs(track.eta());
  randomgen::RandUniform rUniform{0,1};
  bool accept = false;
  if (eta < 1.35 &&  pt>0.5 ) {
    accept = rUniform.next()<0.95;
  }
  else if (eta < 2.5 && pt>0.5) {
    accept= rUniform.next()<0.9;
  }
  return accept;
}
  
 
  


double CMSTracker::ptResolution(const Track& track) const {
  double pt = track.pt();  // TODO inherited from Colin: depends on the field
  (void)pt; //suppress unused parameter warning
  return 5e-3;
}

CMSField::CMSField(const VolumeCylinder& volume, double magnitude)
    : Field(volume, Material(0, 0), magnitude) {}

CMSField::CMSField(const VolumeCylinder&& volume, double magnitude)
    : Field(volume, Material(0, 0), magnitude) {}

} // end namespace papas
// def space_resolution(self, ptc):
// pass

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

// m_elements["hcal"]= new HCAL();
// m_elements["field"]= new DField(3.8);

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