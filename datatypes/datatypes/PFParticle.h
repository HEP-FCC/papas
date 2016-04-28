//
//  Created by Alice Robson on 29/11/15.

#ifndef DataTypes_H
#define DataTypes_H
/**
 file datatypes.h
 */
//#include "TVector3.h"
#include "particle.h"
#include "path.h"
class Track;
class TLorentzVector;
typedef std::shared_ptr<Path> sptrPath; ///shared pointer to allow for striaghtline or helix


class PFParticle: public Particle {
public:
  bool isElectroMagnetic() const;
  PFParticle() = default;
  PFParticle(long uniqueid, int pdgid, TLorentzVector tlv, TVector3 vertex = TVector3(0., 0.,0.), double  field = 0.);
  PFParticle(long uniqueid,const Track& track);
  sptrPath path() const { return m_path;}
  //Path& path()  {if (m_isHelix) return m_helix; else return m_path;}
  //const class Path& constPath() const   {if (m_isHelix) return m_helix; else return m_path;}
  // Helix& helix() {return std::dynamic_cast<Helix>(m_path);}
  TVector3 pathPosition(std::string name) const;
  //long id() const { return m_uniqueId;}
  //void setHelix(const Path& path);
  //void setPath(const Path& path);
  void setPath(sptrPath path) {m_path = path;}
  bool isHelix() const { return m_isHelix;}
private:
  //long m_uniqueId;
  TVector3 m_vertex;
  sptrPath m_path;
  //class Path m_path; // will use either the Path (Straightline) or helix not both
  //class Helix m_helix;
  bool m_isHelix;
};

#endif