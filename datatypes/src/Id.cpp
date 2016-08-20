#include "Id.h"

#include "StringFormatter.h"
#include <cmath>
#include <inttypes.h>
#include <iostream>
// temp
//  Created by Alice Robson on 05/01/16.
//
// Encode information into a unique identifier
//
// accessible intro to use of bitwise operations can be found here
// http://stackoverflow.com/questions/6556961/use-of-the-bitwise-operators-to-pack-multiple-values-in-one-int
//

/** @class   Id Datatypes/Datatypes/Id.h Id.h
 *
 *  @brief Encode information into a unique identifier
 *  Example usage: IdType uid=Id::MakeId(kEcalCluster)
 *
 *  @author  Alice Robson
 *  @date    2016-04-05
 */

namespace papas {

unsigned int Id::s_counter = 1;  /// static which will be used to create a unique long

IdType Id::makeId(ItemType type, unsigned int uniqueid) {

  if (type > 6) {  // TODO proper error checking on type
    std::cout << "TOO big" << std::endl;
  }
  // NB uint64_t is needed to make sure the shift is carried out over 64 bits, otherwise
  // if the btshift is 32 or more the shift is undefined and can return 0
  Type id = (((uint64_t)type) << bitshift) | uniqueid;
  s_counter++;

  // std::cout << "makeID: "  << id << " = "<< type << " : uid = " << uniqueid << std::endl;;
  return id;
}

void Id::reset() { s_counter = 1; }

Id::ItemType Id::itemType(Type id) { return static_cast<ItemType>(id >> bitshift); }

unsigned int Id::uniqueId(Type id) { return id & (uint64_t)(pow(2, bitshift) - 1); }

papas::Layer Id::layer(Type id) {
  if (Id::isEcal(id))
    return papas::Layer::kEcal;
  else if (Id::isHcal(id))
    return papas::Layer::kHcal;
  else if (Id::isTrack(id))
    return papas::Layer::kTracker;
  else
    return papas::Layer::kNone;
}

Id::ItemType Id::itemType(papas::Layer layer) {
  if (layer == papas::Layer::kEcal)
    return ItemType::kEcalCluster;
  else if (layer == papas::Layer::kHcal)
    return ItemType::kHcalCluster;
  else if (layer == papas::Layer::kTracker)
    return ItemType::kTrack;
  else
    return ItemType::kNone;
}

char Id::typeShortCode(IdType id) {
  std::string typelist = ".ehtprb....";
  return typelist[(unsigned int)Id::itemType(id)];
  // TODO error handling
}

std::string Id::pretty(IdType id) { return string_format("%1c%-8d", Id::typeShortCode(id), Id::uniqueId(id)); }
}  // end namespace papas