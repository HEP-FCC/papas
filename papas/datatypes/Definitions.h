#ifndef Definitions_h
#define Definitions_h

#include <inttypes.h>

namespace papas {
typedef uint64_t Identifier;

/// describe position of a Cluster within the Detector
enum Position { kVertex, kEcalIn, kEcalOut, kEcalDecay, kHcalIn, kHcalOut };
/// Detector layers
enum Layer { kNone, kTracker, kEcal, kHcal, kField };
}

#endif /* Definitions_h */
