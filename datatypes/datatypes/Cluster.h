//
//  Cluster.hpp
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#ifndef Cluster_h
#define Cluster_h


#include "TVector3.h"
#include "particle.h"
#include "path.h"


class TLorentzVector;
typedef std::shared_ptr<Path> sptrPath; ///shared pointer to allow for striaghtline or helix


/// Function to create a new TLorentzVector
TLorentzVector makeTLorentzVector(int pdgid, double theta, double  phi,
                                  double energy);

class Cluster  {
public:
  Cluster(double energy, TVector3 position, double size_m, long id);
  Cluster() = default;
  Cluster(Cluster&& c)    = default;
  Cluster(const Cluster&) = default; //=default {std::cout<< "copy" ;} ;
  Cluster& operator+=(const Cluster& rhs);
  double angularSize() const {return m_angularSize;}
  double size() const     {return m_size;}
  double pt() const       {return m_pt;}
  double energy() const   {return m_energy;}
  double eta() const      {return m_position.Eta();}
  long id() const         {return m_uniqueId;}
  TVector3 position() const {return m_position;}
  void setEnergy(double energy);
  void setSize(double value) ;
  std::vector<long> subClusters() const { return m_subClusters;};
  static double s_maxEnergy; //AJR is this in the right place
  
  friend std::ostream& operator<<(std::ostream& os, const Cluster& cluster); //TODO move to helper class
  
protected:
  long m_uniqueId;
  double m_size;
  double m_angularSize;
  double m_pt;
  TVector3 m_position;
  double m_energy;
  std::vector<long> m_subClusters;
  
};


#include <stdio.h>

#endif /* Cluster_h */
