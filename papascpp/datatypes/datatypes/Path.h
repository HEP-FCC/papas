//
//  Created by Alice Robson on 02/12/15.
//
//
#ifndef path_h
#define path_h

#include <unordered_map>
#include <vector>
#include <memory>
#include "TLorentzVector.h"
#include "TVector3.h"
#include "Definitions.h"


namespace papas {

/// Path followed by a particle in 3D space.
class Path {
  ///
  /// Assumes constant speed magnitude both along the z axis and in the transverse plane.
  /// Path base class is for straightline.
  ///
public:
  typedef std::unordered_map<papas::Position, TVector3, std::hash<int>> Points;
  typedef std::shared_ptr<Path> Ptr;  /// shared pointer to allow for striaghtline or helix

  Path(TLorentzVector p4, TVector3 origin, double field);
  Path();
  virtual ~Path() = default;

  //void addPoint(int layer, TVector3 vec) { m_points[layer] = vec; }
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
} // end namespace papas

#endif /* path_h */
