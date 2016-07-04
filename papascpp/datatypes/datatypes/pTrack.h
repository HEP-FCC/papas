#ifndef PAPAS_TRACK_H
#define PAPAS_TRACK_H

#include "Path.h"
#include "TLorentzVector.h"
#include "TVector3.h"

namespace papas {

class Track {
public:
  Track(TVector3 p3, double charge, Path::Ptr path);
  Track() : m_uniqueId(0), m_path(std::make_shared<Path>()){};
  // Track(Track& T);
  // Track(const Track& T);
  // Track(Track&& c);
  // Track(const Track&& c);
  Track& operator=(const Track& T) = default;
  // Track& operator=(Track& T);
  // Track& operator=(Track&&);
  //~Track();

  double pt() const { return m_p3.Perp(); }
  double energy() const { return m_p3.Mag(); }
  double eta() const { return m_p3.Eta(); }
  double charge() const { return m_charge; }
  IdType id() const { return m_uniqueId; }
  TVector3 p3() const { return m_p3; }
  double theta() const { return M_PI / 2. - m_p3.Theta(); }
  Path::Ptr path() const { return m_path; }
  void setPath(Path::Ptr path) { m_path = path; }
  void setEnergy(double energy);
  void setSize(double value);
  static double s_maxenergy;  // AJR is this in the right place
  std::string info() const;

protected:
  IdType m_uniqueId;
  double m_pt;
  TVector3 m_p3;
  double m_charge;
  Path::Ptr m_path;  // not owned by track but useful to know where it is
};

std::ostream& operator<<(std::ostream& os, const Track& track);

}  // end namespace papas
#endif  // PAPAS_TRACK_H
