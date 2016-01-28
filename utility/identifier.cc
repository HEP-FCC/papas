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

#include "identifier.h"
#include "enums.h"


int Identifier::s_counter =
   0; /// static which will be used to create a unique long

void Identifier::setCounter(int
                            startid) /// allows user to start counter at another point
{
   s_counter = startid;
}

//TODO rename as OBJECTTYpe not PFObjectType
long Identifier::makeIdentifier(fastsim::enumDataType
                                type, // 6 bits  (to be refined later )
                                fastsim::enumLayer layer,       // 4 bits
                                fastsim::enumSubtype subtype,   // 3 bits
                                fastsim::enumSource source,     // 3 bits
                                int uniqueid)
{
   // consider reordering to source ... layer .. type ,.. subtype ..uniqueid
   s_counter++; //default is to start at 1 for first id so that id=0 means unset
   long id = (uniqueid << 16) | ((int)source << 13) | ((int)subtype << 10) | ((
                int)layer << 6) | (int)type;

   return id;
}

long Identifier::makeAnotherIdentifier(long id)
{
   int last = (id & 0b1111111111111111);
   long newid = (s_counter << 16) | last
                ; //keep last 16 bits but update the unique id
   s_counter++;
   return newid;
}

fastsim::enumLayer Identifier::getLayer(long id)
{
   int layer = (id >> 6) & 0b1111 ;//(4 bits)
   return static_cast<fastsim::enumLayer>(layer);
}

fastsim::enumSubtype Identifier::getSubtype(long id)
{
   int subtype = (id >> 10) & 0b111 ;//(3 bits)
   return static_cast<fastsim::enumSubtype>(subtype);
}

fastsim::enumSource Identifier::getSource(long id)
{
   int source = (id >> 13) & 0b111 ;//(3 bits)
   return static_cast<fastsim::enumSource>(source);
}

fastsim::enumDataType Identifier::getDataType(long id)
{
   int type = id & 0b111111 ;//(preserve the 6 rightmost bits)
   return static_cast<fastsim::enumDataType>(type);
}

int Identifier::getUniqueID(long id)
{
   return (id >> 16);
}

long Identifier::makeClusterID(fastsim::enumLayer layer,
                               fastsim::enumSubtype subtype)
{
   return        Identifier::makeIdentifier(fastsim::enumDataType::CLUSTER,
                 layer,
                 subtype,
                 fastsim::enumSource::SIMULATION);
}

long Identifier::makeTrackID( fastsim::enumSubtype subtype)
{
   return        Identifier::makeIdentifier(fastsim::enumDataType::TRACK,
                                            fastsim::enumLayer::TRACKER,
                                            subtype,
                                            fastsim::enumSource::SIMULATION);
}
long Identifier::makeParticleID(eSource source)
{
   return Identifier::makeIdentifier(fastsim::enumDataType::PARTICLE,
                                     fastsim::enumLayer::NONE,
                                     fastsim::enumSubtype::RAW,
                                     source);
}


bool Identifier::isUniqueIDMatch(long id, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                               fastsim::enumSubtype subtype)
{
   return (Identifier::getLayer(id)==layer && Identifier::getSubtype(id)==subtype && Identifier::getDataType(id)==datatype);
}

bool Identifier::isUniqueIDMatch(long id, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                                 fastsim::enumSubtype subtype,fastsim::enumSource source)
{
   return (Identifier::getLayer(id)==layer && Identifier::getSubtype(id)==subtype && Identifier::getDataType(id)==datatype
           && Identifier::getSource(id)==source);
}





/*

int test()
{
   using namespace fastsim;
   Identifier::setCounter(7777);
   long id=Identifier::makeIdentifier(enumDataType::TRACK,
                                      enumLayer::TRACKER,
                                      enumSubtype::SMEARED,
                                      enumSource::RECONSTRUCTION);


   long id2=Identifier::makeIdentifier(enumDataType::CLUSTER,
                                       enumLayer::HCAL,
                                       enumSubtype::MERGED,
                                       enumSource::SIMULATION);

   enumDataType type=Identifier::getDataType(id);
   enumDataType type2=Identifier::getDataType(id2);
   enumSource source =Identifier::getSource(id2);
   enumLayer layer =Identifier::getLayer(id2);
   enumSubtype subtype=Identifier::getSubtype(id2);
   return 0;
}*/

