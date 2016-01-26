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

///ECAL Class (CMS implementation)
/** This ECAL is implmented specifically for CMS

      It is intended to be replaced by users to match the required detector characteristics

 */

//CMSECAL inherit from ECAL clustersize/acceptance/energy resolution
//ECAL inherits from DetectorElement
class CMSECAL: public ECAL {
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
   bool acceptance(const Cluster& cluster)  const override;
   double energyResolution(double energy)  const override;
   //TODOAJR space_resolution(self, ptc):
private:
   double m_eta_crack;
   double m_emin;
   std::vector<double> m_eres;  ///energy resolution
};


class CMSHCAL: public HCAL {
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
   virtual double getPtResolution(const Track&) const override;
   virtual bool   acceptance(const Track&) const override;
      //TODOAJR space_resolution(self, ptc):
private:
    
};
class CMSField: public Field {
public:
   //TODO consider best approach for below?
   ///constructor - allows the Material and Volume to be created on the fly
   CMSField(fastsim::enumLayer layer, const VolumeCylinder&& volume,
              double
              );
   ///constructor - requires the Material and Volume to be already in existance
   CMSField(fastsim::enumLayer layer, const VolumeCylinder& volume, double);
   //virtual double ptResolution(const Track&) const override;
   //virtual bool   acceptance(const Track&) const override;
   //TODOAJR space_resolution(self, ptc):
private:
    double m_magnitude;
};


class CMS: public BaseDetector {
public:
   CMS();
private:

};

// Below here is TODO
/*
class Tracker: public DetectorElement {
//TODO acceptance and resolution depend on the particle type
public:
   Tracker();
   bool acceptance(const Track& track) const override;
   double pt_resolution(const Track& track);
protected:

private:
   //VolumeCylinder m_temp_volume;
   //Material m_temp_mat ;
   std::vector<double> m_eres;
};



class DField: public DetectorElement {
public:
   DField(double magnitude);
private:
   double m_magnitude;
};



*/

#endif

/*AJRTODO def acceptance(self, cluster):
    energy = cluster.energy
    eta = abs(cluster.position.Eta())
    if eta < 3. :
        return energy>4.
    elif eta < 5.:
        return energy>7.
    else:
        return False

def space_resolution(self, ptc):
    pass



class Tracker(DetectorElement):
#TODO acceptance and resolution depend on the particle type

def __init__(self):
    volume = VolumeCylinder('tracker', 1.29, 1.99)
    mat = material.void
    super(Tracker, self).__init__('tracker', volume,  mat)

def acceptance(self, track):
    # return False
    pt = track.pt
    eta = abs(track.p3.Eta())
    if eta < 2.5 and pt>0.5:
        return random.uniform(0,1)<1. # CMS without tracker material effects
    else:
        return False

def pt_resolution(self, track):
    # TODO: depends on the field
    pt = track.pt
    return 5e-3




cms = CMS()
*/
