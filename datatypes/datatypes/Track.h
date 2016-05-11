#ifndef TRACK_H
#define TRACK_H

#include "particle.h"
class Path;
//#include "path.h"


class Track{
public:
  typedef std::shared_ptr<Path> sptrPath; ///shared pointer to allow for striaghtline or helix

  Track(TVector3 p3, double charge, sptrPath path, long id);
  Track() : m_uniqueId(0), m_path(std::make_shared<Path>()) {};
  //Track(Track& T);
  //Track(const Track& T);
  //Track(Track&& c);
  //Track(const Track&& c);
  //Track& operator=(const Track& T);
  //Track& operator=(Track& T);
  //Track& operator=(Track&&);
  //~Track();
  
  double pt() const       {return m_p3.Perp();}
  double energy() const   {return m_p3.Mag();}
  double eta() const      {return m_p3.Eta();}
  double charge() const   {return m_charge;}
  long   id() const       {return m_uniqueId;}
  TVector3 p3() const     {return m_p3;}
  sptrPath path() const { return m_path;}
  void setPath(sptrPath path) { m_path = path;}
  void setEnergy(double energy);
  void setSize(double value) ;
  static double s_maxenergy; //AJR is this in the right place
  
protected:
  long m_uniqueId;
  double m_pt;
  TVector3 m_p3;
  double m_charge;
  sptrPath m_path;  //not owned by track but useful to know where it is
};



#endif //TRACK_H
