//
//  Definitions.h
//  papas
//
//  Created by Alice Robson on 12/05/16.
//
//

#ifndef Definitions_h
#define Definitions_h

#include <inttypes.h>

namespace papas {
typedef uint64_t IdType;

/// describe position of a cluster within the Detector
enum Position { kVertex, kEcalIn, kEcalOut, kEcalDecay, kHcalIn, kHcalOut };
/// detector layers
enum Layer { kNone, kTracker, kEcal, kHcal, kField };
// enum SubType { kNONE, RAW, MERGED, SMEARED };  // TODO could this go in clusters class
// enum enumSource { NONE, SIMULATION, RECONSTRUCTION, OBSERVED };  // TODO get rid
}

#endif /* Definitions_h */