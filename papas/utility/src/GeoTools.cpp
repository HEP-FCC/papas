#include "papas/utility/GeoTools.h"
#include <math.h>
#include <string>

namespace papas {

std::vector<std::pair<double, double>> circleIntersection(double x1, double y1, double r1, double r2) {
  /// Intersections between a circle 1 and a circle 2 centred at origin.'''
  bool switchxy = (x1 == 0.);
  if (switchxy) {
    double temp = x1;
    x1 = y1;
    y1 = temp;
  }

  double A = (pow(r2, 2) - pow(r1, 2) + pow(x1, 2) + pow(y1, 2)) / (2 * x1);
  double B = y1 / x1;
  double a = 1 + pow(B, 2);
  double b = -2 * A * B;
  double c = pow(A, 2) - pow(r2, 2);
  double delta = pow(b, 2) - 4 * a * c;
  if (delta<0.)
    throw std::string("no solution");
  double yp = (-b + sqrt(delta)) / (2 * a);
  double ym = (-b - sqrt(delta)) / (2 * a);
  double xp = sqrt(pow(r2, 2) - pow(yp, 2));
  if (fabs(pow((xp - x1), 2) + pow((yp - y1), 2) - pow(r1, 2)) > 1e-9) xp = -xp;
  double xm = sqrt(pow(r2, 2) - pow(ym, 2));
  if (fabs(pow((xm - x1), 2) + pow((ym - y1), 2) - pow(r1, 2)) > 1e-9) xm = -xm;

  std::pair<double, double> mpair{xm, ym};
  std::pair<double, double> ppair{xp, yp};

  if (switchxy) {
    mpair = {ym, xm};
    ppair = {yp, xp};
  }

  return std::vector<std::pair<double, double>>{mpair, ppair};
}

}  // end namespace papas
