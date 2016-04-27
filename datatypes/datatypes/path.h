//
//  Created by Alice Robson on 02/12/15.
//
//
#ifndef path_h
#define path_h

#include <unordered_map>
#include <vector>
#include "TLorentzVector.h"
#include "TVector3.h"

//temp
#include <iostream>


/// Path followed by a particle in 3D space.
class Path {
   ///
   ///Assumes constant speed magnitude both along the z axis and in the transverse plane.
   ///Path base class is for straightline.
   ///
public:
   typedef std::unordered_map<std::string, TVector3> Points;
  
   Path(const TLorentzVector& p4, TVector3 origin, double field);
   Path();
  
   void addPoint(std::string label, TVector3 vec) { m_points[label] = vec;}
   double timeAtZ(double z) const;
   double deltaT(double path_length) const;
   double vZ() const;
   double vPerp() const;
   double speed() const {return m_speed;}
   TVector3 udir() const {return m_udir;}
   TVector3 origin() const {return m_origin;}
   bool hasNamedPoint(std::string name) const;
   TVector3 namedPoint(std::string name) const;
   virtual TVector3 pointAtTime(double time) const;
   const Points& points() const { return m_points;}
   double field() const {return m_field;}
  //static Path NullPath; //this allows classes used in stl containers to include a reference to a path
protected:
   TVector3 m_udir;
   double m_speed;
   TVector3 m_origin;
   //TODO make this an enumeration
   Points m_points;
   double m_field; //set to 0 for a straightline
private:
   
};

//TODO underdevelopment
class Helix : public Path {
public:
   Helix();
   Helix(const TLorentzVector& p4, const TVector3& origin, double field, double charge);
   std::vector<double> polarAtTime(double time) const;
   double timeAtPhi(double phi) const;
   double phi(double x, double y) const;
   double rho() const {return m_rho;};
   double pathLength(double deltat) const;
   TVector3 pointFromPolar(const std::vector<double>& polar) const;
   TVector3 pointAtTime(double time) const override;
   TVector3 pointAtZ(double z)  const;
   TVector3 pointAtPhi(double phi) const;
   TVector3 extremePointXY() const {return m_extremePointXY;};
   TVector3 centerXY() const {return m_centerXY;};

private:
   //double m_charge;
   double m_rho;    ///Radius of Helix
   double m_omega;
   double m_phi0;
   double m_phiMin; ///Minimum angle of ARC
   double m_phiMax;
   TVector3 m_vOverOmega;
   TVector3 m_centerXY;
   TVector3 m_extremePointXY;
};

#endif /* path_h */
