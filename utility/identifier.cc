//
//  identifier.cpp
//  fastsim_cmake
//
//  Created by Alice Robson on 05/01/16.
//
//

#include "utility/identifier.h"

int Identifier::s_counter=0;

void Identifier::setCounter(int startid)
{
   s_counter=startid;
}
//rename as OBJECTTYpe not PFObjectType
long Identifier::makeIdentifier(identifier::enumPFObjectType type, // 6 bits  (to be refined later )
                                identifier::enumLayer layer,       // 4 bits
                                identifier::enumSubtype subtype,   // 3 bits
                                identifier::enumSource source,     // 3 bits
                                int uniqueid )
{
   // consider reordering to source ... layer .. type ,.. subtype ..uniqueid
   long id= (uniqueid << 16)|  ((int)source<<13 )|  ((int)subtype << 10) |  ((int)layer << 6) | (int)type;
   s_counter++;
   return id;
}

long Identifier::makeAnotherIdentifier(long id)
{
   int last = (id & 0b1111111111111111);
   long newid = ( s_counter<< 16) | last ; //keep last 16 bits but update the unique id
   s_counter++;
   return newid;
}

identifier::enumLayer Identifier::getLayer(long id){
   int layer = (id >> 6) & 0b1111 ;//(4 bits)
   return static_cast<identifier::enumLayer>( layer);
}

identifier::enumSubtype Identifier::getSubtype(long id){
   int subtype = (id >> 10) & 0b111 ;//(3 bits)
   return static_cast<identifier::enumSubtype>( subtype);
}

identifier::enumSource Identifier::getSource(long id){
   int source = (id >> 13) & 0b111 ;//(3 bits)
   return static_cast<identifier::enumSource>( source);
}

identifier::enumPFObjectType Identifier::getPFObjectType(long id)
{
   int type = id & 0b111111 ;//(preserve the 6 rightmost bits)
   return static_cast<identifier::enumPFObjectType>( type);
}

int Identifier::getUniqueID(long id)
{
   return (id >> 16);
}

/*

int test()
{
   using namespace identifier;
   Identifier::setCounter(7777);
   long id=Identifier::makeIdentifier(enumPFObjectType::TRACK,
                                      enumLayer::TRACKER,
                                      enumSubtype::SMEARED,
                                      enumSource::RECONSTRUCTION);
   
   
   long id2=Identifier::makeIdentifier(enumPFObjectType::CLUSTER,
                                       enumLayer::HCAL,
                                       enumSubtype::MERGED,
                                       enumSource::SIMULATION);
   
   enumPFObjectType type=Identifier::getPFObjectType(id);
   enumPFObjectType type2=Identifier::getPFObjectType(id2);
   enumSource source =Identifier::getSource(id2);
   enumLayer layer =Identifier::getLayer(id2);
   enumSubtype subtype=Identifier::getSubtype(id2);
   return 0;
}*/

