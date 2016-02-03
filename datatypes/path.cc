//
//  path.cpp
//  fastsim_cmake
//
//  Created by Alice Robson on 02/12/15.
//
//

#include "path.h"
#include "deltar.h"
#include <iostream>

double gconstc = 299792458.0; //TODO constants.c)

//Path::Path(const TLorentzVector& p4, std::shared_ptr<const TVector3> origin):

Path::Path()
{
}

Path::Path(const TLorentzVector& p4, TVector3 origin) :
   m_udir(p4.Vect().Unit()),
   m_speed(p4.Beta() * gconstc),
   m_origin(origin.X(), origin.Y(), origin.Z())
{
   m_points["vertex"] = m_origin;
}

/*!
 * @discussion Find the time taken to reach a position on z axis
 * @param z
 * @return double
 */
double Path::getTimeAtZ(double z) const
{
   double dest_time = (z - m_origin.Z()) / getVZ();
   return dest_time;
}


double Path::getDeltaT(double path_length) const
{
   ///Time needed to follow a given path length'''
   return path_length / m_speed;
}

TVector3 Path::getPointAtTime(double  time) const
{
   ///Returns the 3D point on the path at a given time'''
   TVector3 ppoint = m_origin + m_udir * m_speed * time;
   return ppoint; //AJRTODO check this works OK
}

double Path::getVZ() const
{
   ///Speed magnitude along z axis'''
   return m_speed * m_udir.Z();
}

double Path::getVPerp() const
{
   ///Speed magnitude in the transverse plane'''
   return m_speed * m_udir.Perp();
}

bool Path::hasNamedPoint(std::string name) const//TODO change to enum
{
   return (m_points.find(name) != m_points.end());
}

TVector3 Path::getNamedPoint(std::string name) //const //TODO change to enum
{
   if (hasNamedPoint(name)) {
      return m_points[name]; //TODO consider why does not work if function is const
   }
   else return TVector3(0,0,0); //todo consider if this is sufficient for issing value case
}

Helix::Helix()
{
}


Helix::Helix(double field, double charge, const TLorentzVector&  p4,
             const TVector3& origin) :
   Path(p4, origin),
   m_rho(p4.Perp() / (fabs(charge) * field) * 1e9 / gconstc),
   m_vOverOmega(p4.Vect())
{

   m_vOverOmega *= 1. / (charge * field) * 1e9 / gconstc;
   m_omega = charge * field * gconstc * gconstc / (p4.M() * p4.Gamma() * 1e9);
   TVector3 momperp_xy = TVector3(-p4.Y(), p4.X(), 0.).Unit();
   TVector3 origin_xy = TVector3(origin.X(), origin.Y(), 0.);
   m_centerXY = origin_xy - charge * momperp_xy * m_rho;
   m_extremePointXY = TVector3(m_rho, 0., 0.);
   
   if (m_centerXY.X() != 0 or m_centerXY.Y() != 0)
      m_extremePointXY = m_centerXY + m_centerXY.Unit() * m_rho;

   // calculate phi range with the origin at the center,
   // for display purposes
   TVector3 center_to_origin = origin_xy - m_centerXY;
   m_phi0 = center_to_origin.Phi();
   m_phiMin = m_phi0 * 180 / M_PI;
   m_phiMax = m_phiMin + 360.;
}

std::vector<double> Helix::getPolarAtTime(double time)  const
{
   double z = getVZ() * time + m_origin.Z();

   double phi = - m_omega * time + m_phi0;
   return std::vector<double> {m_rho, z, phi};
}

double Helix::getTimeAtPhi(double phi)  const
{
   double time = deltaPhi(m_phi0, phi) / m_omega;
   return time;
}

double Helix::getPhi(double x, double y)  const
{
   TVector3 xy = TVector3(x, y, 0.);
   xy -= m_centerXY;
   return xy.Phi();
}

TVector3 Helix::getPointFromPolar(const std::vector<double>& polar) const
{

   double z = polar[1];
   double phi = polar[2];

   TVector3 xy = m_centerXY + m_rho * TVector3(cos(phi), sin(phi), 0.);
   return TVector3(xy.X(), xy.Y(), z);
}


TVector3 Helix::getPointAtTime(double time)  const
{

   double z = getVZ() * time + m_origin.Z();
   double x = m_origin.X() + m_vOverOmega.Y() * (1 - cos(m_omega * time))
              + m_vOverOmega.X() * sin(m_omega * time);
   double y = m_origin.Y() - m_vOverOmega.X() * (1 - cos(m_omega * time))
              + m_vOverOmega.Y() * sin(m_omega * time);
   return TVector3(x, y, z);
}

TVector3 Helix::getPointAtZ(double z)  const
{
   double time = getTimeAtZ(z);
   return getPointAtTime(time);
}

TVector3 Helix::getPointAtPhi(double phi)  const
{
   double time = getTimeAtPhi(phi);
   return getPointAtTime(time);
}

double Helix::getPathLength(double deltat)  const
{
   ///ds2 = dx2+dy2+dz2 = [w2rho2 + vz2] dt2'''

   //std::cout << m_omega << " rho " << m_rho << " vz " << getVZ() << " deltat " <<
   //          deltat;
   return sqrt(m_omega * m_omega * m_rho * m_rho + getVZ() * getVZ()) * deltat;
}




