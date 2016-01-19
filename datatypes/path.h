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


/// Path followed by a particle in 3D space.
class Path {
   ///
   ///Assumes constant speed magnitude both along the z axis and in the transverse plane.
   ///Path base class is for straightline.
   ///
public:
   Path(const TLorentzVector& p4, const TVector3& origin);
   void addPoint(std::string label, TVector3& vec) { m_points[label] = vec;}

   double getTimeAtZ(double z) const ;
   double getDeltaT(double path_length) const;
   double getVZ() const ;
   double getVPerp() const ;
   double getSpeed() const {return m_speed;};
   const TVector3& getUdir() const {return m_udir ;};
   const TVector3& getOrigin() const {return m_origin ;};
   const TVector3& getNamedPoint(std::string name);
   virtual TVector3 getPointAtTime(double time) const;

protected:
   //TODO think whether these TVector3s are OK (or should be pointers).
   TVector3 m_udir;
   double m_speed;
   TVector3 m_origin;
   std::unordered_map<std::string, TVector3>
   m_points; //TODO make this an enumeration

private:
};

//TODO underdevelopment
class Helix : public Path {
public:
   Helix(double field, double charge, const TLorentzVector& p4,
         const TVector3& origin);
   std::vector<double> getPolarAtTime(double time) const;
   double getTimeAtPhi(double phi) const;
   double getPhi(double x, double y) const;
   double getPathLength(double deltat) const;
   TVector3 getPointFromPolar(const std::vector<double>& polar) const;
   TVector3 getPointAtTime(double time) const override;

private:
   //double m_charge;
   double m_rho;    ///Radius of Helix
   double m_omega;
   double m_phi0;
   double m_phiMin; ///Minimum angle of ARC
   double m_phiMax;
   TVector3 m_vOverOmega;
   TVector3 m_centerXy;
   TVector3 m_extremePointXy;
};



#endif /* path_h */
