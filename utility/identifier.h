//
//  identifier.hpp
//  fastsim_cmake
//
//  Created by Alice Robson on 05/01/16.
//
//

#ifndef identifier_h
#define identifier_h
#include <stdint.h>
#include <iostream>
#include "enummanager.h"

//QUERIES on long to be 64 bit
//ENUM macro OK

//using longid= long long etc
//not rugent decide if this shoul dbe long or something else

namespace identifier { //THese enums should perhaps just go into the Identifier class
   //use a Macro that allows Enum objects to be easily accessed as strings.
   ENUM_WITH_STRINGS(enumPFObjectType ,PARTICLE,CLUSTER,TRACK)
   ENUM_WITH_STRINGS(enumLayer ,HCAL,ECAL,TRACKER)
   ENUM_WITH_STRINGS(enumSubtype ,RAW,MERGED,SMEARED)
   ENUM_WITH_STRINGS(enumSource , SIMULATION,RECONSTRUCTION,OBSERVED)
   
   
   //enum class enumPFObjectType {PARTICLE=0, CLUSTER, TRACK};
   //enum class enumLayer {HCAL=0, ECAL, TRACKER};
   //enum class enumSubtype {RAW=0, MERGED, SMEARED};
   //enum class enumSource {SIMULATION=0, RECONSTRUCTION, OBSERVED};
};

class Identifier
{
public:
   Identifier() {};
   static long makeIdentifier(identifier::enumPFObjectType type,
                              identifier::enumLayer layer,
                              identifier::enumSubtype subtype,
                              identifier::enumSource source,
                              int uniqueid = Identifier::s_counter );
   static long makeAnotherIdentifier(long id);//uses same enums but sets a new uniqueID.
   static void setCounter(int startid);  ///intended for use once at start
   
   //Access stored Idendtifier information
   static identifier::enumLayer        getLayer(long id);
   static identifier::enumSubtype      getSubtype(long id);
   static identifier::enumSource       getSource(long id);
   static identifier::enumPFObjectType getPFObjectType(long id);
   static int getUniqueID(long id);
   //TODO add a checkValid function
private:
   static int s_counter;
   
};




#endif /* identifier_hpp */
