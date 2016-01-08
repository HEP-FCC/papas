#ifndef MY_UTILITIES_H
#define MY_UTILITIES_H

// c++
#include <iostream>
#include <sstream>
#include <fstream>

// Root

#include "TMath.h"
#include "TVector3.h"

//! my_utilities class
class my_utilities  {

public:

   //! my_utilities constructor
   my_utilities();
   //! my_utilities destructor
   ~my_utilities();

   //! my return
   Float_t myReturn(Float_t input);
   bool is_point(TVector3& point);


};
#endif // MY_UTILITIES_H
