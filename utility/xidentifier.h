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

//using longid= uint64_t;
//using intid= uint32_t;
//use 32bit for now.... best way to guarantee size?

namespace identifier {
   enum class enumPFObjectType {PARTICLE=0, CLUSTER, TRACK};
   enum class enumLayer {HCAL=0, ECAL, TRACKER};
   enum class enumSubtype {RAW=0, MERGED, SMEARED};
   enum class enumSource {SIMULATION=0, RECONSTRUCTION, OBSERVED};
};

template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
   return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

class Identifier
{
public:
   Identifier() {};
   
   static long makeIdentifier(identifier::enumPFObjectType type,
                              identifier::enumLayer layer,
                              identifier::enumSubtype subtype,
                              identifier::enumSource source,
                              int uniqueid = Identifier::s_counter //default value
   );
   static void setCounter(int startid);
   static identifier::enumLayer getLayer(long id); //Query naming of these function$'
   static identifier::enumSubtype getSubtype(long id);
   static identifier::enumSource getSource(long id);
   static identifier::enumPFObjectType getPFObjectType(long id);
   static int getUniqueID(long id);
   
private:
   static int s_counter;
   //
};

int Identifier::s_counter=0;

void Identifier::setCounter(int startid)
{
   s_counter=startid;
}

long Identifier::makeIdentifier(identifier::enumPFObjectType type, // 6 bits  (to be refined later )
                                identifier::enumLayer layer,       // 4 bits
                                identifier::enumSubtype subtype,   // 3 bits
                                identifier::enumSource source,     // 3 bits
                                int uniqueid )
{
   
   long id = (uniqueid << 16)|  ((int)source<<13 )|  ((int)subtype << 10) |  ((int)layer << 6) | (int)type;
   s_counter++;
   return id;
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
}


#endif /* identifier_hpp */
