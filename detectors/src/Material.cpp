//
//  Created by Alice Robson on 09/11/15.
//
//

#include "Material.h"

namespace papas {

Material::Material(double x0, double lambdaI) : m_x0(x0), m_lambdaI(lambdaI) {}

double Material::pathLength(bool isEm) const {
  // AJRTODO check with Colin about what happens if x= is none
  double freepath;
  if (isEm)
    freepath = m_x0;
  else
    freepath = m_lambdaI;

  if (freepath == 0.) {
    return std::numeric_limits<double>::max();
  }

  else {
    double pl = randomgen::RandExponential(1. / freepath).next();
    return pl;
  }
}

TestMaterial::TestMaterial(double x0, double lambdaI) : Material(x0, lambdaI) {}

double TestMaterial::PathLength(/*AJRTODO const Particle& ptc)*/) { return 99.999; }

}  // end namespace papas
