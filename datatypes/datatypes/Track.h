#ifndef PAPAS_TRACK_H
#define PAPAS_TRACK_H

#include "TLorentzVector.h"
#include "TVector3.h"
#include "Path.h"

namespace papas {

class Track {
public:
  Track(TVector3 p3, double charge, Path::Ptr path, long id);
  Track() : m_uniqueId(0), m_path(std::make_shared<Path>()){};
  // Track(Track& T);
  // Track(const Track& T);
  // Track(Track&& c);
  // Track(const Track&& c);
  // Track& operator=(const Track& T);
  // Track& operator=(Track& T);
  // Track& operator=(Track&&);
  //~Track();

  double pt() const { return m_p3.Perp(); }
  double energy() const { return m_p3.Mag(); }
  double eta() const { return m_p3.Eta(); }
  double charge() const { return m_charge; }
  long id() const { return m_uniqueId; }
  TVector3 p3() const { return m_p3; }
  Path::Ptr path() const { return m_path; }
  void setPath(Path::Ptr path) { m_path = path; }
  void setEnergy(double energy);
  void setSize(double value);
  static double s_maxenergy;  // AJR is this in the right place

protected:
  long m_uniqueId;
  double m_pt;
  TVector3 m_p3;
  double m_charge;
  Path::Ptr m_path;  // not owned by track but useful to know where it is
};
} // end namespace papas
#endif  // PAPAS_TRACK_H
