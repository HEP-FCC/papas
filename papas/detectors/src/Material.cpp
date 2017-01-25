//
//  Created by Alice Robson on 09/11/15.
//
//

#include "papas/detectors/Material.h"
#include "papas/utility/TRandom.h"

namespace papas {

Material::Material(std::string name, double x0, double lambdaI) : m_name(name), m_x0(x0), m_lambdaI(lambdaI) {}

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
    double pl = rootrandom::Random::expovariate(1. / freepath);

    return pl;
  }
}
}
// end namespace papas