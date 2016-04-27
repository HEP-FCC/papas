
#ifndef propogator_h
#define propogator_h
#include "detector.h"
#include "datatypes.h"


class Propagator {
public:
   Propagator();
   //virtual void propagateOne(PFParticle& ptc, fastsim::enumLayer Layer,
   //                          bool inner = true) = 0;
   
   virtual void propagateOne(PFParticle& ptc, const SurfaceCylinder & cyl)=0;

protected:
   virtual void propagateOne(PFParticle& ptc, std::string cylindername,
                             double cylinderz, double cylinderradius) = 0;

};

class StraightLinePropagator  {
public:
   StraightLinePropagator();
   //void propagateOne(PFParticle& ptc, fastsim::enumLayer Layer,
   //                  bool inner = true) override;
   void propagateOne(PFParticle& ptc, const SurfaceCylinder & cyl) ;

private:
   void propagateOne(PFParticle& ptc, std::string cylindername, double cylinderz,
                     double cylinderradius) ;


};

class HelixPropagator {
public:
   HelixPropagator( double field);
   virtual void propagateOne(PFParticle& ptc, const SurfaceCylinder & cyl) ;
   private:
   double m_field;
   void propagateOne(PFParticle& ptc, std::string cylindername, double cylinderz,
                     double cylinderradius,const Field& field,
                     bool debugInfo) ;


};
#endif

