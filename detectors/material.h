//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef  MATERIAL_H
#define MATERIAL_H

#include <string>
#include <limits>


const int c_vecsize = 10000;
const double c_exp_lambda = 3.5; //AJR where does this come from

///Material
/**
  Users need to define material properties for their detectors
*/
class Material {
public:
   Material(const std::string&, double x0, double lambda);
   virtual double PathLength(/*AJRTODO const Particle &*/);
private:
   const std::string m_name;
   double m_x0;
   double m_lambdaI;
};


//Temporary
class TestMaterial: public Material {
public:
   TestMaterial(const std::string&, double x0, double lambda);
   double PathLength(/*Particle &*/);
private:
   const std::string m_name;
   //double m_x0;
   //double m_lambdaI;
};

#endif

