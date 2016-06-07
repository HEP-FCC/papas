//
//  Cluster.h
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#ifndef Cluster_h
#define Cluster_h

#include <stdio.h>
#include "TVector3.h"
#include "Id.h"
#include "Definitions.h"

namespace papas {

class Cluster{
public:

  Cluster(double energy, TVector3 position, double size_m, IdType id);
  Cluster(const Cluster& cluster, IdType id);
  Cluster() = default;
  Cluster(Cluster&& c)    = default;
  Cluster(const Cluster&) = default;
  Cluster& operator=(const Cluster&)= default;;//=default {std::cout<< "copy" ;} ;
  Cluster& operator+=(const Cluster& rhs);
  double angularSize() const {return m_angularSize;}
  double size() const     {return m_size;}
  double pt() const       {return m_pt;}
  double energy() const   {return m_energy;}
  double eta() const      {return m_p3.Eta();}
  double theta() const    {return M_PI/2. - m_p3.Theta();}
  IdType id() const         {return m_uniqueId;}
  TVector3 position() const {return m_p3;}
  void setEnergy(double energy);
  void setSize(double value) ;
  const std::vector<IdType>& subClusters() const { return m_subClusters;};
  static double s_maxEnergy; //AJR is this in the right place
  std::string info() const;
  
protected:
  IdType m_uniqueId;
  double m_size;
  double m_angularSize;
  double m_pt;
  TVector3 m_p3;
  double m_energy;
  std::vector<IdType> m_subClusters;
};
  
  std::ostream& operator<<(std::ostream& os, const Cluster& cluster); 


} // end namespace papas


#endif /* Cluster_h */
