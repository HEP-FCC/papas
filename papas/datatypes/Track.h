#ifndef PAPAS_TRACK_H
#define PAPAS_TRACK_H

#include "TLorentzVector.h"
#include "TVector3.h"
#include "papas/datatypes/Path.h"

namespace papas {
 /** @brief Determines the trajectory in space and time of a particle (charged or neutral).
  
attributes:
  - p3 : momentum in 3D space (px, py, pz)
  - charge : particle charge
  - path : contains the trajectory parameters and points
  
*/
class Track {
public:
  /** Constructor
   @param[in] p3 3-momentum of particle
   @param[in] charge particle charge
   @param[in] path associated path
   @param[in] index used to create track unique id, normally the index of the collection to which the track will belong
   @param[in] the subtype of the track used in creating the unique id
   */
  Track(const TVector3& p3, double charge, const Path::Ptr path, unsigned int index, char subtype = 'u');
  double pt() const { return m_p3.Perp(); } ///<momentum
  double energy() const { return m_p3.Mag(); } ///<energy
  double eta() const { return m_p3.Eta(); }
  double charge() const { return m_charge; }
  IdType id() const { return m_uniqueId; } ///<unique identifier
  const TVector3& p3() const { return m_p3; } /// momentum
  double theta() const { return M_PI / 2. - m_p3.Theta(); }
  const Path::Ptr path() const { return m_path; }
  void setEnergy(double energy);
  void setSize(double value);
  static double s_maxenergy;  // AJR is this in the right place
  std::string info() const; ///< string representation of track
protected:
  IdType m_uniqueId; ///< Unique identifier of track
  double m_pt; ///< momentum
  TVector3 m_p3;  ///< momentum in 3D space (px, py, pz)
  double m_charge; ///< Charge of associated particle
  const Path::Ptr m_path;  ///< pointer to path (not owned by track)
private:
};

std::ostream& operator<<(std::ostream& os, const Track& track);

}  // end namespace papas
#endif  // PAPAS_TRACK_H
