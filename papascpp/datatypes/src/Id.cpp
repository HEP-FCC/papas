//
//  Id.cpp
//  papas
//
//  Created by Alice Robson on 19/05/16.
//
//

#include "Id.h"

#include <cmath>
#include <inttypes.h> 
#include <iostream> //temp
//  Created by Alice Robson on 05/01/16.
//
//Encode information into a unique identifier
//
// accessible intro to use of bitwise operations can be found here
//http://stackoverflow.com/questions/6556961/use-of-the-bitwise-operators-to-pack-multiple-values-in-one-int
//


namespace papas {
  
  unsigned int Id::s_counter = 1; /// static which will be used to create a unique long
                                  //const unsigned int Id::bitshift =32;
  /*void Id::setCounter(int startid) /// allows user to start counter at another point
  {
    s_counter = startid;
  }*/
  
  //TODO rename as OBJECTTYpe not PFObjectType
  Id::Type Id::makeId(ItemType type, unsigned int uniqueid)
  {
    
     //default is to start at 1 for first id so that id=0 means unset
                 //long id = (uniqueid << 16) | ((int)source << 13) | ((int)subtype << 10) | ((
                 //int)layer << 6) | (int)type;
    
    if (type>6) { //TODO error cghecking on type
      std::cout <<"TOO big" << std::endl;
    }
    //NB uint64_t is needed to make sure the shift is carried out over 64 bits, otherwise
    //if the btshift is 32 or more the shift is undefined and can return 0
    Type id =  ( ((uint64_t)type) << bitshift) | uniqueid;
    s_counter++;

    std::cout << "makeID: "  << id << " = "<< type << " : uid = " << uniqueid << std::endl;;
       
    /*std::cout<< "size type "<< sizeof(unsigned long)<<std::endl;
    std::cout<< "size type "<< sizeof(unsigned long long)<<std::endl;
    std::cout<< "size unsigned int "<< sizeof(unsigned int)<<std::endl;*/
    return id;
  }
  
  
  Id::ItemType Id::itemType(Type id)
  {
    return static_cast<ItemType>(id >> bitshift);
  }
  
  unsigned int Id::uniqueId(Type id)
  {
    return id & (uint64_t)(pow(2, bitshift) - 1) ; }
  
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
    std::string typelist = ".ehtprb....";
    return typelist[(unsigned int)Id::itemType(id)];
    //TODO error handling
  }
} // end namespace papas


 
 

