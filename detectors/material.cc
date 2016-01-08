//
//  Created by Alice Robson on 09/11/15.
//
//
#include "material.h"

#include <random>
//class Particle;


/* Decides whether a cluster will be seen by a detector
 @param name Name of material
 @param x0 ??set to
 @param lambdaI ??? AJRTODO
 @return true is cluster is detected, false it if is too small to be seen
 */

Material::Material(const std::string& name, double x0,
                   double lambdaI) : m_name(name), m_x0(x0), m_lambdaI(lambdaI)
{
}



double Material::PathLength(/*AJRTODO const Particle& ptc)*/)
{
   //AJRTODO check with Colin about what happens if x= is none
   double freepath;
   /*if (ptc.is_em()){
       freepath = m_x0;
   }
   else{
       freepath= m_lambaI;
   }*/
   freepath = m_x0;
   freepath = m_lambdaI; //AJRTODO remove this reinstate above

   if (freepath == 0.) {
      return std::numeric_limits<double>::max(); //AJR or float?
   } else { // make random number exp distribution
      std::default_random_engine generator;
      std::exponential_distribution<double> distribution(
         c_exp_lambda); //AJR doubl check as expected & write test
      double rnumber = distribution(generator);
      return rnumber;
   }
}



TestMaterial::TestMaterial(const std::string&  name, double x0,
                           double lambdaI): Material(name, x0, lambdaI)
{
}



double TestMaterial::PathLength(/*AJRTODO const Particle& ptc)*/)
{
   return 99.999;
}
/*
void = Material('void', 0, 0)


if __name__ == '__main__':
import matplotlib.pyplot as plt
a = np.random.exponential(25., 10000)
n, bins, patches = plt.hist(a, 50, normed=1, facecolor='green', alpha=0.75)
plt.show()*/
