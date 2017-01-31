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
/** @brief Class to provide random number generation based on ROOT random numbers
  *
  * Note that the use of ROOT to provide random numbers allows the random number generation in Python and in
  * cpp to be aligned with each other.
  */
class Random {
public:
  /**  @brief  Sets the random generator seed
   *   @param[in]  seed value of the seed. If set to 0 (default) then the seed will be a random number      */
  static void seed(long seed = 0) { s_random = TRandom(seed); }
  /**  @brief  produce an exponential random number for the inverse of the parameter
   *   @param[in]  a 1 over exponential distribution parameter     */
  static double expovariate(double a) { return s_random.Exp(1. / a); }
  /**  @brief  produce an exponential random number
   *   @param[in]  a exponential distribution parameter     */
  static double exponential(double a) { return s_random.Exp(a); }
  /**  @brief  produce an gaussian/normally distributed random number
   *   @param[in]  a mean
   *   @param[in]  b stddev*/
  static double gauss(double a, double b) { return s_random.Gaus(a, b); }
  /**  @brief  produce uniformly distributed random value in the interval a, b
   *   @param[in]  a start of interval
   *   @param[in]  b end of interval*/
  static double uniform(double a, double b) { return s_random.Uniform(a, b); }

private:
  static TRandom s_random; ///< Root random number producer.
};
}

#endif /* TRandom_h */