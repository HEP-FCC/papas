//
//  Helix.cpp
//  papas
//
//  Created by Alice Robson on 11/05/16.
//
//

#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/Helix.h"
#include "papas/utility/DeltaR.h"
#include <array>

namespace papas {
extern double gconstc;

Helix::Helix() {}

Helix::Helix(const TLorentzVector& p4, const TVector3& origin, double charge, double field)
    : Path(p4, origin, field), m_vOverOmega(p4.Vect()) {
  if (charge * field == 0) throw "invalid parameters for Helix: charge or field are zero";
  m_rho = p4.Perp() / (fabs(charge) * field) * 1e9 / gconstc;
  m_vOverOmega *= 1. / (charge * field) * 1e9 / gconstc;
  m_omega = charge * field * gconstc * gconstc / (p4.M() * p4.Gamma() * 1e9);
  TVector3 momperp_xy = TVector3(-p4.Y(), p4.X(), 0.).Unit();
  TVector3 origin_xy = TVector3(origin.X(), origin.Y(), 0.);
  m_centerXY = origin_xy - charge * momperp_xy * m_rho;
  m_extremePointXY = TVector3(m_rho, 0., 0.);
  if (m_centerXY.X() != 0 or m_centerXY.Y() != 0) m_extremePointXY = m_centerXY + m_centerXY.Unit() * m_rho;
  // calculate phi range with the origin at the center,
  // for display purposes
  TVector3 center_to_origin = origin_xy - m_centerXY;
  m_phi0 = center_to_origin.Phi();
  m_phiMin = m_phi0 * 180 / M_PI;
  m_phiMax = m_phiMin + 360.;
}

std::array<double, 3> Helix::polarAtTime(double time) const {
  double z = vZ() * time + m_origin.Z();
  double phi = -m_omega * time + m_phi0;
  return std::array<double, 3>{{m_rho, z, phi}};
}

double Helix::timeAtPhi(double phi) const {
  double time = deltaPhi(m_phi0, phi) / m_omega;
  return time;
}

double Helix::phi(double x, double y) const {
  TVector3 xy = TVector3(x, y, 0.);
  xy -= m_centerXY;
  return xy.Phi();
}

TVector3 Helix::pointFromPolar(const std::vector<double>& polar) const {
  double z = polar[1];
  double phi = polar[2];
  TVector3 xy = m_centerXY + m_rho * TVector3(cos(phi), sin(phi), 0.);
  return TVector3(xy.X(), xy.Y(), z);
}

TVector3 Helix::pointAtTime(double time) const {
  double z = vZ() * time + m_origin.Z();
  double x = m_origin.X() + m_vOverOmega.Y() * (1 - cos(m_omega * time)) + m_vOverOmega.X() * sin(m_omega * time);
  double y = m_origin.Y() - m_vOverOmega.X() * (1 - cos(m_omega * time)) + m_vOverOmega.Y() * sin(m_omega * time);
  return TVector3(x, y, z);
}

TVector3 Helix::pointAtZ(double z) const {

  double time = timeAtZ(z);
  return pointAtTime(time);
}

TVector3 Helix::pointAtPhi(double phi) const {
  double time = timeAtPhi(phi);
  return pointAtTime(time);
}

double Helix::maxTime() const {
  double maxz = 0;
  double minz = 0;
  // don't go further than HCalIn (if it exists)
  if (hasNamedPoint(kHcalIn)) {
    return timeAtZ(namedPoint(kHcalIn).Z());
  }
  for (const auto& p : m_points) {
    if (p.second.Z() > 0)
      maxz = fmax(maxz, p.second.Z());
    else
      minz = fmin(minz, p.second.Z());
  }
  if (maxz > 0)
    return timeAtZ(maxz);
  else
    return timeAtZ(minz);
}

double Helix::pathLength(double deltat) const {
  return sqrt(m_omega * m_omega * m_rho * m_rho + vZ() * vZ()) * deltat;
}
}  // end namespace papas
