//
//  Id.hpp
//  papas
//
//  Created by Alice Robson on 19/05/16.
//
//

#ifndef Id_hpp
#define Id_hpp

#include "papas/datatypes/Definitions.h"
#include <inttypes.h>
#include <iostream>

namespace papas {

/** Used to uniquely identify all clusters, tracks, blocks etc in PAPAS and also used in Nodes which
 * store the history (linkages) between items
 *
 * The Id Identifier is a 64bit unsigned integer
 * which is encoded using bit shifts:-
 * - a unique id (counter 1, 2, 3 etc) and
 * - the IdType:  ecalCluster, hcalCluster, track, particle
 *
 * Example usage: IdType uid=Id::MakeId(kEcalCluster)
 *
 *  @author  Alice Robson
 *  @date    2016-04-05
 */
class Id {
public:
  Id(){};
  typedef IdType Type;
  /// @enum the type of the item eg Particle, Cluster etc
  enum ItemType { kNone = 0, kEcalCluster = 1, kHcalCluster, kTrack, kParticle, kRecParticle, kBlock };

  static IdType makeId(ItemType type, unsigned int uniqueid = Id::s_counter);  ///< creates a new ID of given type
  static IdType makeEcalId(unsigned int uniqueid = Id::s_counter) { return makeId(kEcalCluster, uniqueid); }
  static IdType makeHcalId(unsigned int uniqueid = Id::s_counter) { return makeId(kHcalCluster, uniqueid); }
  static IdType makeTrackId(unsigned int uniqueid = Id::s_counter) { return makeId(kTrack, uniqueid); }
  static IdType makeParticleId(unsigned int uniqueid = Id::s_counter) { return makeId(kParticle, uniqueid); }
  static IdType makeRecParticleId(unsigned int uniqueid = Id::s_counter) { return makeId(kRecParticle, uniqueid); }
  static IdType makeBlockId(unsigned int uniqueid = Id::s_counter) { return makeId(kBlock, uniqueid); }

  static bool isEcal(Type id) { return (Id::itemType(id) == kEcalCluster); }
  static bool isHcal(Type id) { return (Id::itemType(id) == kHcalCluster); }
  static bool isCluster(Type id) { return (Id::isEcal(id) || Id::isHcal(id)); }
  static bool isTrack(Type id) { return (Id::itemType(id) == kTrack); }
  static bool isParticle(Type id) { return (Id::itemType(id) == kParticle); }
  static bool isRecParticle(Type id) { return (Id::itemType(id) == kRecParticle); }
  static bool isBlock(Type id) { return (Id::itemType(id) == kBlock); }

  static ItemType itemType(papas::Layer layer);  ///< Uses detector layer to work out itemType
  static papas::Layer layer(Type id);            ///< Says which detector layer the item belongs to, may be kNone
  static ItemType itemType(Type id);             ///< Returns encoded ItemType eg kParticle etc;
  ///< Returns encoded unique id, normally starts at one and incremented by one for each new id
  static unsigned int uniqueId(Type id);
  static char typeLetter(Type id);    ///< One letter short code eg 'e' for ecal, 't' for track, 'x' for unknown
  static std::string pretty(IdType id);  ///< Pretty string Id name eg e101 for an ecal wiht uniqueid 101;
  static void reset();  ///< Resets Id counter back to 0. NB care needed that Ids produced prior to this are thrown away
  static const unsigned int bitshift = 32;  ///< encoding parameter (used also by EdgeKey)
private:
  static unsigned int counter() { return s_counter; };  ///< provides the unique id part of the Id
  // TODO add a checkValid function
private:
  static unsigned int s_counter;  ///<starts at 1 and increments by 1 each time
};
}  // end nam
#endif /* Id_hpp */
