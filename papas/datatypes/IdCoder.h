#ifndef IdCoder_h
#define IdCoder_h

#include "papas/datatypes/Definitions.h"
#include <inttypes.h>
#include <iostream>

namespace papas {

/**
  @brief An IdCoder is a uniqueid that contains encoded information about an element

 IdCoders are used to uniquely identify all clusters, tracks, blocks etc in PAPAS
 They are also used in Nodes which store the history (linkages) between items.

 Given an identifier, we can determine the type of an object, for example an ecal_cluster.
 and then use this to retrieve the cluster from a cluster collection.

 The IdCoder class consists of a set of static methods that can be used
 to create(encode) and to dissect(decode) identifiers.

 The identifier is 64 bits wide and stores info as follows
 from left: bits 64 to 61 = PFOBJECTTYPE enumeration eg ECAL, HCAL, PARTICLE (max value = 7)
 bits 60 to 53 = subtype - a single char eg 'g'
 bits 52 to 20 = encoded float value eg energy
 bits 21 to 1 = unique id (max value = 2097152 -1)

 Note that sorting on id will result in sorting by:
 type
 subtype
 value (small to large)
 uniqueid
 reverses sorting will result in items sorted into groups of type and subtype with largest values first in
 each type and subtype group.

 Usage example:
 @code
 IdType uid=IdCoder::MakeId(kEcalCluster, 's', 4.35);
 @endcode
 *
 *  @author  Alice Robson
 *  @date    2016-04-06
 */
class IdCoder {
public:
  IdCoder(){};

  /// @enum the type of the item eg Particle, Cluster etc
  enum ItemType { kNone = 0, kEcalCluster = 1, kHcalCluster, kTrack, kParticle, kBlock };
  typedef char SubType;
  /** Makes new identifier.
   @param[in]  type is an enum IdCoder::ItemType to say whether this id is for a cluster, particle etc
   @param[in]  subtype is a single letter subtype code  eg 'm' for merged
   @param[in]  value: a float representing energy or momentum etc
   @param[in]  uniqueid: not used in normal use
   @return identifier
   */
  static IdType makeId(unsigned int uniqueid, ItemType type, char subtype = 'u', float value = 0.0);

  /** returns the item type of the the identifier
   This is one of: None = 0, kEcalCluster = 1, kHcalCluster, kTrack, kParticle, kBlock
   @param[in] id: the unique identifier
   @return an enum IdCoder::ItemType
   */
  static ItemType itemType(IdType id);  ///< Returns encoded ItemType eg kParticle etc;

  /** returns the subtype of the identifier eg 'm'
   Some possible existing uses
   eg
   'g' generated
   'r' reconstructed
   'u' unspecified
   't' true
   's' simulated (particles)
        smeared (tracks ecals hcals)
        split (blocks)
   @param[in] id: unique identifier
   @return single letter subtype
   */
  static char subtype(IdType id);  /// return the one letter subtype

  /** returns the float value encoded in the identifier
   @param[in] id: unique identifier
   @return the encoded value
   */
  static float value(IdType id);  /// return the float value

  /** Takes an identifier and returns the index component of it
   @param[in] id: unique identifier
   @return the  index
   */
  static unsigned int index(IdType id);  ///< Returns encoded unique id
  

  /** Takes an identifier and returns the unique counter component of it
   @param[in] id: unique identifier
   @return the unique counter
   */
  static unsigned int uniqueId(IdType id);  ///< Returns encoded unique id

  static char typeLetter(IdType id);  ///< One letter short code eg 'e' for ecal, 't' for track, 'x' for unknown
  static std::string typeAndSubtype(IdType id);  ///< Two letter string of type and subtype eg "em"
  static std::string pretty(IdType id);  ///< Pretty string Id name eg "es101" for a smeared ecal with uniqueid 101;
  /** boolean test of whether identifier is from an ecal cluster
  @param ident: unique identifier
   */
  static bool isEcal(IdType id) { return (IdCoder::itemType(id) == kEcalCluster); }

  /** boolean test of whether identifier is from an hcal cluster
   @param ident: unique identifier
   */
  static bool isHcal(IdType id) { return (IdCoder::itemType(id) == kHcalCluster); }

  /** boolean test of whether identifier is from a cluster
   @param ident: unique identifier
   */
  static bool isCluster(IdType id) { return (IdCoder::isEcal(id) || IdCoder::isHcal(id)); }

  /** boolean test of whether identifier is from an track
   @param ident: unique identifier
   */
  static bool isTrack(IdType id) { return (IdCoder::itemType(id) == kTrack); }

  /** boolean test of whether identifier is from a particle
   @param ident: unique identifier
   */
  static bool isParticle(IdType id) { return (IdCoder::itemType(id) == kParticle); }

  /** boolean test of whether identifier is from a block
   @param ident: unique identifier
   */
  static bool isBlock(IdType id) { return (IdCoder::itemType(id) == kBlock); }

  /** Uses detector layer to work out what itemType is appropriate
   @param layer: detector layer as an enumeration eg kEcal
   @return ItemType enumeration value eg kEcalCluster
   */
  static ItemType itemType(papas::Layer layer);  ///< Uses detector layer to work out itemType
  static ItemType itemType(char s);

  /** Uses identifier type to work out what detector layer the item belongs to, may be kNone
   @param id: unique identifier
   @return ItemType enumeration value papas::Layer eg kTracker
   */
  static papas::Layer layer(IdType id);

  // Used by Edge, must be better way to do this?
  static int bitshift() { return m_bitshift; }

private:
  static const unsigned int m_bitshift1 = 61;  ///< encoding parameter
  static const unsigned int m_bitshift2 = 53;  ///< encoding parameter
  static const unsigned int m_bitshift = 21;   ///< encoding parameter (max size of counter)
  /// checks that the identifier can be correctly decoded
  static bool checkValid(IdType id, ItemType type, char subt, float val, unsigned int uid);
  static bool checkUIDValid(IdType id, unsigned int uniqueid);
  static uint64_t floatToBits(float value);  /// convert float into binary
  static float bitsToFloat(uint64_t bits);   /// convert binary into float
};
}  // end namespace
#endif /* IdCoder_h */
