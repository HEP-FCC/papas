//
//  simpleidentifier.h
//
//  Created by Alice Robson on 05/01/16.
//
//

#ifndef simpleidentifier_h
#define simpleidentifier_h
#include <stdint.h>


enum class enumDataType {PARTICLE, TRACK, CLUSTER};
enum class enumSubType {RAW, MERGED, SMEARED};

class Identifier {
public:
   Identifier() {};
   static long makeIdentifier(enumDataType datatype, enumSubType subtype);
   //Access stored Identifier information
   static enumDataType        getDataType(long id);
   static enumSubType         getSubType(long id);
   static int getUniqueID(long id);
private:
   static int s_counter;
};




#endif /* simpleidentifier_h */
