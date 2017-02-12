#ifndef GEOTOOLS_H
#define GEOTOOLS_H

#include <algorithm>
#include <vector>

namespace papas {
/** Calculate intersection between a circle 1 and a circle 2 centred at origin.
 x1 X coordinate of centre of circle 1
 y1 Y coordinate of centre of circle 2
 r1 Radius of circle 1
 r2 Radius of circle 2 (which has centre (0,0))
 */
std::vector<std::pair<double, double>> circleIntersection(double x1, double y1, double r1, double r2);
}  // end namespace papas
#endif
