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
  /** Constructor: makes new cluster with a new id based on a copy of an existing cluster. The new id must be provided.
   @param[in]  const Cluster& cluster to be copied
   @param[in]  IdType id new unique id to be provided by user
   example usage:
   auto mergedCluster = Cluster(clusters.at(id), Id::makeId(Id::itemType(id)));
   */
  Cluster(const Cluster& cluster, IdType id);
  Cluster() = default;
  ~Cluster() = default;
  Cluster(Cluster&& c) = default;
  Cluster(const Cluster& cluster) = default;
  Cluster& operator=(const Cluster&) = default;
  Cluster& operator+=(const Cluster& rhs); ///< merges a cluster into an existing cluster
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
  const std::vector<const Cluster*>& subClusters() const { return m_subClusters; };
  static double maxEnergy() { return s_maxEnergy;};
  std::string info() const;

protected:
  IdType m_uniqueId;
  double m_size;
  double m_angularSize;
  double m_pt;
  TVector3 m_p3;
  double m_energy;
  std::vector<const Cluster*> m_subClusters; ///< A cluster may be a merging of subClusters
  static double s_maxEnergy; ///< Maximum energy over all clusters
};

std::ostream& operator<<(std::ostream& os, const Cluster& cluster);

}  // end namespace papas

#endif /* Cluster_h */
