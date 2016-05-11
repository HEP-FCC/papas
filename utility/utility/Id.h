//
//  simpleidentifier.h for DAG example
//
//  Created by Alice Robson on 05/01/16.
//
//

#ifndef identifier_h
#define identifier_h
#include <stdint.h>
#include <iostream>
#include "enums.h"

class Id {
public:
  Id() {};
  typedef fastsim::enumLayer   eLayer;
  typedef fastsim::enumSubtype  eSubtype;
  typedef fastsim::enumSource  eSource;
  typedef fastsim::enumDataType eDataType;
  
  static long makeId(eDataType type, //check name with Colin
                             eLayer layer,
                             eSubtype subtype,
                             eSource source,
                             int uniqueid = Id::s_counter);
  static long makeAnotherId(long
                                    existingid);///uses same enums as existing id  but sets a new uniqueId.
  
  static long makeClusterId(eLayer layer, eSubtype subtype = eSubtype::RAW);
  static long makeECALClusterId(eSubtype subtype = eSubtype::RAW) { return Id::makeClusterId(eLayer::ECAL, subtype); };
  static long makeHCALClusterId(eSubtype subtype = eSubtype::RAW) { return Id::makeClusterId(eLayer::HCAL, subtype); };
  static long makeParticleId(eSource source);
  static long makeTrackId( fastsim::enumSubtype subtype= eSubtype::RAW);
  static long makeBlockId();
  
  
  static bool isCluster(long id);
  static bool isEcal(long id);
  static bool isHcal(long id);
  static bool isTrack(long id);
  static bool isBlock(long id);
  
  static bool isUniqueIdMatch(long id, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                              fastsim::enumSubtype subtype,fastsim::enumSource source);
  static bool isUniqueIdMatch(long id, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                              fastsim::enumSubtype subtype);
  static bool isSmeared(long id);
  
  static void setCounter(int startid);  ///intended for use once at start
  
  //Access stored Idendtifier information
  static fastsim::enumLayer        layer(long id);
  static fastsim::enumSubtype      subType(long id);
  static fastsim::enumSource       source(long id);
  static fastsim::enumDataType     dataType(long id);
  
  static char typeShortCode(long id); ///One letter code eg 'e' for ecal, 't' for track, 'x' for unknown
  static int uniqueId(long id);
  
  //TODO add a checkValid function
private:
  static int s_counter;
  
};




#endif /* identifier_hpp */
