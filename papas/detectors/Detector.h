//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef DETECTOR_H
#define DETECTOR_H

#include <memory>

#include "papas/detectors/Calorimeter.h"
#include "papas/detectors/DetectorElement.h"
#include "papas/detectors/Field.h"
#include "papas/detectors/SurfaceCylinder.h"
#include "papas/detectors/Tracker.h"
#include "papas/datatypes/Track.h"
#include "papas/datatypes/SimParticle.h"

namespace papas {

/// Base Detector Class which needs to be inherited from for new detectors
///
/**
   Class from which user can provide their own detector code
   It must contain an HCAL, ECAL, Tracker and Field element
 */

class Detector {
public:
  Detector();

  /** Returns shared_ptr to detector elements
   * @param[in] layer : enum kEcal, kHcal, kTrack, kField
   */
  std::shared_ptr<const DetectorElement> element(papas::Layer layer) const;
  /// return the ecal or hcal
  /** Returns ecal or hcal
   * @param[in] layer : enum kEcal, kHcal
   */
  std::shared_ptr<const Calorimeter> calorimeter(papas::Layer layer) const;
  std::shared_ptr<const Calorimeter> ecal() const { return m_ecal; }    ///<access the ecal
  std::shared_ptr<const Calorimeter> hcal() const { return m_hcal; }    ///<access the hcal
  std::shared_ptr<const Tracker> tracker() const { return m_tracker; }  ///<access the tracker
  std::shared_ptr<const Field> field() const { return m_field; };       ///<access the field
  double electronAcceptance(const Track& track) const {
    return track.p3().Mag() > 5 && fabs(track.p3().Eta()) < 2.5;}
  
  double electronEnergyResolution(const SimParticle& ptc) const {
    return 0.1 / sqrt(ptc.e());}
  
  double muonAcceptance(const Track& track) const {
  return track.pt() > 5. && fabs(track.p3().Eta()) < 2.5;}
  
  double muonPtResolution(const SimParticle& ptc) const {
    return 0.02;}
  

protected:
  // shared pointers allow user to have their own derived ECAL and HCAL calorimeter class
  // that has a fixed interface defined by Calorimeter
  std::shared_ptr<const class Calorimeter> m_ecal;
  std::shared_ptr<const class Calorimeter> m_hcal;
  std::shared_ptr<const Tracker> m_tracker;
  std::shared_ptr<const Field> m_field;

private:
  std::list<SurfaceCylinder> m_cylinders;
};
}  // end namespace papas
#endif
