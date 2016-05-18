
#ifndef propogator_h
#define propogator_h
#include "detector.h"

namespace papas {
class PFParticle;


class Propagator {
public:
   Propagator();
   //virtual void propagateOne(PFParticle& ptc, Id::Layer Layer,
   //                          bool inner = true) = 0;
   
   virtual void propagateOne(PFParticle& ptc, const SurfaceCylinder & cyl)=0;

protected:
  virtual void propagateOne(PFParticle& ptc, papas::Position layer,
                             double cylinderz, double cylinderradius) = 0;

};

class StraightLinePropagator  {
public:
   StraightLinePropagator();
   //void propagateOne(PFParticle& ptc, Id::Layer Layer,
   //                  bool inner = true) override;
   void propagateOne(PFParticle& ptc, const SurfaceCylinder & cyl) ;

private:
   void propagateOne(PFParticle& ptc, papas::Position layer, double cylinderz,
                     double cylinderradius) ;


};

class HelixPropagator {
public:
   HelixPropagator( double field);
   virtual void propagateOne(PFParticle& ptc, const SurfaceCylinder & cyl) ;
   private:
   double m_field;
   void propagateOne(PFParticle& ptc, papas::Position layer, double cylinderz,
                     double cylinderradius,const Field& field,
                     bool debugInfo) ;


};
} // end namespace papas
#endif
