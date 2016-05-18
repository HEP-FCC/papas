//
//  Definitions.h
//  papas
//
//  Created by Alice Robson on 12/05/16.
//
//

#ifndef Definitions_h
#define Definitions_h


namespace papas {
  enum Position {kVertex,  kEcalIn, kEcalOut, kEcalDecay, kHcalIn, kHcalOut};
  enum Layer { kNone, kTracker, kEcal, kHcal, kField};
  enum SubType { kNONE, RAW, MERGED, SMEARED};
  enum enumSource { NONE, SIMULATION, RECONSTRUCTION, OBSERVED};

}


#endif /* Definitions_h */
