//
//  simpleidentifier.cpp
//
//  Created by Alice Robson on 05/01/16.
//
//Encode information into a unique identifier
//
// accessible intro to use of bitwise operations can be found here
//http://stackoverflow.com/questions/6556961/use-of-the-bitwise-operators-to-pack-multiple-values-in-one-int
//



#include "Identifier.h"

//#include "enummanager.h"
#include "enums.h"


int Identifier::s_counter = 0; /// static which will be used to create a unique long

void Identifier::setCounter(int startid) /// allows user to start counter at another point
{
   s_counter = startid;
}

//TODO rename as OBJECTTYpe not PFObjectType
long Identifier::makeIdentifier(fastsim::enumDataType type,     // 6 bits  (to be refined later )
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

fastsim::enumLayer Identifier::layer(long id)
{
   int layer = (id >> 6) & 0b1111 ;//(4 bits)
   return static_cast<fastsim::enumLayer>(layer);
}

fastsim::enumSubtype Identifier::subType(long id)
{
   int subtype = (id >> 10) & 0b111 ;//(3 bits)
   return static_cast<fastsim::enumSubtype>(subtype);
}

fastsim::enumSource Identifier::source(long id)
{
   int source = (id >> 13) & 0b111 ;//(3 bits)
   return static_cast<fastsim::enumSource>(source);
}

fastsim::enumDataType Identifier::dataType(long id)
{
   int type = id & 0b111111 ;//(preserve the 6 rightmost bits)
   return static_cast<fastsim::enumDataType>(type);
}

int Identifier::uniqueID(long id)
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

long Identifier::makeBlockID( )
{
  return        Identifier::makeIdentifier(fastsim::enumDataType::BLOCK,
                                           fastsim::enumLayer::NONE,
                                           fastsim::enumSubtype::NONE,
                                           fastsim::enumSource::RECONSTRUCTION);
}

long Identifier::makeParticleID(eSource source)
{
   return Identifier::makeIdentifier(fastsim::enumDataType::PARTICLE,
                                     fastsim::enumLayer::NONE,
                                     fastsim::enumSubtype::RAW,
                                     source);
}

bool Identifier::isCluster(long id)
{
  return (isEcal(id) || isHcal(id) );
}

bool Identifier::isEcal(long id)
{
  return (Identifier::layer(id)==fastsim::enumLayer::ECAL);
}

bool Identifier::isHcal(long id)
{
  return (Identifier::layer(id)==fastsim::enumLayer::ECAL);
}

bool Identifier::isTrack(long id)
{
  return (Identifier::dataType(id)==fastsim::enumDataType::TRACK);
}

bool Identifier::isBlock(long id)
{
  return (Identifier::dataType(id)==fastsim::enumDataType::BLOCK);
}

bool Identifier::isUniqueIDMatch(long id, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                               fastsim::enumSubtype subtype)
{
   return (Identifier::layer(id)==layer && Identifier::subType(id)==subtype && Identifier::dataType(id)==datatype);
}

bool Identifier::isUniqueIDMatch(long id, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                                 fastsim::enumSubtype subtype,fastsim::enumSource source)
{
   return (Identifier::layer(id)==layer && Identifier::subType(id)==subtype && Identifier::dataType(id)==datatype
           && Identifier::source(id)==source);
}

bool Identifier::isSmeared(long id)
{
   return (Identifier::subType(id)==fastsim::enumSubtype::SMEARED );
}

char Identifier::typeShortCode(long id)
{
  auto layer = Identifier::layer(id);
  auto dataType = Identifier::dataType(id);
  if (dataType==fastsim::enumDataType::CLUSTER &&  layer==fastsim::enumLayer::ECAL)
    return 'e';
  else if (dataType==fastsim::enumDataType::CLUSTER &&  layer==fastsim::enumLayer::HCAL)
    return 'h';
  else if (dataType==fastsim::enumDataType::TRACK)
    return 't';
  else if (dataType==fastsim::enumDataType::PARTICLE)
    return 'p';
  else
    return 'x';
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

   enumDataType type=Identifier::dataType(id);
   enumDataType type2=Identifier::dataType(id2);
   enumSource source =Identifier::source(id2);
   enumLayer layer =Identifier::layer(id2);
   enumSubtype subtype=Identifier::subtype(id2);
   return 0;
}*/

