#ifndef Helix_h
#define Helix_h

#include "papas/datatypes/Path.h"

namespace papas {
// TODO more documentation needed. Ask Colin for help.
class Helix : public Path {
  /// Decribes helix path of charged particles
public:
  /// Default Constructor
  Helix();
  ~Helix() = default;
  /**Constructor
   @param p4 4-momentum
   @param origin position of start of path
   @param magnetic field
   @param charge of associated particle
   */
  Helix(const TLorentzVector& p4, const TVector3& origin, double charge, double field = 0);
  /** Returns the polar coordinates on the path at a given time
   @param time the time
   @return the polar coordinates of the particle along the path at time
   */
  std::array<double, 3> polarAtTime(double time) const;
  /** Returns the polar coordinates on the path at a given time
   @param time the time
   @return the polar coordinates of the particle along the path at time
   */
  double timeAtPhi(double phi) const;
  double phi(double x, double y) const;
  double rho() const { return m_rho; }
  double pathLength(double deltat) const;
  TVector3 pointFromPolar(const std::vector<double>& polar) const;
  /**return a TVector3 with cartesian coordinates at time t
   @param time time t
  */
  TVector3 pointAtTime(double time) const override;
  /**return a TVector3 with cartesian coordinates at position z on z axis
   @param z z coordinate on z axis
   */
  TVector3 pointAtZ(double z) const;
  /**return a TVector3 with cartesian coordinates at angle phi
   @param phi angle
   */
  TVector3 pointAtPhi(double phi) const;
  const TVector3& extremePointXY() const { return m_extremePointXY; }
  const TVector3& centerXY() const { return m_centerXY; }
  double maxTime() const;

private:
  double m_rho;  ///< Radius of Helix
  double m_omega;
  double m_phi0;
  double m_phiMin;  ///< Minimum angle of ARC
  double m_phiMax;
  TVector3 m_vOverOmega;
  TVector3 m_centerXY;
  TVector3 m_extremePointXY;
};

}  // end namespace papas

#endif /* Helix_h */
