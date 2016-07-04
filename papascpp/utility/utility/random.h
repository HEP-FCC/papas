//
//  rand.hpp
//  papas
//
//  Created by Alice Robson on 26/04/16.
//
//

#ifndef rand_h
#define rand_h

#include <random>

namespace randomgen {
  
extern std::random_device rdevice;
extern  void  setEngineSeed(double seed= rdevice());
  
class RandUniform{
public:
  /// If a seed if given this will start from a known point and thus be repeatable
  RandUniform(double from, double to);
  double next();
  static void setSeed(double);
private:
  std::uniform_real_distribution<> m_dist;
};

class RandNormal{
public:
  /// If seed is set manually then will give repeatable results for other RandNormal instances
  // If makeseed is used it means that instances of RandNormal are independent but
  // can be consistenlty generated usng the makeseeds own seed.
  RandNormal(double mean, double sd);
  double next();
  static void setSeed(double);
private:
  std::normal_distribution<> m_dist;
};


class RandExponential{
public:
  /// if seeded this will start from a known point and thus be repeatable
  RandExponential(double lambda);
  double next();
  static void setSeed(double);
private:
  std::exponential_distribution<double>  m_dist;
};
  
}


 #endif /* rand_hpp */
