#include "random.h"

namespace fastsim{
   std::default_random_engine engine;
   RandNormal makeseed(10000,1000000,5);//mean, sd and seed. The seed can be changed and will change all program results
                                        // if left same program results should be consistent. needs to be checked across machines however
   double randomGaussian(double mean, double sd)
   {
      auto  rn= RandNormal(mean,sd);
      return rn();
      
   }
}