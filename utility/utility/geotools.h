#ifndef GEOTOOLS_H
#define GEOTOOLS_H

#include <vector>
#include <algorithm>

namespace papas {
std::vector<std::pair<double,double>> circleIntersection(double x1,double  y1, double r1, double r2);
} // end namespace papas
#endif