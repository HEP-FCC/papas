//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PAPAS_GTRAJECTORIES_H
#define PAPAS_GTRAJECTORIES_H

#include <list>
#include <utility>

#include "papas/display/Drawable.h"
#include "papas/display/GBlob.h"
#include "papas/display/GTrajectory.h"
#include "papas/datatypes/Path.h"
#include "TVector3.h"

namespace papas {

class Cluster;
class Track;
class Particle;
class SimParticle;

/// Used to display the tracks and clusters on output graphs
class GTrajectories : public Drawable {
public:
  /// Initial implementation for a line (should accept list of particles eventually (or equiv))
  GTrajectories(const std::vector<TVector3>& points);
  /// Initial implementation for a cluster (should accept list of particles eventually (or equiv))
  GTrajectories(const Cluster& cluster);
  GTrajectories(const Track& track);
  GTrajectories(const SimParticle& particle);
  GTrajectories(const Particle& particle, int linestyle, int linecolor, int linewidth);
  void Draw(const std::string& projection) override;
  void addStraight(const Path::Ptr path, const TVector3& tvec, int linestyle, int linecolor, int linewidth);
  void addHelix(const Path::Ptr path, const TVector3& tvec, int linestyle, int linecolor);
  void addNamedPoints(const Path::Points& path, const TVector3& tvec, int linestyle, int linecolor, int linewidth);
  void addPoints(const std::vector<TVector3>& points, const TVector3& tvec, int linestyle, int linecolor,
                 int linewidth);
  void addPoints(const std::vector<TVector3>& points, double scale, int linestyle, int linecolor, int linewidth);

private:
  std::list<GTrajectory> m_gTrajectories;  ///<all the tracks
  std::list<GBlob> m_gBlobs;               ///<All the clusters
};

}  // end namespace papas

#endif
