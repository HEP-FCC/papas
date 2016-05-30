//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef CMS_H
#define CMS_H

#include <vector>
#include <string>
#include "detector.h"
#include "Definitions.h"



namespace papas {

class Track;
class Particle;
class Cluster;
class Material;
class VolumeCylinder;
/**
 * @file CMS.h
 * @brief Implementation of CMS detector
 */


///CMSECAL Class (CMS implementation)
/** This ECAL is implmented specifically for CMS

      It is intended to be replaced by users to match the required detector characteristics

 */
//CMSECAL inherits from ECAL clustersize/acceptance/energy resolution
//ECAL inherits from DetectorElement
class CMSECAL : public Calorimeter {
public:
  // AJRTODO check whether both cosntructors are needed
  /// constructor - allows the Material and Volume to be created on the fly
  CMSECAL(papas::Layer layer, const VolumeCylinder&& volume, const Material&& material, double eta_crack,
          std::vector<double> emin, const std::vector<std::vector<double>> eres);

  /// constructor - requires the Material and Volume to be already in existance
  /*ECAL(papas::Layer layer, const VolumeCylinder& volume,
       const Material& material , double eta_crack, double emin,
       const std::vector<double>& eres);*/

  double clusterSize(const Particle& ptc) const override;
  bool acceptance(const Cluster& cluster) const override;
  double energyResolution(double energy, double eta = 0 ) const override;
  // TODOAJR space_resolution(self, ptc):
private:
  double m_etaCrack;
  // double m_emin;
  // bool m_isBarrel;
  std::vector<double> m_emin;
  std::vector<std::vector<double>> m_eres;  /// energy resolution
};

class CMSHCAL: public Calorimeter {
public:
   //TODO consider best approach for below?
   ///constructor - allows the Material and Volume to be created on the fly
   CMSHCAL(Layer layer,
           const VolumeCylinder&& volume,
           const Material&& material,
           double m_eta_crack,
           std::vector<std::vector<double>> eres,
           std::vector<std::vector<double>> eresp);
   ///constructor - requires the Material and Volume to be already in existance
   CMSHCAL(Layer layer,
           const VolumeCylinder& volume,
           const Material& material,
           double m_eta_crack,
           std::vector<std::vector<double>> eres,
           std::vector<std::vector<double>> eresp);

   double clusterSize(const Particle& ptc) const override;
   bool acceptance(const Cluster& cluster)  const override;
   double energyResolution(double energy, double eta=0)  const override;
   double energyResponse(double energy, double  eta=0) const override;
   //TODOAJR space_resolution(self, ptc):
private:
   double m_etaCrack;
   std::vector<std::vector<double>> m_eres; ///energy resolution
   std::vector<std::vector<double>> m_eresp; ///ask Colin what this is};

};
  
class CMSTracker: public Tracker {
public:
   //TODO consider best approach for below?
   ///constructor - allows the Material and Volume to be created on the fly
   CMSTracker(papas::Layer layer, const VolumeCylinder&& volume );
   ///constructor - requires the Material and Volume to be already in existance
   CMSTracker(papas::Layer layer, const VolumeCylinder& volume);
   virtual double ptResolution(const Track&) const override;
   virtual bool   acceptance(const Track&) const override;
      //TODOAJR space_resolution(self, ptc):
private:    
};

class CMSField: public Field {
public:
   //TODO consider best approach for below?
   ///constructor - allows the Material and Volume to be created on the fly
   CMSField(papas::Layer layer, const VolumeCylinder&& volume,
              double magnitude
              );
   ///constructor - requires the Material and Volume to be already in existance
   CMSField(papas::Layer layer, const VolumeCylinder& volume, double);
   
private:
   
};


class CMS: public Detector {
public:
   CMS();
private:

};
  
} // end namespace papas
#endif
