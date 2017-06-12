#ifndef PAPAS_TRACK_H
#define PAPAS_TRACK_H

#include "papas/datatypes/Definitions.h"

#include "TVector3.h"

#include <memory>

namespace papas {

class Path;
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
   @param[in] index used to create track identifier, normally the index of the collection to which the track will belong
   @param[in] the subtype of the track used in creating the identifer
   */
  Track(const TVector3& p3, double charge, const std::shared_ptr<Path> path, uint32_t index, char subtype = 'u');
  double energy() const { return m_p3.Mag(); }  ///<energy
  double charge() const { return m_charge; }
  Identifier id() const { return m_id; }       ///<identifier
  const TVector3& p3() const { return m_p3; }  /// momentum
  const std::shared_ptr<Path> path() const { return m_path; }
  void setEnergy(double energy);
  void setSize(double value);
  std::string info() const;  ///< string representation of track
protected:
  Identifier m_id;                     ///< Identifier of track
  TVector3 m_p3;                       ///< momentum in 3D space (px, py, pz)
  double m_charge;                     ///< Charge of associated particle
  const std::shared_ptr<Path> m_path;  ///< pointer to path (not owned by track)
private:
};

std::ostream& operator<<(std::ostream& os, const Track& track);

}  // end namespace papas
#endif  // PAPAS_TRACK_H
