#ifndef FASTSIMRANDOM_H
#define FASTSIMRANDOM_H

#include <random>
#include <functional>
#include <iostream>


namespace fastsim{
   
  
   //extern std::default_random_engine engine;
   
   
   class RandNormal;
   extern RandNormal makeseed; //should prob make this a uniform distrubution instead
 
   
   
   class RandNormal{
   public:
      /// If seed is set manually then will give repeatable results for other RandNormal instances
      // If makeseed is used it means that instances of RandNormal are independent but
      // can be consistenlty generated usng the makeseeds own seed.
      RandNormal(double mean, double sd, double seed=fastsim::makeseed()) :
      r(std::bind(std::normal_distribution<>(mean,sd),std::default_random_engine(seed))) { };
      double operator() () const {return r(); };  ///generate values
      
   private:
      std::function<double()> r;
   };
   
   
   class RandExponential{
   public:
      RandExponential(double lambda, double seed=fastsim::makeseed()) : // this will start from a known point and thus be repeatable
       r(std::bind(std::exponential_distribution<>(lambda),std::default_random_engine(seed))) {};
      double operator() () const {return r(); }; ///generate values
   private:
     std::function<double()> r; //the "bind" function as extracted from Stroustrup example
      
   };
   
   extern double randomGaussian(double mean, double sd);
   
   /*
   class RandNormal{
   public:
      RandNormal(double mean, double sd) ///this will seed randomly
      :  r (std::bind(std::normal_distribution<>(mean,sd),std::default_random_engine(rd()))){};
      RandNormal(double mean, double sd, double seed): /// this will start from a known point and thus be repeatable
      r(std::bind(std::normal_distribution<>(mean,sd),std::default_random_engine(seed))) {};
      double operator() () const {return r(); };  ///generate values
   private:
      
      std::function<double()> r;
      
   };
   class RandExponential{
   public:
      RandExponential(double lambda) : ///this will seed randomly
         r (std::bind(std::exponential_distribution<>(lambda),std::default_random_engine(rd()))){};
      RandExponential(double lambda, double seed) : // this will start from a known point and thus be repeatable
       r(std::bind(std::exponential_distribution<>(lambda),std::default_random_engine(seed))) {};
      double operator() () const {return r(); }; ///generate values
   private:
      //std::random_device rd; //used to set a random seed so that two instances of this class do not give same values
      std::function<double()> r; //the "bind" function as extracted from Stroustrup example
      
   };*/


}





#endif
