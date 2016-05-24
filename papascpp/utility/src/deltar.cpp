#include "deltar.h"
#include <cmath>
#include "TLorentzVector.h"


namespace papas {

double deltaPhi(double  p1, double p2)
{
   ///Computes delta phi, handling periodic limit conditions.'''
   double res = p1 - p2;
   while (res > M_PI)
      res -= 2 * M_PI;
   while (res < -M_PI)
      res += 2 * M_PI;
   return res;
}
double deltaR(double e1, double p1, double e2, double p2)
{
   ///Take either 4 arguments (eta,phi, eta,phi) ""
   double de = e1 - e2;
   double dp = deltaPhi(p1, p2);
   return sqrt(de * de + dp * dp);
}

/*double deltaR( TLorentzVector e1,TLorentzVector  p1){
///takes two objects that have 'eta', 'phi' methods)"
    return deltaR(e1.eta(),e1.phi(), p1.eta(), p1.phi()) ;
}*/

} // end namespace papas