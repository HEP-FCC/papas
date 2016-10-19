//
//  Created by Alice Robson on 09/11/15.
//
//

#include "papas/display/GTrajectories.h"
#include "papas/datatypes/Helix.h"
#include "papas/datatypes/Path.h"
#include "papas/datatypes/SimParticle.h"
#include "papas/datatypes/Track.h"
//#include "papas/datatypes/Id.h"

namespace papas {

void GTrajectories::addStraight(const Path::Ptr path, const TVector3& tvec, int linestyle, int linecolor,
                                int linewidth) {
  addNamedPoints(path->points(), tvec, linestyle, linecolor, linewidth);
}

void GTrajectories::addPoints(const std::vector<TVector3>& points, const TVector3& tvec, int linestyle, int linecolor,
                              int linewidth) {

  std::vector<double> X;
  std::vector<double> Y;
  std::vector<double> Z;
  std::vector<double> tX;  // for thetaphi graphs
  std::vector<double> tY;  // for thetaphi graphs

  // Extract vectors of x, y and z values
  int i = 0;
  for (auto p : points) {

    X.push_back(p.X());
    Y.push_back(p.Y());
    Z.push_back(p.Z());
    if (i == 1) {
      tX.push_back(M_PI_2 - p.Theta());
      tY.push_back(p.Phi());
    } else {
      tX.push_back(M_PI_2 - tvec.Theta());
      tY.push_back(tvec.Phi());
    }
    i += 1;
    // first point is wrong and should be tppoint = description.p4().Vect()
  }
  m_gTrajectories.push_back(GTrajectory(X, Y, Z, tX, tY, 0, 0, linestyle, linecolor, linewidth));
}

void GTrajectories::addPoints(const std::vector<TVector3>& points, double scale, int linestyle, int linecolor,
                              int linewidth) {

  std::vector<double> X;
  std::vector<double> Y;
  std::vector<double> Z;
  std::vector<double> tX;  // for thetaphi graphs
  std::vector<double> tY;  // for thetaphi graphs

  // Extract vectors of x, y and z values
  // int i = 0;
  for (auto p : points) {

    X.push_back(p.X() * scale);
    Y.push_back(p.Y() * scale);
    Z.push_back(p.Z() * scale);

    tX.push_back(M_PI_2 - p.Theta());
    tY.push_back(p.Phi());
  }
  // i += 1;
  m_gTrajectories.push_back(GTrajectory(X, Y, Z, tX, tY, 0, 0, linestyle, linecolor, linewidth));
  m_gTrajectories.push_back(GTrajectory(X, Y, Z, tX, tY, 2, 0.3, linestyle, linecolor, linewidth));
}

void GTrajectories::addNamedPoints(const Path::Points& points, const TVector3& tvec, int linestyle, int linecolor,
                                   int linewidth) {

  std::vector<double> X;
  std::vector<double> Y;
  std::vector<double> Z;
  std::vector<double> tX;  // for thetaphi graphs
  std::vector<double> tY;  // for thetaphi graphs

  // Extract vectors of x, y and z values
  int i = 0;
  for (auto p : points) {

    X.push_back(p.second.X());
    Y.push_back(p.second.Y());
    Z.push_back(p.second.Z());
    if (i == 1) {
      tX.push_back(M_PI_2 - p.second.Theta());
      tY.push_back(p.second.Phi());
    } else {

      tX.push_back(M_PI_2 - tvec.Theta());
      tY.push_back(tvec.Phi());
    }
    i += 1;
    // first point is wrong and should be tppoint = description.p4().Vect()
  }
  m_gTrajectories.push_back(GTrajectory(X, Y, Z, tX, tY, 2, 0.7, linestyle, linecolor, linewidth));
}

void GTrajectories::addHelix(const Path::Ptr path, const TVector3& tvec, int linestyle, int linecolor) {

  int npoints = 100;
  std::vector<double> X;
  std::vector<double> Y;
  std::vector<double> Z;
  std::vector<double> tX;  // for thetaphi graphs
  std::vector<double> tY;  // for thetaphi graphs
  std::shared_ptr<Helix> sp_helix = std::dynamic_pointer_cast<Helix>(path);

  double maxTime = sp_helix->maxTime();
  std::vector<TVector3> points;

  for (int i = 0; i < npoints; i++) {
    double time = (maxTime / npoints) * (i);
    points.push_back(sp_helix->pointAtTime(time));
  }

  addPoints(points, tvec, linestyle, linecolor, 1);
}

void GTrajectory::setColor(int color) {
  for (auto const& graph : m_graphs) {
    graph.second->SetMarkerColor(color);
  }
}

void GTrajectory::Draw(const std::string& projection /*,
                       const std::string&  opt*/) const {
  std::string usedopt = "lpsame";  // AJRTODO

  m_graphs.at(projection)->Draw(usedopt.c_str());  // "at" instead of  [] otherwise fails because of const ness

  // raise ValueError('implement drawing for projection ' + projection )
};

GTrajectories::GTrajectories(const SimParticle& particle) {
  if (particle.charge() != 0) {
    if (abs(particle.pdgId()) > 100) {
      addHelix(particle.path(), particle.p4().Vect(), 1, 1);
      addNamedPoints(particle.path()->points(), particle.p4().Vect(), 1, 1, 0);
    } else {
      addHelix(particle.path(), particle.p4().Vect(), 3, 7);
      addNamedPoints(particle.path()->points(), particle.p4().Vect(), 1, 5, 3);
    }

  } else {
    if (particle.pdgId() == 22)
      addStraight(particle.path(), particle.p4().Vect(), 2, 2, 1);
    else
      addStraight(particle.path(), particle.p4().Vect(), 2, 1, 1);
    addNamedPoints(particle.path()->points(), particle.p4().Vect(), 1, 5, 0);
  }
}

GTrajectories::GTrajectories(const Particle& particle, int linetype, int linewidth, int linecolor) {
  addPoints(std::vector<TVector3>{TVector3(0, 0, 0), particle.p4().Vect().Unit()}, log(particle.e()), linetype,
            linewidth, linecolor);
}

GTrajectories::GTrajectories(const Track& track) {
  if (track.charge() != 0) {
    addHelix(track.path(), track.p3(), 1, 1);
    addStraight(track.path(), track.p3(), 0, 0, 0);
  } else
    addStraight(track.path(), track.p3(), 1, 1, 1);
}

/// Constructor for showing clusters
GTrajectories::GTrajectories(const Cluster& cluster) { m_gBlobs.push_back(GBlob(cluster)); }

void GTrajectories::Draw(const std::string& projection) {
  // draw tracks
  for (auto const& traj : m_gTrajectories) {
    traj.Draw(projection);
  }
  // draw clusters
  for (auto const& traj : m_gBlobs) {
    traj.Draw(projection);
  }
}

}  // end namespace papas
