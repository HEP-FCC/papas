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

class Identifier {
public:
   Identifier() {};
   typedef fastsim::enumLayer eLayer;
   typedef fastsim::enumSubtype eSubtype;
   typedef fastsim::enumSource eSource;

   static long makeIdentifier(fastsim::enumDataType
                              type, //chack name with Colin
                              eLayer layer,
                              fastsim::enumSubtype subtype,
                              fastsim::enumSource source,
                              int uniqueid = s_counter);
   static long makeAnotherIdentifier(long
                                     existingid);///uses same enums as existing id  but sets a new uniqueID.
   static long makeClusterID(eLayer layer, eSubtype subtype = eSubtype::RAW);
   
   static long makeECALClusterID(eSubtype subtype = eSubtype::RAW) { return Identifier::makeClusterID(eLayer::ECAL, subtype); };
   static long makeHCALClusterID(eSubtype subtype = eSubtype::RAW) { return Identifier::makeClusterID(eLayer::HCAL, subtype) ;};
   static long makeParticleID(eSource source);
   static long makeTrackID( fastsim::enumSubtype subtype= eSubtype::RAW);
   
  
   static bool isUniqueIDMatch(long id, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                               fastsim::enumSubtype subtype,fastsim::enumSource source);
   static bool isUniqueIDMatch(long id, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                        fastsim::enumSubtype subtype);
   static bool isSmeared(long id);

   static void setCounter(int startid);  ///intended for use once at start

   //Access stored Idendtifier information
   static fastsim::enumLayer        getLayer(long id);
   static fastsim::enumSubtype      getSubtype(long id);
   static fastsim::enumSource       getSource(long id);
   static fastsim::enumDataType getDataType(long id);
   static int getUniqueID(long id);
   //TODO add a checkValid function
private:
   static int s_counter;

};




#endif /* identifier_hpp */
