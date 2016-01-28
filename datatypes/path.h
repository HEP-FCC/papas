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
   Path(const TLorentzVector& p4, TVector3 origin);
   Path();
   void addPoint(std::string label, TVector3 vec) { std::cout<<label ; m_points[label] = vec;}

   double getTimeAtZ(double z) const ;
   double getDeltaT(double path_length) const;
   double getVZ() const ;
   double getVPerp() const ;
   double getSpeed() const {return m_speed;};
   TVector3 getUdir() const {return m_udir ;};
   TVector3 getOrigin() const {return m_origin ;};
   TVector3 getNamedPoint(std::string name);
   virtual TVector3 getPointAtTime(double time) const;
   const Points& getPoints(){ return m_points;};
protected:
   //TODO think whether these TVector3s are OK (or should be pointers).
   TVector3 m_udir;
   double m_speed;
   TVector3 m_origin;
   //TODO make this an enumeration
   Points m_points;
private:
};

//TODO underdevelopment
class Helix : public Path {
public:
   Helix();
   Helix(double field, double charge, const TLorentzVector& p4,
         const TVector3& origin);
   std::vector<double> getPolarAtTime(double time) const;
   double getTimeAtPhi(double phi) const;
   double getPhi(double x, double y) const;
   double getRho() const {return m_rho;};
   double getPathLength(double deltat) const;
   TVector3 getPointFromPolar(const std::vector<double>& polar) const;
   TVector3 getPointAtTime(double time) const override;
   TVector3 getPointAtZ(double z)  const;
   TVector3 getPointAtPhi(double phi)  const;
   TVector3 getExtremePointXY() const  {return m_extremePointXY;};
   TVector3 getCenterXY() const {return m_centerXY;};

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
