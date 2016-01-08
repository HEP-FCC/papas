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
   ///
public:
   //Path(const TLorentzVector& p4,std::shared_ptr<const TVector3> origin);
   Path(const TLorentzVector& p4,const TVector3 & origin);
   double getTimeAtZ(double z) const ;
   double getDeltaT(double path_length) const;
   virtual TVector3 getPointAtTime(double time) const;
   double getVZ() const ;
   double getVPerp() const ;
protected:
   TVector3 m_udir;
   double m_speed;
   //std::shared_ptr<const TVector3> m_origin;
   TVector3 m_origin;
   std::unordered_map<std::string,std::shared_ptr<const TVector3>> m_points;
   
private:
};


class Helix : public Path{
public:
   //Helix(double field,double charge,const TLorentzVector&  p4,std::shared_ptr<const TVector3> origin);
   Helix(double field,double charge,const TLorentzVector& p4,const TVector3& origin);
   std::vector<double> getPolarAtTime(double time) const;
   double getTimeAtPhi(double phi) const;
   double getPhi(double x, double y) const;
   TVector3 getPointFromPolar(const std::vector<double> & polar) const;
   TVector3 getPointAtTime(double time) const override;
   double getPathLength(double deltat) const;
   
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
