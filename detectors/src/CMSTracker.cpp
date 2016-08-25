//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @file CMS.cc
 * @brief Implementation of the CMS detector
 */
#include "CMSTracker.h"
#include "Random.h"
#include "pTrack.h"

namespace papas {

CMSTracker::CMSTracker(const VolumeCylinder& volume) : Tracker(Layer::kTracker, volume, Material(0, 0)) {}

CMSTracker::CMSTracker(const VolumeCylinder&& volume) : Tracker(Layer::kTracker, volume, Material(0, 0)) {}

bool CMSTracker::acceptance(const Track& track) const {
  double pt = track.pt();
  double eta = fabs(track.eta());
  randomgen::RandUniform rUniform{0, 1};
  bool accept = false;
  if (eta < 1.35 && pt > 0.5) {
    accept = rUniform.next() < 0.95;
  } else if (eta < 2.5 && pt > 0.5) {
    accept = rUniform.next() < 0.9;
  }
  return accept;
}

double CMSTracker::ptResolution(const Track& track) const {
  double pt = track.pt();  // TODO inherited from Colin: depends on the field
  (void)pt;                // suppress unused parameter warning
  return 5e-3;
}

}  // end namespace papas