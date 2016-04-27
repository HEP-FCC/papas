//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  MATERIAL_H
#define MATERIAL_H

#include <string>
#include <limits>
#include "enums.h"
#include "rand.h"

const int c_vecsize = 10000;
const double c_exp_lambda = 3.5; //AJR where does this come from

///Material
/**
  Users need to define material properties for their detectors
*/
class Material {
public:
   Material(/*fastsim::enumLayer layer,*/ double x0, double lambda);
   virtual double pathLength(bool is_em) const;
  
private:
  //fastsim::enumLayer m_layer;
   double m_x0;
   double m_lambdaI;
   randomgen::RandExponential m_randomLambda;
   randomgen::RandExponential m_randomX0;
  };


//Temporary
class TestMaterial: public Material {
public:
   TestMaterial( double x0, double lambda);
   double PathLength(/*Particle &*/);
private:
   //fastsim::enumLayer m_layer;
   //double m_x0;
   //double m_lambdaI;
};

#endif

