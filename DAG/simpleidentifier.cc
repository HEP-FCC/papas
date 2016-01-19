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

#include "utility/identifier.h"


int Identifier::s_counter=0; /// static which will be used to create a unique long

void Identifier::setCounter(int startid) /// allows user to start counter at another point
{
   s_counter=startid;
}

//TODO rename as OBJECTTYpe not PFObjectType
long Identifier::makeIdentifier(enumDataType datatype,   // 2 bits
                                enumSubType subtype),    // 3 bits
{
   s_counter++; //default is to start at 1 for first id so that id=0 means unset
   long id= (uniqueid << 5)|    ((int)subtype << 2) |  (int)datatype;
   return id;
}

fastsim::enumSubtype Identifier::getSubtype(long id) {
   int subtype = (id >> 2) & 0b111 ;//(3 bits)
   return static_cast<fastsim::enumSubtype>( subtype);
}

fastsim::enumSource Identifier::getDataType(long id) {
   int datatype= (id ) & 0b11 ;//(2 bits)
   return static_cast<fastsim::enumSource>( datatype);
}

int Identifier::getUniqueID(long id) {
   return (id >> 16);
}




