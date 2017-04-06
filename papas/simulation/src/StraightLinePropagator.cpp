

//#include <iostream>
#include "papas/simulation/StraightLinePropagator.h"
#include "papas/datatypes/Helix.h"
#include "papas/datatypes/Particle.h"
#include "papas/datatypes/Path.h"
#include "papas/utility/GeoTools.h"

#include <memory>

namespace papas {

StraightLinePropagator::StraightLinePropagator(std::shared_ptr<const Field> field) : Propagator(field) {}

void StraightLinePropagator::propagateOne(Particle& ptc, const SurfaceCylinder& cyl) const {
  auto layer = cyl.layer();
  double cylinderz = cyl.z();
  double cylinderradius = cyl.radius();
  std::shared_ptr<Path> line = ptc.path();
  if (line == nullptr) {
    line = std::make_shared<Path>(Path(ptc.p4(), ptc.startVertex(), ptc.charge()));
    ptc.setPath(line);
  }
  auto udir = line->unitDirection();
  auto origin = line->origin();
  double theta = udir.Theta();
  if (fabs(origin.Z()) > cylinderz || origin.Perp() > cylinderradius) return;  //  particle created outside the cylinder
  double zbar = line->unitDirection().Z();                                     // Z of unit direction vector
  if (zbar != 0) {
    double destz = (zbar > 0) ? cylinderz : -cylinderz;
    double length = (destz - origin.Z()) / cos(theta);  // TODO check Length >0
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
      // double km = (-b - sqrt(delta))/(2*a);
      // positive propagation -> correct solution.
      double kp = (-b + sqrt(delta)) / (2 * a);
      // print delta, km, kp
      destination = origin + udir * kp;
      // TODO deal with Z == 0
      // TODO deal with overlapping cylinders
    }
    line->addPoint(layer, destination);
  }
}

}  // end namespace papas
