//
//  Created by Alice Robson on 29/11/15.
//  TODO RENAME THIS to DATATYPES
//
#ifndef DataTypes_H
#define DataTypes_H
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

//TODO remove this and the distance functions
class Track;


class Cluster  {
public:
  Cluster(double energy, const TVector3& position, double size_m, long id);
  Cluster() = default;
  Cluster(Cluster && c)   = default;
  Cluster(const Cluster&) = default;/*  {
                                      std::cout<< "copy" ;} ;*/
  Cluster& operator+=(const Cluster& rhs);
  double angularSize() const {return m_angularSize;};
  double size() const     {return m_size;};
  double pt() const       {return m_pt;};
  double energy() const   {return m_energy;};
  double eta() const      {return m_position.Eta();};
  long ID() const         {return m_uniqueID;}
  TVector3 position() const {return m_position;};
  //DistanceData distance(const Cluster& clust) const;
  //DistanceData distance(const Track& track) ;//const;
  //DistanceData clusterdistanceData(const TVector3& point) const;
  //DistanceData setDistanceToCluster(const Cluster& clust) const;
  // DistanceData setDistanceToPoint(const TVector3& point) const;
  void setEnergy(double energy);
  void setSize(double value) ;
  std::vector<long> subClusters() const { return m_subClusters;};
  static double s_maxEnergy; //AJR is this in the right place
  
protected:
  long m_uniqueID;
  double m_size;
  double m_angularSize;
  double m_pt;
  TVector3 m_position;
  double m_energy;
  std::vector<long> m_subClusters;
  
};


class Track{
public:
  Track(TVector3 p3, double charge, Path& path, long id);
  Track() : m_uniqueID(0), m_path(Path::NullPath) {};
  Track(const Track& T) =default;
  
  double pt() const       {return m_p3.Perp();};
  double energy() const   {return m_p3.Mag();};
  double eta() const      {return m_p3.Eta();};
  double charge() const   {return m_charge;};
  long   ID() const       {return m_uniqueID;}
  Path&  path() const      {return m_path;}
  
  //std::pair<bool, double> isInside(const TVector3& point) const;
  void setEnergy(double energy);
  void setSize(double value) ;
  static double s_maxenergy; //AJR is this in the right place
  
  //DistanceData distance(const Cluster& clust) ; //const; can't get to compile with const due to path issue
  //DistanceData distance(const Track& track) const;
  
  
protected:
  long m_uniqueID;
  double m_pt;
  TVector3 m_p3;
  double m_charge;
  Path& m_path; //not owned by track but useful to know where it is
  
};


class SimParticle: public Particle {
public:
  bool isCharged() const;
  SimParticle() = default;
  SimParticle(long uniqueid, int pdgid, TLorentzVector tlv, double  field=0., TVector3 vertex= TVector3(0., 0.,0.));
  Path& path()   {if (m_isHelix) return m_helix; else return m_path;};
  const class Path& constPath() const   {if (m_isHelix) return m_helix; else return m_path;};
  Helix& helix() {return m_helix;}
  TVector3 pathPosition(std::string name);
  
private:
  TVector3 m_vertex;
  class Path m_path;
  class Helix m_helix;
  bool m_isHelix;
};

#endif