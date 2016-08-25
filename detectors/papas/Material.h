//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef MATERIAL_H
#define MATERIAL_H

//#include <string>
//#include <limits>
#include "Definitions.h"
#include "Random.h"

namespace papas {

/// Detector Material - users need to define material properties for their detector elements
class Material {
public:
  Material(double x0, double lambda);
  virtual double pathLength(bool isEm) const;

private:
  double m_x0;
  double m_lambdaI;
};

  /*// Temporary //TODO
class TestMaterial : public Material {
public:
  TestMaterial(double x0, double lambda);
  double PathLength();

private:
};*/
}  // end namespace papas
#endif
