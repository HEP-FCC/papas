//
//  Created by Alice Robson on 09/11/15.
//
//

#include "material.h"

Material::Material(fastsim::enumLayer layer, double x0, double lambdaI) :
m_layer(layer), m_x0(x0), m_lambdaI(lambdaI),m_randExp(new fastsim::RandExponential(lambdaI))
{
}

double Material::pathLength(bool is_em) const
{
   //AJRTODO check with Colin about what happens if x= is none
   double freepath;
   if (is_em){
       freepath = m_x0;
   }
   else{
       freepath= m_lambdaI;
   }
   
   if (freepath == 0.) {
      return std::numeric_limits<double>::max(); //maximum value for a double
   } else { 
      return (*m_randExp)();
   }
}


TestMaterial::TestMaterial(fastsim::enumLayer layer, double x0,
                           double lambdaI): Material(layer, x0, lambdaI)
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
