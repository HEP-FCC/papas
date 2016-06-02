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


/** @class   Id Datatypes/Datatypes/Id.h Id.h
 *
 *  @brief Encode information into a unique identifier
 *  Example usage: Id::Type uid=Id::MakeId(kEcalCluster)
 *
 *  @author  Alice Robson
 *  @date    2016-04-05
 */


namespace papas {
  
  /** A Particle Flow Block stores a set of element ids that are connected to each other
   together with the edge data (distances) for each possible edge combination
   
   class attributes:
   
   uniqueid : the block's unique id generated from Id class
   element_uniqueids : list of uniqueids of its elements
   pfevent : contains the tracks and clusters and a get_object method to allow access to the
   underlying objects given their uniqueid
   edges : Dictionary of all the edge cominations in the block dict{edgekey : Edge}
   use  get_edge(id1,id2) to find an edge
   is_active : bool true/false, set to false if the block is subsequently subdivided
   
   Usage:
   block = PFBlock(element_ids,  edges, pfevent)
   for uid in block.element_uniqueids:
   print pfevent.get_object(uid).__str__() + "\n"
   
   */


  
  unsigned int Id::s_counter = 1; /// static which will be used to create a unique long
                                  //const unsigned int Id::bitshift =32;
  /*void Id::setCounter(int startid) /// allows user to start counter at another point
  {
    s_counter = startid;
  }*/
  
  //TODO rename as OBJECTTYpe not PFObjectType
  Id::Type Id::makeId(ItemType type, unsigned int uniqueid)
  {

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


 
 

