//
//  Created by Alice Robson on 09/11/15.
//
//
/**
 * @file CMS.cc
 * @brief Implementation of the CMS detector
 */
#include "papas/detectors/CMSTracker.h"
#include "papas/datatypes/Track.h"
#include "papas/utility/TRandom.h"

namespace papas {

CMSTracker::CMSTracker(const VolumeCylinder& volume) : Tracker(Layer::kTracker, volume, Material("void", 0, 0)) {}

CMSTracker::CMSTracker(const VolumeCylinder&& volume) : Tracker(Layer::kTracker, volume, Material("void", 0, 0)) {}

bool CMSTracker::acceptance(const Track& track) const {
  double pt = track.p3().Perp();
  double eta = fabs(track.p3().Eta());
  // randomgen::RandUniform rUniform{0, 1};
  bool accept = false;
  if (eta < 1.35 && pt > 0.5) {
    accept = rootrandom::Random::uniform(0, 1) < 0.95;
  } else if (eta < 2.5 && pt > 0.5) {
    accept = rootrandom::Random::uniform(0, 1) < 0.9;
  }
  return accept;
}

double CMSTracker::ptResolution(const Track& track) const {
  double pt = track.p3().Perp();  // TODO inherited from Colin: depends on the field
  (void)pt;                // suppress unused parameter warning
  return 1.1e-2;           // updated on 9/16 from 5e-3;
}

}  // end namespace papas
