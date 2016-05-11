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



#include "Id.h"

//#include "enummanager.h"
#include "enums.h"


int Id::s_counter = 0; /// static which will be used to create a unique long

void Id::setCounter(int startid) /// allows user to start counter at another point
{
   s_counter = startid;
}

//TODO rename as OBJECTTYpe not PFObjectType
long Id::makeId(fastsim::enumDataType type,     // 6 bits  (to be refined later )
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

long Id::makeAnotherId(long id)
{
   int last = (id & 0b1111111111111111);
   long newid = (s_counter << 16) | last
                ; //keep last 16 bits but update the unique id
   s_counter++;
   return newid;
}

fastsim::enumLayer Id::layer(long id)
{
   int layer = (id >> 6) & 0b1111 ;//(4 bits)
   return static_cast<fastsim::enumLayer>(layer);
}

fastsim::enumSubtype Id::subType(long id)
{
   int subtype = (id >> 10) & 0b111 ;//(3 bits)
   return static_cast<fastsim::enumSubtype>(subtype);
}

fastsim::enumSource Id::source(long id)
{
   int source = (id >> 13) & 0b111 ;//(3 bits)
   return static_cast<fastsim::enumSource>(source);
}

fastsim::enumDataType Id::dataType(long id)
{
   int type = id & 0b111111 ;//(preserve the 6 rightmost bits)
   return static_cast<fastsim::enumDataType>(type);
}

int Id::uniqueId(long id)
{
   return (id >> 16);
}

long Id::makeClusterId(fastsim::enumLayer layer, fastsim::enumSubtype subtype) {
  return Id::makeId(fastsim::enumDataType::CLUSTER, layer, subtype, fastsim::enumSource::SIMULATION);
}

long Id::makeTrackId(fastsim::enumSubtype subtype) {
  return Id::makeId(
      fastsim::enumDataType::TRACK, fastsim::enumLayer::TRACKER, subtype, fastsim::enumSource::SIMULATION);
}

long Id::makeBlockId() {
  return Id::makeId(fastsim::enumDataType::BLOCK,
                    fastsim::enumLayer::NONE,
                    fastsim::enumSubtype::NONE,
                    fastsim::enumSource::RECONSTRUCTION);
}

long Id::makeParticleId(eSource source)
{
   return Id::makeId(fastsim::enumDataType::PARTICLE,
                                     fastsim::enumLayer::NONE,
                                     fastsim::enumSubtype::RAW,
                                     source);
}

bool Id::isCluster(long id)
{
  return (isEcal(id) || isHcal(id) );
}

bool Id::isEcal(long id)
{
  return (Id::layer(id)==fastsim::enumLayer::ECAL);
}

bool Id::isHcal(long id)
{
  return (Id::layer(id)==fastsim::enumLayer::HCAL);
}

bool Id::isTrack(long id)
{
  return (Id::dataType(id)==fastsim::enumDataType::TRACK);
}

bool Id::isBlock(long id)
{
  return (Id::dataType(id)==fastsim::enumDataType::BLOCK);
}

bool Id::isUniqueIdMatch(long id, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                               fastsim::enumSubtype subtype)
{
   return (Id::layer(id)==layer && Id::subType(id)==subtype && Id::dataType(id)==datatype);
}

bool Id::isUniqueIdMatch(long id, fastsim::enumDataType datatype, fastsim::enumLayer layer,
                                 fastsim::enumSubtype subtype,fastsim::enumSource source)
{
   return (Id::layer(id)==layer && Id::subType(id)==subtype && Id::dataType(id)==datatype
           && Id::source(id)==source);
}

bool Id::isSmeared(long id)
{
   return (Id::subType(id)==fastsim::enumSubtype::SMEARED );
}

char Id::typeShortCode(long id)
{
  auto layer = Id::layer(id);
  auto dataType = Id::dataType(id);
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
   Id::setCounter(7777);
   long id=Id::makeIdentifier(enumDataType::TRACK,
                                      enumLayer::TRACKER,
                                      enumSubtype::SMEARED,
                                      enumSource::RECONSTRUCTION);


   long id2=Id::makeIdentifier(enumDataType::CLUSTER,
                                       enumLayer::HCAL,
                                       enumSubtype::MERGED,
                                       enumSource::SIMULATION);

   enumDataType type=Id::dataType(id);
   enumDataType type2=Id::dataType(id2);
   enumSource source =Id::source(id2);
   enumLayer layer =Id::layer(id2);
   enumSubtype subtype=Id::subtype(id2);
   return 0;
}*/

