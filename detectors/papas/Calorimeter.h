//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef CALORIMETOR_H
#define CALORIMETOR_H

#include "geometry.h"
#include "Material.h"
#include <list>

namespace papas {

class Particle;
class Material;
class VolumeCylinder;
class Cluster;
class Track;


/** The Calorimeter class  is the basis of HCAL and ECAL elements.
    It contains virtual functions that must be implemented by users so as to be detector specific.
    Users must provide clusterSize/acceptance/energyResolution methods for the inheriting class
 */
class Calorimeter : public DetectorElement {
public:
  enum LOCATION { kBarrel = 0, kEndCap = 1 };
  using DetectorElement::DetectorElement;

  /** energy Resolution of ECAL
   @param[in] energy : TODO
   @param[in] eta : angle of arrival
   @return minimum energy resolution of the detector
   */
  virtual double energyResolution(double energy, double eta) const = 0;

  /** energy response of ECAL
   @param[in] energy : TODO
   @param[in] eta : angle of arrival
   @return TODO ask COLIN
   */
  virtual double energyResponse(double energy, double eta = 0) const = 0;

  /** Minimum size that will be seen by a detector
   @param[in]  const Particle& ptc : particle that is to be detected
   @return size of resulting cluster (TODO units)
   */
  virtual double clusterSize(const Particle& ptc) const = 0;

  /** Decides whether a cluster will be seen by a detector
   @param[in]  the cluster to be analysed
   @return true is cluster is detected, false it if is too small to be seen
   */
  virtual bool acceptance(const Cluster& ptc) const = 0;

private:
};

}  // end namespace papas
#endif
