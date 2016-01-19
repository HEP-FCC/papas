//
//  simpleidentifier.cpp
//
//  Created by Alice Robson on 05/01/16.
//
//Encode information into a unqiue identifier
//
// accessible intro to us eof bitwise operations can be found here
//http://stackoverflow.com/questions/6556961/use-of-the-bitwise-operators-to-pack-multiple-values-in-one-int
//

#include "simpleidentifier.h"



int Identifier::s_counter =
   0; /// static which will be used to create a unique long


long Identifier::makeIdentifier(enumDataType datatype,   // 2 bits
                                enumSubType subtype)    // 3 bits
{
   s_counter++; //default is to start at 1 for first id so that id=0 means unset
   long id = (s_counter << 5) | ((int)subtype << 2) | (int)datatype;
   return id;
}

enumSubType Identifier::getSubType(long id)
{
   int subtype = (id >> 2) & 0b111 ;//(3 bits)
   return static_cast<enumSubType>(subtype);
}

enumDataType Identifier::getDataType(long id)
{
   int datatype = (id) & 0b11 ;//(2 bits)
   return static_cast<enumDataType>(datatype);
}

int Identifier::getUniqueID(long id)
{
   return (id >> 5);
}





