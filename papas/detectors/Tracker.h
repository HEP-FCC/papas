//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef TRACKER_H
#define TRACKER_H

#include "papas/detectors/DetectorElement.h"
#include "papas/detectors/Material.h"
#include <list>

namespace papas {

class Track;

///< Base class for tracker detector element. User must define own tracker class
class Tracker : public DetectorElement {
public:
  using DetectorElement::DetectorElement;
  /**  @brief virtual function that user must define to calculate the limit of momentum that can be distinguished by
   * tracker
   * @param[in] track Track for which the limit of momentum is to be determined
   */
  virtual double ptResolution(const Track& track) const = 0;
  /**  @brief virtual function that user must define to calculate the tracker acceptance
   * @param[in] track Track for which the acceptance is needed
   */
  virtual bool acceptance(const Track& track) const = 0;

protected:
};

}  // end namespace papas
#endif
