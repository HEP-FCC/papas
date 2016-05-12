//
//  Definitions.h
//  papas
//
//  Created by Alice Robson on 12/05/16.
//
//

#ifndef Definitions_h
#define Definitions_h
#include "enummanager.h"



namespace papas {
  enum Position {kVertex,  kEcalIn, kEcalOut, kEcalDecay, kHcalIn, kHcalOut};
  enum Layer { kNone, kTracker, kEcal, kHcal, kField};

  //ENUM_WITH_STRINGS(Layer, kVertex, kTracker, kEcal, kEcalIn, kEcalOut, kEcalDecay, kHcal, kHcalIn, kHcalOut, kField);

}

//TODO
namespace papas {
  
  ENUM_WITH_STRINGS(enumSubtype , NONE, RAW, MERGED, SMEARED)
  ENUM_WITH_STRINGS(enumSource , NONE, SIMULATION, RECONSTRUCTION, OBSERVED)
}

#endif /* Definitions_h */
