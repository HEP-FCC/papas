#ifndef FASTSIMRANDOM_H
#define FASTSIMRANDOM_H

#include <random>
#include <functional>
#include <iostream>


namespace fastsim{
   
  
   
   class RandUniform;
   extern RandUniform makeseed; //should prob make this a uniform distrubution instead
 
   
   
   class RandUniform{
   public:
      /// If a seed if given this will start from a known point and thus be repeatable
      RandUniform(double from , double to, double seed=fastsim::makeseed()) :
        r(std::bind(std::uniform_real_distribution<double>(from, to),std::default_random_engine(seed))) {};
      double operator() () const {return r(); }; ///generate values
   private:
      std::function<double()> r; //the "bind" function as extracted from Stroustrup example
      
   };
  
   class RandNormal{
   public:
      /// If seed is set manually then will give repeatable results for other RandNormal instances
      // If makeseed is used it means that instances of RandNormal are independent but
      // can be consistenlty generated usng the makeseeds own seed.
      RandNormal(double mean, double sd, double seed= (fastsim::makeseed)()) :
      r(std::bind(std::normal_distribution<>(mean,sd),std::default_random_engine(seed))) { };
      double operator() () const {return r(); };  ///generate values
      
   private:
      std::function<double()> r;
   };
   
   
   class RandExponential{
   public:
     /// if seeded this will start from a known point and thus be repeatable

      RandExponential(double lambda, double seed=fastsim::makeseed()) :
        r(std::bind(std::exponential_distribution<double>(lambda),std::default_random_engine(seed))) {};
      double operator() () const {return r(); }; ///generate values
                                                 //double operator() (double mean) const {return r(mean); }; ///generate values
   private:
     std::function<double()> r; //the "bind" function as extracted from Stroustrup example
   };
  
  
  
   extern double randomGaussian(double mean, double sd);
   extern double randomExponential(double lambda);
   extern double randomUniform(double from, double to);
  extern double testExponential(double lambda);


}





#endif
