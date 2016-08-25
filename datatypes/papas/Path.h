//
//  Created by Alice Robson on 02/12/15.
//
//
#ifndef path_h
#define path_h

#include "Definitions.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include <map>
#include <memory>
#include <vector>

namespace papas {

/// Path followed by a particle in 3D space.
///
/// Assumes constant speed magnitude both along the z axis and in the transverse plane.
/// Path base class is essentially straightline but can be inherited from to make Helix etc
///
class Path {
  public:
  typedef std::map<papas::Position, TVector3> Points;

  typedef std::shared_ptr<Path> Ptr;  /// shared pointer to allow for striaghtline or helix

  Path(TLorentzVector p4, TVector3 origin, double field = 0.);
  Path();
  virtual ~Path() = default;

  void addPoint(papas::Position layer, TVector3 vec) { m_points[layer] = vec; }
  double timeAtZ(double z) const;
  double deltaT(double path_length) const;
  double vZ() const;
  double vPerp() const;
  double speed() const { return m_speed; }
  TVector3 unitDirection() const { return m_unitDirection; }
  TVector3 origin() const { return m_origin; }
  bool hasNamedPoint(papas::Position layer) const;
  TVector3 namedPoint(papas::Position layer) const;
  virtual TVector3 pointAtTime(double time) const;
  const Points& points() const { return m_points; }
  double field() const { return m_field; }

protected:
  TVector3 m_unitDirection;
  double m_speed;
  TVector3 m_origin;
  Points m_points;
  double m_field;  // set to 0 for a straightline
private:
};

  ///Alternative name for Path class
class StraightLine : public Path {
  using Path::Path;
};
}  // end namespace papas

#endif /* path_h */
