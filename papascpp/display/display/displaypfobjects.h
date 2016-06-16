//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef DISPLAY_DISPLAYPFOBJECTS_H
#define DISPLAY_DISPLAYPFOBJECTS_H

#include "Path.h"
#include "TArc.h"
#include "TColor.h"
#include "TEllipse.h"
#include "TGraph.h"
#include "TPolyLine.h"
#include "TVector3.h"
#include "displaygeometry.h"
#include <string>
#include <unordered_map>
#include <utility>

namespace papas {

// class Cluster;
class PFParticle;

/// Used to display Clusters on output graphs
class GBlob {
public:
  GBlob(const Cluster& cluster);
  void Draw(const std::string& projection, const std::string& opt = "") const;

private:
  std::string m_layer;  ///<which layer to plot at
  // outside circle of cluster
  std::unordered_map<std::string, std::unique_ptr<TEllipse>> m_contours;
  // inside circle of cluster
  std::unordered_map<std::string, std::unique_ptr<TEllipse>> m_inners;
};

/// Used to display a track on output graphs
class GTrajectory {
public:
  GTrajectory(const std::vector<double>& X, const std::vector<double>& Y, const std::vector<double>& Z,
              const std::vector<double>& tX, const std::vector<double>& tY,int markerstyle, double markersize,
              int linestyle, int linecolor, int linewidth);
  void setColor(int color);
  void Draw(const std::string& projection) const;
private:
  std::unordered_map<std::string, std::unique_ptr<TGraph>> m_graphs; ///< the line/curves for a track on each proj
};

/*class GHelixTrajectory : public GTrajectory {
public:
  GHelixTrajectory(const Path::Points& points, TVector3 tvec, int linestyle = 1, int linecolor = 1);
};*/

/// Used to display the tracks and clusters on output graphs
class GTrajectories : public Drawable {
public:
  /// Initial implementation for a line (should accept list of particles eventually (or equiv))
  GTrajectories(const std::vector<TVector3>& points);
  /// Initial implementation for a cluster (should accept list of particles eventually (or equiv))
  GTrajectories(const Cluster& cluster);
  GTrajectories(const Track& track);
  GTrajectories(const PFParticle& particle);
  void Draw(const std::string& projection) const override;
  void addStraight(Path::Ptr path, TVector3 tvec, int linestyle, int linecolor,int linewidth);
  void addHelix(Path::Ptr  path, TVector3 tvec, int linestyle, int linecolor);
  void addNamedPoints(const Path::Points& path, TVector3 tvec, int linestyle, int linecolor,int linewidth);
  void addPoints(const std::vector<TVector3>&  points, TVector3 tvec, int linestyle, int linecolor,int linewidth);
private:
  std::list<GTrajectory> m_gTrajectories;  ///<all the tracks
  std::list<GBlob> m_gBlobs;               ///<All the clusters
};

}  // end namespace papas

#endif