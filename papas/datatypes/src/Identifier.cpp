#include "papas/datatypes/Identifier.h"
#include "papas/utility/StringFormatter.h"
#include <assert.h>
#include <cmath>
#include <inttypes.h>
#include <iostream>
#include <bitset>

//
// Encode information into a unique identifier
//
// accessible intro to use of bitwise operations can be found here
// http://stackoverflow.com/questions/6556961/use-of-the-bitwise-operators-to-pack-multiple-values-in-one-int
//

namespace papas {
// max index value is 2** m_bitshift

IdType Identifier::makeId(unsigned int index, ItemType type, char subt, float val) {

  if (type == kNone) {
    throw "Id must have a valid type";
  }

  if (index >= pow(2, m_bitshift) - 1) throw "Identifier unique id is too big: too many identifiers";

  // Shift all the parts and join together
  // NB uint64_t is needed to make sure the shift is carried out over 64 bits, otherwise
  // if the m_bitshift is 32 or more the shift is undefined and can return 0

  IdType typeShift = (uint64_t)type << m_bitshift1;
  IdType valueShift = (uint64_t)Identifier::floatToBits(val) << m_bitshift;
  IdType subtypeShift = (uint64_t) static_cast<int>(tolower(subt)) << m_bitshift2;
  IdType uid = (uint64_t)subtypeShift | (uint64_t)valueShift | (uint64_t)typeShift | index;

  if (!checkValid(uid, type, subt, val, index)) throw "Error occured constructing identifier";
  return uid;
}

Identifier::ItemType Identifier::itemType(IdType id) {
  return static_cast<ItemType>((id >> m_bitshift1) & (uint64_t)(pow(2, 3) - 1));
}

char Identifier::subtype(IdType id) {
  return static_cast<char>((id >> m_bitshift2) & (uint64_t)(pow(2, m_bitshift1 - m_bitshift2) - 1));
}

float Identifier::value(IdType id) {
  // shift to extract the required bits
  int bitvalue = id >> m_bitshift & (uint64_t)(pow(2, m_bitshift2 - m_bitshift) - 1);
  // convert bits back to float
  return bitsToFloat(bitvalue);
}

unsigned int Identifier::index(IdType id) { return id & (uint64_t)(pow(2, m_bitshift) - 1); }
  
unsigned int Identifier::uniqueId(IdType id)  {
    //For some purposes we want a smaller uniqueid without the value information
    //here we consruct a 32 bit uniqueid out of the index and the type and subtype
    unsigned int bitshift = m_bitshift + m_bitshift1 - m_bitshift2;
    IdType typeShift = (uint32_t)Identifier::itemType(id) << bitshift;
    IdType subtypeShift = (uint32_t) static_cast<int>(tolower(Identifier::subtype(id))) << m_bitshift;
    //binary printout std::cout <<"Index" << std::bitset<32>(Identifier::index(id)) <<std::endl;
    uint32_t uniqueid = (uint32_t)subtypeShift | (uint32_t)typeShift | (uint32_t)Identifier::index(id);
    if (!checkUIDValid(id, uniqueid))
      throw "unique id part of Identifier not valid";
    return uniqueid;
}
  

char Identifier::typeLetter(IdType id) {
  // converts from the identifier type enumeration such as kEcalCluster into a single letter decriptor eg 'e'
  std::string typelist = ".ehtpb....";
  return typelist[(unsigned int)Identifier::itemType(id)];
  // TODO error handling
}

Identifier::ItemType Identifier::itemType(char s) {
  // converts from the a single letter decriptor eg 'e' into the type enumeration such as kEcalCluster
  std::string typelist = ".ehtpb";
  auto found = typelist.find(s);
  if (found == typelist.npos) throw "type not found";
  return (ItemType)found;
}

std::string Identifier::typeAndSubtype(IdType id) {
  // produce the two letter type and subtype string such as 'em'
  std::string typeSubType = std::string(1, typeLetter(id)) + std::string(1, subtype(id));
  return typeSubType;
}

std::string Identifier::pretty(IdType id) {
  // pretty version of the identifier
  return Identifier::typeAndSubtype(id) + std::to_string(Identifier::index(id));
}

papas::Layer Identifier::layer(IdType id) {
  if (Identifier::isEcal(id))
    return papas::Layer::kEcal;
  else if (Identifier::isHcal(id))
    return papas::Layer::kHcal;
  else if (Identifier::isTrack(id))
    return papas::Layer::kTracker;
  else
    return papas::Layer::kNone;
}

Identifier::ItemType Identifier::itemType(papas::Layer layer) {
  if (layer == papas::Layer::kEcal)
    return ItemType::kEcalCluster;
  else if (layer == papas::Layer::kHcal)
    return ItemType::kHcalCluster;
  else if (layer == papas::Layer::kTracker)
    return ItemType::kTrack;
  else
    return ItemType::kNone;
}

bool Identifier::checkValid(IdType uid, ItemType type, char subt, float val, unsigned int indx) {
  // verify that it all works, the id should match the items from which it was constructed
  if (index(uid) != indx) return false;
  if (val != 0) {
    if ((fabs(value(uid) - val) >= fabs(val) * 10e-6) | (itemType(uid) != type) | (subtype(uid) != subt)) return false;
  }
  return true;
}
  
  bool Identifier::checkUIDValid(IdType id, unsigned int uniqueid) {
    unsigned int bitshift = m_bitshift + m_bitshift1 - m_bitshift2;
    // verify that it all works, the uniqueid should match the items from which it was constructed
    ItemType it = static_cast<ItemType>((uniqueid >> bitshift) & (uint32_t)(pow(2, 3) - 1));
    char st = static_cast<char>((uniqueid >> m_bitshift) & (uint64_t)(pow(2,bitshift - m_bitshift) - 1));
    unsigned idx = ((uniqueid) & (uint32_t)(pow(2, m_bitshift) - 1));
    if (it != itemType(id) || st != subtype(id) || idx != index(id))
      return false;
    return true;
  }
  


uint64_t Identifier::floatToBits(float value) {
  // CHAR_BIT not known on lxplus assert(CHAR_BIT * sizeof(float) == 32);  // TODO think of somewhere better to put this
  union {
    float value;  // assumes sizeof(float) == sizeof(int)
    int bits;
  } data;
  data.value = value;
  return (uint64_t)(data.bits);
};

float Identifier::bitsToFloat(uint64_t bits) {
  union {
    float value;  // assumes sizeof(float) == sizeof(int)
    int bits;
  } data;
  data.bits = bits;
  return (data.value);
};

}  // end namespace papas