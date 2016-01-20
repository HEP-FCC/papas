

//#include <iostream>
#include "path.h"
#include "pfobjects.h"
#include "propagator.h"

Propagator::Propagator(const BaseDetector& det)
   : m_detector(det)
{

}

void StraightLinePropagator::propagateOne(SimParticle& ptc,
      std::string cylindername, double cylinderz, double cylinderradius)
{
   Path& line = ptc.getPath();
   //print particle.p3().X(),particle.p3().Y(),particle.p3().Z()
   //print particle.vertex.X(),particle.vertex.Y(),particle.vertex.Z()
   //particle.set_path( line )

   TVector3 udir = line.getUdir();
   TVector3 origin = line.getOrigin();
   double theta = udir.Theta();

   double zbar = line.getUdir().Z(); // Z of unit vex
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
         TVector3 destination = origin + udir * kp;
         //TODO deal with Z == 0
         //TODO deal with overlapping cylinders
         std::cout << " X " << destination.X() << " Y" << destination.Y() << " Z " <<
                   destination.Z() << std::endl;
         line.addPoint(cylindername, destination);
      }

   }
}
void StraightLinePropagator::propagateOne(SimParticle& ptc,
      fastsim::enumLayer layer, bool inner)
{
   //DetectorElement elem= ;
   const VolumeCylinder& V = m_detector.getElement(layer)->getVol();
   propagateOne(ptc, V.InnerName(), V.Inner()->Z(), V.Inner()->getRadius());
   //TODO outer as well

}

/*
            class HelixPropagator(Propagator):

            def propagate_one(self, particle, cylinder, field, debug_info=None):
            helix = Helix(field, particle.q(), particle.p4(),
                          particle.vertex)
            particle.set_path(helix)
            is_looper = helix.extreme_point_xy.Mag() < cylinder.rad
            is_positive = particle.p4().Z() > 0.
            if not is_looper:
               xm, ym, xp, yp = circle_intersection(helix.center_xy.X(),
                                                    helix.center_xy.Y(),
                                                    helix.rho,
                                                    cylinder.rad)
 // particle.points[cylinder.name+'_m'] = Point(xm,ym,0)
 // particle.points[cylinder.name+'_p'] = Point(xp,yp,0)
               phi_m = helix.phi(xm, ym)
               phi_p = helix.phi(xp, yp)
               dest_time = helix.time_at_phi(phi_p)
               destination = helix.point_at_time(dest_time)
               if destination.Z()*helix.udir.Z()<0.:
                  dest_time = helix.time_at_phi(phi_m)
                  destination = helix.point_at_time(dest_time)
                  if abs(destination.Z())<cylinder.z:
                     particle.points[cylinder.name] = destination
                     print  str(particle.pdgid) + " : helix point "  + cylinder.name + str( destination.Z())
                     else:
                        is_looper = True
                        if is_looper:
# extrapolating to endcap
                           destz = cylinder.z if helix.udir.Z() > 0. else -cylinder.z
                              dest_time = helix.time_at_z(destz)
                              destination = helix.point_at_time(dest_time)
# destz = cylinder.z if positive else -cylinder.z
                              particle.points[cylinder.name] = destination
                              print "helix point"  + cylinder.name

                              info = Info()
                              info.is_positive = is_positive
                              info.is_looper = is_looper
                              return info

                              straight_line = StraightLinePropagator()

                              helix = HelixPropagator()

 */
