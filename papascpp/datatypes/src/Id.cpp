//
//  Id.cpp
//  papas
//
//  Created by Alice Robson on 19/05/16.
//
//

#include "Id.h"


#include <iostream> //temp
//  Created by Alice Robson on 05/01/16.
//
//Encode information into a unique identifier
//
// accessible intro to use of bitwise operations can be found here
//http://stackoverflow.com/questions/6556961/use-of-the-bitwise-operators-to-pack-multiple-values-in-one-int
//


namespace papas {
  
  int Id::s_counter = 0; /// static which will be used to create a unique long
  
  /*void Id::setCounter(int startid) /// allows user to start counter at another point
  {
    s_counter = startid;
  }*/
  
  //TODO rename as OBJECTTYpe not PFObjectType
  Id::Type Id::makeId(ItemType type, int uniqueid)
  {
    // consider reordering to source ... layer .. type ,.. subtype ..uniqueid
    s_counter++; //default is to start at 1 for first id so that id=0 means unset
                 //long id = (uniqueid << 16) | ((int)source << 13) | ((int)subtype << 10) | ((
                 //int)layer << 6) | (int)type;
    
    //long long thing = ((int)type) << 20;
    //std::cout<< type << ":" << thing << std::endl;
    Type id =  ( ((int)type) << 20) | uniqueid;
    return id;
  }
  
  
  Id::ItemType Id::itemType(Type id)
  {
    return static_cast<ItemType>(id >> 20);
  }
  
  int Id::uniqueId(Type id)
  {
    return id &  0xFFFFF; // 0xFFFFF == 0b11111111111111111111;//11111111111111111111;
  }
  
  papas::Layer Id::layer(Type id) {
    if(Id::isEcal(id) )
      return papas::Layer::kEcal;
    else if(Id::isHcal(id) )
      return papas::Layer::kHcal;
    else if(Id::isTrack(id) )
      return papas::Layer::kTracker;
    else
      return papas::Layer::kNone;
  }
  
  char Id::typeShortCode(Id::Type id)
  {
    std::string typelist = ".eht.......";
    return typelist[(int)Id::itemType(id)];
    /*if (isEcal(id))
      return 'e';
    else if (isHcal(id))
      return 'h';
    else if(isTrack(id))
      return 't';
    else if (isParticle(id))
      return 'p';
    else
      return 'x';*/
  }
} // end namespace papas


 
 

