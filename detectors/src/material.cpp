//
//  Created by Alice Robson on 09/11/15.
//
//

#include "material.h"

Material::Material(/*fastsim::enumLayer layer,*/ double x0, double lambdaI) :
//m_layer(layer),
    m_x0(x0),
    m_lambdaI(lambdaI) ,
    m_randomLambda(lambdaI),
    m_randomX0(x0) {

}

double Material::pathLength(bool is_em) const
{
   //AJRTODO check with Colin about what happens if x= is none
   double freepath;
   if (is_em){
       freepath = m_x0;
       if (m_x0 == 0.) {
         return std::numeric_limits<double>::max();
       }
     //return m_randomX0(); //TODO reinstante once testing is complete
     return m_x0*0.8;
   }
   else{
     if (m_lambdaI == 0.) {
       return std::numeric_limits<double>::max();
     }
     //return m_randomLambda.next(); //TODO reinstante once testing is complete
     return m_lambdaI*0.8;
   }
 
}


TestMaterial::TestMaterial(double x0, double lambdaI): Material( x0, lambdaI)
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
