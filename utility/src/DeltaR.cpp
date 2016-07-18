#include "DeltaR.h"
#include "TLorentzVector.h"
#include <cmath>

namespace papas {

double deltaPhi(double p1, double p2) {
  /// Computes delta phi, handling periodic limit conditions.'''
  double res = p1 - p2;
  while (res > M_PI)
    res -= 2 * M_PI;
  while (res < -M_PI)
    res += 2 * M_PI;
  return res;
}
double deltaR(double e1, double p1, double e2, double p2) {
  /// Take either 4 arguments (eta,phi, eta,phi) ""
  double de = e1 - e2;
  double dp = deltaPhi(p1, p2);
  return sqrt(de * de + dp * dp);
}

}  // end namespace papas