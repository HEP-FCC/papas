
#ifndef deltar_h
#define deltar_h
namespace papas {
  
/** TODO ask Colin what this is
*/
double deltaR(double e1, double p1, double e2, double p2);
  
/**  Computes delta phi, handling periodic limit conditions.
*/
double deltaPhi(double p1, double p2);
}  // end namespace papas

#endif  // deltar_h
