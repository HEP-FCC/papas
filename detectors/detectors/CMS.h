//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef CMS_H
#define CMS_H

#include <vector>
#include <string>
#include "detector.h"
#include "enums.h"

class Track;

/**
 * @file CMS.h
 * @brief Implementation of CMS detector
 */
class Particle;
class Cluster;
class Material;
class VolumeCylinder;

///CMSECAL Class (CMS implementation)
/** This ECAL is implmented specifically for CMS

      It is intended to be replaced by users to match the required detector characteristics

 */
//CMSECAL inherits from ECAL clustersize/acceptance/energy resolution
//ECAL inherits from DetectorElement
class CMSECAL: public Calorimeter {
public:
   //AJRTODO check whether both cosntructors are needed
   ///constructor - allows the Material and Volume to be created on the fly
   CMSECAL(fastsim::enumLayer layer, const VolumeCylinder&& volume,
        const Material&& material, double eta_crack, double emin,
        const std::vector<double>&&
        eres);

   ///constructor - requires the Material and Volume to be already in existance
   /*ECAL(fastsim::enumLayer layer, const VolumeCylinder& volume,
        const Material& material , double eta_crack, double emin,
        const std::vector<double>& eres);*/

   double clusterSize(const Particle& ptc) const override;
   bool acceptance(const Cluster& cluster) const override;
   double energyResolution(double energy) const override;
   //TODOAJR space_resolution(self, ptc):
private:
   double m_eta_crack;
   double m_emin;
   std::vector<double> m_eres;  ///energy resolution
};


class CMSHCAL: public Calorimeter {
public:
   //TODO consider best approach for below?
   ///constructor - allows the Material and Volume to be created on the fly
   CMSHCAL(fastsim::enumLayer layer, const VolumeCylinder&& volume,
        const Material&& material,
        const std::vector<double>&&
        eres);
   ///constructor - requires the Material and Volume to be already in existance
   CMSHCAL(fastsim::enumLayer layer, const VolumeCylinder& volume,
        const Material& material ,
        const std::vector<double>&
        eres);

   double clusterSize(const Particle& ptc) const override;
   bool acceptance(const Cluster& cluster)  const override;
   double energyResolution(double energy)  const override;
   //TODOAJR space_resolution(self, ptc):
private:
   std::vector<double> m_eres; ///energy resolution
};

class CMSTracker: public Tracker {
public:
   //TODO consider best approach for below?
   ///constructor - allows the Material and Volume to be created on the fly
   CMSTracker(fastsim::enumLayer layer, const VolumeCylinder&& volume );
   ///constructor - requires the Material and Volume to be already in existance
   CMSTracker(fastsim::enumLayer layer, const VolumeCylinder& volume);
   virtual double ptResolution(const Track&) const override;
   virtual bool   acceptance(const Track&) const override;
      //TODOAJR space_resolution(self, ptc):
private:    
};

class CMSField: public Field {
public:
   //TODO consider best approach for below?
   ///constructor - allows the Material and Volume to be created on the fly
   CMSField(fastsim::enumLayer layer, const VolumeCylinder&& volume,
              double magnitude
              );
   ///constructor - requires the Material and Volume to be already in existance
   CMSField(fastsim::enumLayer layer, const VolumeCylinder& volume, double);
   
private:
   
};


class CMS: public Detector {
public:
   CMS();
private:

};
#endif
