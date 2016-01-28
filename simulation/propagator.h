
#ifndef propogator_h
#define propogator_h
#include "detector.h"
#include "datatypes.h"


class Propagator {
public:
   Propagator();
   //virtual void propagateOne(SimParticle& ptc, fastsim::enumLayer Layer,
   //                          bool inner = true) = 0;
   
   virtual void propagateOne(SimParticle& ptc, const SurfaceCylinder & cyl)=0;

protected:
   virtual void propagateOne(SimParticle& ptc, std::string cylindername,
                             double cylinderz, double cylinderradius) = 0;

};

class StraightLinePropagator  {
public:
   StraightLinePropagator();
   //void propagateOne(SimParticle& ptc, fastsim::enumLayer Layer,
   //                  bool inner = true) override;
   void propagateOne(SimParticle& ptc, const SurfaceCylinder & cyl) ;

private:
   void propagateOne(SimParticle& ptc, std::string cylindername, double cylinderz,
                     double cylinderradius) ;


};

class HelixPropagator {
public:
   HelixPropagator( double field);
   virtual void propagateOne(SimParticle& ptc, const SurfaceCylinder & cyl) ;
   private:
   double m_field;
   void propagateOne(SimParticle& ptc, std::string cylindername, double cylinderz,
                     double cylinderradius,const Field& field,
                     bool debugInfo) ;


};
#endif

