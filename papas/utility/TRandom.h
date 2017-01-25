//
//  TRandom.h
//  papas
//
//  Created by Alice Robson on 16/01/17.
//
//

#ifndef TRandom_h
#define TRandom_h

#include <TRandom.h>

namespace rootrandom {

class Random {
public:
  static void seed(long seed = 0) {  // if 0 seed is effectivle a random number
    s_random = TRandom(seed);
  }
  static double expovariate(double a) { return s_random.Exp(1. / a); }
  static double exponential(double a) { return s_random.Exp(a); }
  static double gauss(double a, double b) { return s_random.Gaus(a, b); }
  static double uniform(double a, double b) { return s_random.Uniform(a, b); }

private:
  static TRandom s_random;
};
}

#endif /* TRandom_h */
