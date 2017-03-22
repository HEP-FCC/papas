//
//  path.cpp
//  fastsim_cmake
//
//  Created by Alice Robson on 02/12/15.
//
//

#include "papas/datatypes/Path.h"
#include <iostream>

namespace papas {

double gconstc = 299792458.0;  // TODO constants.c)

Path::Path() {} //

Path::Path(const TLorentzVector& p4, const TVector3& origin, double field)
    : m_p4(p4),
      m_unitDirection(p4.Vect().Unit()),
      m_speed(p4.Beta() * gconstc),
      m_origin(origin.X(), origin.Y(), origin.Z()),
      m_field(field)
{
  m_points[papas::Position::kVertex] = m_origin;
}

/*!
 * @discussion Find the time taken to reach a position on z axis
 * @param z
 * @return double
 */
double Path::timeAtZ(double z) const {

  double dest_time = (z - m_origin.Z()) / vZ();
  return dest_time;
}

double Path::deltaT(double path_length) const {
  /// Time needed to follow a given path length'''
  return path_length / m_speed;
}

TVector3 Path::pointAtTime(double time) const {
  /// Returns the 3D point on the path at a given time'''
  TVector3 ppoint = m_origin + m_unitDirection * m_speed * time;
  return ppoint;  // move
}

double Path::vZ() const {
  /// Speed magnitude along z axis'''
  return m_p4.Beta() * gconstc * m_unitDirection.Z();
}

double Path::vPerp() const {
  /// Speed magnitude in the transverse plane'''
  return m_speed * m_unitDirection.Perp();
}

bool Path::hasNamedPoint(papas::Position layer) const { return (m_points.find(layer) != m_points.end()); }

const TVector3& Path::namedPoint(papas::Position layer) const {
  if (hasNamedPoint(layer)) {
    return m_points.at(layer);
  } else
    throw "Layer not found for path namedPoint";
}

}  // end namespace papas
