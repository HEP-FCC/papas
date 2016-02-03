//
//  Created by Alice Robson on 29/11/15.
//  TODO RENAME THIS to DATATYPES
//
#ifndef datatypes_H
#define datatypes_H
/**
 file datatypes.h
 */
#include <string>
#include <list>
#include <utility>
#include "particle.h"
#include "particledata.h"
#include "TVector3.h"
#include "path.h"
#include "../pfalgo/distance.h"
#include <iostream>


/// Function to create a new TLorentzVector
TLorentzVector makeParticleLorentzVector(int pdgid, double theta, double  phi,
      double energy);
class Track;


class Cluster  {
public:
   Cluster(double energy, const TVector3& position, double size_m,
           long id);
   Cluster()=default;
   //Cluster(Cluster && c) = default;
   //Cluster(const Cluster& c) = default;
   double getAngularSize() const {return m_angularsize;};
   double getSize() const     {return m_size;};
   double getPt() const       {return m_pt;};
   double getEnergy() const   {return m_energy;};
   double getEta() const      {return m_position.Eta();};
   long getID() const         {return m_uniqueid;}
   TVector3 getPosition() const {return m_position;};
   DistanceData getDistance(const Cluster& clust) const;
   DistanceData getDistance(const Track& track) ;//const;
   bool isInside(const TVector3& point) const;
   double getPointDistance(const TVector3& point) const;
   void setEnergy(double energy);
   void setSize(double value) ;
   static double s_maxenergy; //AJR is this in the right place

protected:
   long m_uniqueid;
   double m_size;
   double m_angularsize;
   double m_pt;
   TVector3 m_position;
   double m_energy;

};


class Track{
public:
   Track(TVector3 p3, double charge,Path& path, long id);
   Track() =default;
   Track(const Track& T) =default;

   double getPt() const       {return m_p3.Perp();};
   double getEnergy() const   {return m_p3.Mag();};
   double getEta() const      {return m_p3.Eta();};
   double getCharge() const   {return m_charge;};
   long   getID() const       {return m_uniqueid;}
   
   std::pair<bool, double> isInside(const TVector3& point) const;
   void setEnergy(double energy);
   void setSize(double value) ;
   static double s_maxenergy; //AJR is this in the right place
   
   DistanceData getDistance(const Cluster& clust) ; //const; can't get to compile with const due to path issue
   DistanceData getDistance(const Track& track) const;

   
protected:
   long m_uniqueid;
   double m_pt;
   TVector3 m_p3;
   double m_charge;
   Path* m_path; //not owned by track but useful to know where it is
};


class SimParticle: public Particle {
public:
   bool IsCharged() const;
   SimParticle() =default;
   SimParticle(long uniqueid, int pdgid, TLorentzVector tlv, double  field=0.,
               TVector3 vertex= TVector3(0., 0.,0.));
   
   Path& getPath()   {if (m_isHelix) return m_helix; else return m_path;};
   Helix& getHelix() {return m_helix;}
   TVector3 getPathPosition(std::string name);
   
private:
   TVector3 m_vertex;
   Path m_path;
   Helix m_helix;
   bool m_isHelix;
};

#endif