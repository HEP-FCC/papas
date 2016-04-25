#include "random.h"

namespace fastsim{
   std::default_random_engine engine;
   std::exponential_distribution<double> expDist(1.);
  
   //makeseed is a uniform number generator that may be used to see all the random numbers used in FASTSIM
   // Option One:
   //    set the seed (3rd argument in the makeseed constructor below).
   //    using this seed will mean that the same numbers are generated wach time the program is run
   //
   // Option Two:
   //    supply something that is random for the 3rd argument (eg time of run) this will give different results
   //    for each run of the protram
   
   RandUniform makeseed(1,1000000,5);//from to and seed. The seed can be changed and will change all program results
                                        // if left same program results should be consistent. needs to be checked across machines however
   double randomGaussian(double mean, double sd)
   {
      auto  rn= RandNormal(mean,sd);
      return rn();  
   }
   
   double randomUniform(double from, double to)
   {
      auto  rn= RandUniform(from, to);
      return rn();
      
   }
   
   double randomExponential(double lambda)
   {
      auto  rn= RandExponential(lambda);
      return rn();
   }
  
 
}