//
//  Cluster.h
//  papas
//
//  Created by Alice Robson on 28/04/16.
//
//

#ifndef Cluster_h
#define Cluster_h

#include "Definitions.h"
#include "Id.h"
#include "TVector3.h"
#include <stdio.h>

namespace papas {

class Cluster {
public:
  /** Constructor
   @param[in]  double energy: Cluster energy
   @param[in]  TVector3 position: location of Cluster
   @param[in]  double size_m: size of cluster (units?)
   @param[in]  Id::ItemType type of cluster eg kEcalCluster or kHcalCluster
    */
  Cluster(double energy, TVector3 position, double size_m, Id::ItemType id);
  /** Constructor: makes a complete copy of the original cluster and sets a new unique id
   @param[in]  const Cluster& cluster
   */
  Cluster(const Cluster& cluster, Id::Type id);
  Cluster() = default;
  ~Cluster() = default;
  Cluster(Cluster&& c) = default;
  Cluster(const Cluster& cluster) = default;
  Cluster& operator=(const Cluster&) = default;  // {std::cout<< "copy" ;} ;
  Cluster& operator+=(const Cluster& rhs);
  double angularSize() const { return m_angularSize; }
  double size() const { return m_size; }
  double pt() const { return m_pt; }
  double energy() const { return m_energy; }
  double eta() const { return m_p3.Eta(); }
  double theta() const { return M_PI / 2. - m_p3.Theta(); }
  IdType id() const { return m_uniqueId; }
  TVector3 position() const { return m_p3; }
  void setEnergy(double energy);
  void setSize(double value);
  const std::vector<IdType>& subClusters() const { return m_subClusters; };
  static double s_maxEnergy;  // AJR is this in the right place
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

}  // end namespace papas

#endif /* Cluster_h */
