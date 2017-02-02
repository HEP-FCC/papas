#ifndef Cluster_h
#define Cluster_h

#include "TVector3.h"
#include "papas/datatypes/Definitions.h"
#include "papas/datatypes/Identifier.h"
#include <list>
#include <stdio.h>

namespace papas {

/** @brief The Cluster class can be used with raw, smeared and merged clusters.
  Example usage:
  @code
  @endcode
*/
class Cluster {
public:
  /** Constructor
   @param[in]  energy Cluster energy
   @param[in]  position  location of Cluster
   @param[in]  size_m size of cluster (units?)
   @param[in]  id Identifier type of cluster eg kEcalCluster or kHcalCluster
   @param[in]  subtype single char describing type of cluster eg s = smeared, t= true, m = merged
    */
  Cluster(double energy, const TVector3& position, double size_m, unsigned int counter, Identifier::ItemType id, char subtype = 't');

  /** Constructor: makes new cluster with a new id based on a copy of an existing cluster. The new id must be provided.
   @param[in]  cluster the cluster that is to be "copied"
   @param[in]  type eg Identifier::kHcalCluster the Identifier type
   @param[in]  subtype subtype of cluster eg 'm' for merged, 's' for smeared. Defaults to 'u' for unset.
   @param[in]  val the value that will be used when creating the Cluster identifier and which is used for sorting.
                When creating a merged cluster it should ideally be set to the total eneergy of the cluster
      */
  Cluster(const Cluster& cluster, unsigned int counter, Identifier::ItemType type, char subtype = 'u', float val = 0.0);
  Cluster() = default;
  Cluster(Cluster&& c);                       // needed for unordered_map
  Cluster(const Cluster& cluster) = default;  // needed for unordered_map
  Cluster& operator+=(const Cluster& rhs);    ///< merges a cluster into an existing cluster
  double angularSize() const;  ///< The angle that the cluster boundary makes (not valid for merged clusters)
  double size() const;  ///< The radius of the cluster
  double pt() const { return m_pt; } ///< Transverse momentum (magnitude of p3 in transverse plane)
  double energy() const { return m_energy; } ///< Energy
  double eta() const { return m_p3.Eta(); } ///< Pseudo-rapidity (-ln(tan self._tlv.Theta()/2))
  double theta() const { return M_PI / 2. - m_p3.Theta(); } ///< Angle w/r to transverse plane
  IdType id() const { return m_uniqueId; }  ///< Unique identifier
  const TVector3& position() const { return m_p3; } ///< position (x, y, z)
  void setEnergy(double energy); ///< Set cluster energy
  void setSize(double value); ///< Set cluster size
  const std::list<const Cluster*>& subClusters() const { return m_subClusters; };
  std::string info() const;  ///< returns a text descriptor of the cluster
  // static that returns max cluster energy (intended for display purposes)
  static double maxEnergy() { return s_maxEnergy; };
protected:
  IdType m_uniqueId; ///< Unique Identifier for Cluster
  double m_size; ///< Cluster size (radius?)
  double m_angularSize;  ///< Cluster angular size (only valid for non-merged clusters)
  double m_pt; ///< Transverse momentum (magnitude of p3 in transverse plane)
  TVector3 m_p3; ///< position (x, y, z)
  double m_energy; ///< Energy
  std::list<const Cluster*> m_subClusters;  ///< list of subClusters
  static double s_maxEnergy;                  ///< Maximum energy over all clusters
};

std::ostream& operator<<(std::ostream& os, const Cluster& cluster);

}  // end namespace papas

#endif /* Cluster_h */
