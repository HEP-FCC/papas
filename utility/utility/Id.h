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
#include "Definitions.h"


class Id {
public:
  Id() {};
  typedef long Type;
 
  typedef papas::enumSubtype  eSubtype;
  typedef papas::enumSource  eSource;
  enum DataType{kParticle, kCluster, kTrack, kBlock};

  static long makeId(DataType type, //check name with Colin
                             papas::Layer layer,
                             eSubtype subtype,
                             eSource source,
                             int uniqueid = Id::s_counter);
  static long makeAnotherId(long
                                    existingid);///uses same enums as existing id  but sets a new uniqueId.
  
  static long makeClusterId(papas::Layer layer, eSubtype subtype = eSubtype::RAW);
  static long makeECALClusterId(eSubtype subtype = eSubtype::RAW) { return Id::makeClusterId(papas::Layer::kEcal, subtype); };
  static long makeHCALClusterId(eSubtype subtype = eSubtype::RAW) { return Id::makeClusterId(papas::Layer::kHcal, subtype); };
  static long makeParticleId(eSource source);
  static long makeTrackId( papas::enumSubtype subtype= eSubtype::RAW);
  static long makeBlockId();
  
  
  static bool isCluster(long id);
  static bool isEcal(long id);
  static bool isHcal(long id);
  static bool isTrack(long id);
  static bool isBlock(long id);
  
  static bool isUniqueIdMatch(long id, DataType datatype, papas::Layer layer,
                              papas::enumSubtype subtype,papas::enumSource source);
  static bool isUniqueIdMatch(long id, DataType datatype, papas::Layer layer,
                              papas::enumSubtype subtype);
  static bool isSmeared(long id);
  
  static void setCounter(int startid);  ///intended for use once at start
  
  //Access stored Idendtifier information
  static papas::Layer        layer(long id);
  static papas::enumSubtype      subType(long id);
  static papas::enumSource       source(long id);
  static DataType dataType(long id);
  
  static char typeShortCode(long id); ///One letter code eg 'e' for ecal, 't' for track, 'x' for unknown
  static int uniqueId(long id);
  
  //TODO add a checkValid function
private:
  static int s_counter;
  
};




#endif /* identifier_hpp */
