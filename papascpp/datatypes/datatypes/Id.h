//
//  Id.hpp
//  papas
//
//  Created by Alice Robson on 19/05/16.
//
//

#ifndef Id_hpp
#define Id_hpp

#include <inttypes.h>
#include <iostream>
#include "Definitions.h"

namespace papas {
  
  class Id {
  public:
    Id() {};
    typedef IdType Type;
    
    //typedef papas::Subtype  papas::SubType;
    typedef papas::enumSource  eSource;
    enum ItemType{kNone=0,  kEcalCluster=1, kHcalCluster, kTrack, kParticle, kRecParticle, kBlock};
    
    static Id::Type makeId(ItemType type, //check name with Colin
                       unsigned int uniqueid = Id::s_counter);
    //static long makeAnotherId(long
    //                          existingid);///uses same enums as existing id  but sets a new uniqueId.
    
    static Id::Type makeEcalId(unsigned int uniqueid = Id::s_counter) {return makeId(kEcalCluster, uniqueid);}
    static Id::Type makeHcalId(unsigned int uniqueid = Id::s_counter) {return makeId(kHcalCluster, uniqueid);}
    static Id::Type makeTrackId(unsigned int uniqueid = Id::s_counter) {return makeId(kTrack, uniqueid);}
    static Id::Type makeParticleId(unsigned int uniqueid = Id::s_counter) {return makeId(kParticle, uniqueid);}
    static Id::Type makeRecParticleId(unsigned int uniqueid = Id::s_counter) {return makeId(kRecParticle, uniqueid);}
    static Id::Type makeBlockId(unsigned int uniqueid = Id::s_counter) {return makeId(kBlock, uniqueid);}

    static papas::Layer layer(Type id) ;
    
    
    static bool isEcal(Type id) {return (Id::itemType(id)==kEcalCluster);}
    static bool isHcal(Type id) {return (Id::itemType(id)==kHcalCluster);}
    static bool isCluster(Type id) {return (Id::isEcal(id) || Id::isHcal(id));}
    static bool isTrack(Type id) {return (Id::itemType(id)==kTrack);}
    static bool isParticle(Type id) {return (Id::itemType(id)==kParticle);}
    static bool isRecParticle(Type id) {return (Id::itemType(id)==kRecParticle);}
    static bool isBlock(Type id)  {return (Id::itemType(id)==kBlock);}
    
    static ItemType itemType(Type id);
    static unsigned int uniqueId(Type id);
    
    static char typeShortCode(Type id); ///One letter code eg 'e' for ecal, 't' for track, 'x' for unknown
                                        //static int uniqueId(long id);
    
    //TODO add a checkValid function
  private:
    static unsigned int s_counter;
    static const unsigned int bitshift = 32;
  
  
  };
}// end nam
#endif /* Id_hpp */
