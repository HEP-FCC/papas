

//#include <iostream>
#include "path.h"
#include "propagator.h"
#include "geotools.h"
#include "PFParticle.h"



Propagator::Propagator()
{
}

StraightLinePropagator::StraightLinePropagator()
{
}

void StraightLinePropagator::propagateOne(PFParticle& ptc,
                                          std::string cylindername,
                                          double cylinderz,
                                          double cylinderradius)
{
  std::shared_ptr<Path> line = ptc.path();
   
   TVector3 udir = line->udir();
   TVector3 origin = line->origin();
   double theta = udir.Theta();
   
   double zbar = line->udir().Z(); // Z of unit vex
   if (zbar != 0) {
      double destz = (zbar > 0) ? cylinderz : -cylinderz;
      double length = (destz - origin.Z()) / cos(theta); //TODO check Length >0
      TVector3 destination = origin + udir * length;
      double rdest = destination.Perp();
      if (rdest > cylinderradius) {
         TVector3 udirxy = TVector3(udir.X(), udir.Y(), 0.);
         TVector3 originxy = TVector3(origin.X(), origin.Y(), 0.);
         // solve 2nd degree equation for intersection
         // between the straight line and the cylinder
         // in the xy plane to get k,
         // the propagation length
         double a = udirxy.Mag2();
         double b = 2 * udirxy.Dot(originxy);
         double c = originxy.Mag2() - pow(cylinderradius, 2);
         double delta = pow(b, 2) - 4 * a * c;
         //double km = (-b - sqrt(delta))/(2*a);
         // positive propagation -> correct solution.
         double kp = (-b + sqrt(delta)) / (2 * a);
         // print delta, km, kp
         destination = origin + udir * kp;
         //TODO deal with Z == 0
         //TODO deal with overlapping cylinders
         
      }
#if 0
         std::cout << " X " << destination.X() << " Y" << destination.Y() << " Z " <<
          destination.Z() << std::endl;
#endif
      line->addPoint(cylindername, destination);
   }
}


void StraightLinePropagator::propagateOne(PFParticle& ptc,
                                          const SurfaceCylinder & cyl)
{
   propagateOne(ptc, cyl.getName(), cyl.Z(), cyl.getRadius());
}



HelixPropagator::HelixPropagator(double field) :
   m_field(field)
{
}


void HelixPropagator::propagateOne(PFParticle& ptc,
                                   const SurfaceCylinder & cyl)
{
   auto helix = std::static_pointer_cast<Helix>(ptc.path());
   
   bool is_looper = helix->extremePointXY().Mag() < cyl.getRadius();
   double udir_z=helix->udir().Z();
   
   if (!is_looper) {
      auto intersect =
      circleIntersection(helix->centerXY().X(),helix->centerXY().Y(), helix->rho(), cyl.getRadius());
      
      double phi_m = helix->phi(intersect[0].first, intersect[0].second);
      double phi_p = helix->phi(intersect[1].first, intersect[1].second);
      
      TVector3 destination = helix->pointAtPhi(phi_p);
      if (destination.Z()*udir_z<0.) {
         destination = helix->pointAtPhi(phi_m);
      }
      
      if (fabs(destination.Z())<cyl.Z()){
         helix->addPoint(cyl.getName(),destination);
      }
      else
         is_looper = true;
   }
   if (is_looper)
   {
      double destz = cyl.Z();
      if (udir_z < 0.)
         destz = -destz;
      TVector3 destination = helix->pointAtZ(destz);
      helix->addPoint(cyl.getName(), destination);
   }
   
}



      
       
       
       
       
       
       
       
    
