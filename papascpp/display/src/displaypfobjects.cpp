//
//  Created by Alice Robson on 09/11/15.
//
//

#include "Cluster.h"
#include "Helix.h"
#include "Id.h"
#include "Path.h"
#include "SimParticle.h"
#include "displaypfobjects.h"
#include "pTrack.h"
#include <iostream>
//#include "Id.h"

namespace papas {

GBlob::GBlob(const Cluster& cluster) {
  TVector3 pos = cluster.position();
  double radius = cluster.size();
  double thetaphiradius = cluster.angularSize();
  double max_energy = Cluster::s_maxEnergy;
  double iradius = radius * cluster.energy() / max_energy;
  double ithetaphiradius = thetaphiradius * cluster.energy() / max_energy;

  int color = 1;

  m_contours["xy"] = std::unique_ptr<TEllipse>{new TEllipse(pos.X(), pos.Y(), radius)};
  m_inners["xy"] = std::unique_ptr<TEllipse>{new TEllipse(pos.X(), pos.Y(), iradius)};
  m_contours["yz"] = std::unique_ptr<TEllipse>{new TEllipse(pos.Z(), pos.Y(), radius)};
  m_inners["yz"] = std::unique_ptr<TEllipse>{new TEllipse(pos.Z(), pos.Y(), iradius)};
  m_contours["xz"] = std::unique_ptr<TEllipse>{new TEllipse(pos.Z(), pos.X(), radius)};
  m_inners["xz"] = std::unique_ptr<TEllipse>{new TEllipse(pos.Z(), pos.X(), iradius)};
  m_contours["ECAL_thetaphi"] =
      std::unique_ptr<TEllipse>{new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), thetaphiradius)};
  m_inners["ECAL_thetaphi"] = std::unique_ptr<TEllipse>{new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), ithetaphiradius)};
  m_contours["HCAL_thetaphi"] =
      std::unique_ptr<TEllipse>{new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), thetaphiradius)};
  m_inners["HCAL_thetaphi"] = std::unique_ptr<TEllipse>{new TEllipse(M_PI_2 - pos.Theta(), pos.Phi(), ithetaphiradius)};

  // Loop thorugh inners and outers applying settings
  for (auto const& contour : m_contours) {
    contour.second->SetLineColor(4);
    contour.second->SetFillStyle(0);
  }
  for (auto const& inner : m_inners) {
    inner.second->SetFillColor(color);
    inner.second->SetFillStyle(3002);
  }
}

void GBlob::Draw(const std::string& projection, const std::string& opt) const {

  // some things are not sensible so skip these
  if ((m_layer == papas::Layer::kEcal && projection == "HCAL_thetaphi") ||
      (m_layer == papas::Layer::kHcal && projection == "ECAL_thetaphi"))
    return;

  std::string useopt = opt + "psame";

  // find the approprite projection and plot it
  if (m_contours.find(projection) != m_contours.end()) {
    m_contours.at(projection)->Draw(useopt.c_str());  // "at" instead of  []; otherwise fails because of const ness
  }

  // TODOAJR else
  // raise ValueError('implement drawing for projection ' + projection )
  if (m_inners.find(projection) != m_inners.end()) {
    m_inners.at(projection)->Draw(useopt.c_str());
  }
}

// TODO remname graph to line??
GTrajectory::GTrajectory(const std::vector<double>& X, const std::vector<double>& Y, const std::vector<double>& Z,
                         const std::vector<double>& tX, const std::vector<double>& tY, int markerstyle,
                         double markersize, int linestyle, int linecolor, int linewidth) {
  int npoints = X.size();
  // pass the vectors to the various projections
  m_graphs["xy"] = std::unique_ptr<TGraph>{new TGraph(npoints, &X[0], &Y[0])};
  m_graphs["yz"] = std::unique_ptr<TGraph>{new TGraph(npoints, &Z[0], &Y[0])};
  m_graphs["xz"] = std::unique_ptr<TGraph>{new TGraph(npoints, &Z[0], &X[0])};

  m_graphs["ECAL_thetaphi"] = std::unique_ptr<TGraph>{new TGraph(npoints, &tX[0], &tY[0])};
  m_graphs["HCAL_thetaphi"] = std::unique_ptr<TGraph>{new TGraph(npoints, &tX[0], &tY[0])};

  // AJRTODO add in other projections

  // set graph styles
  for (auto const& graph : m_graphs) {
    graph.second->SetMarkerStyle(markerstyle);
    graph.second->SetMarkerSize(markersize);
    graph.second->SetLineStyle(linestyle);
    graph.second->SetLineColor(linecolor);
    graph.second->SetLineWidth(linewidth);
  }
}


void GTrajectories::addStraight(Path::Ptr path, TVector3 tvec, int linestyle, int linecolor, int linewidth) {
  addNamedPoints(path->points(), tvec, linestyle, linecolor, linewidth);
}

void GTrajectories::addPoints(const std::vector<TVector3>& points, TVector3 tvec, int linestyle, int linecolor,
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
  int i = 0;
  for (auto p : points) {

    X.push_back(p.X() * scale);
    Y.push_back(p.Y() * scale);
    Z.push_back(p.Z() * scale);

    tX.push_back(M_PI_2 - p.Theta());
    tY.push_back(p.Phi());
  }
  i += 1;
  m_gTrajectories.push_back(GTrajectory(X, Y, Z, tX, tY, 0, 0, linestyle, linecolor, linewidth));
  m_gTrajectories.push_back(GTrajectory(X, Y, Z, tX, tY, 2, 0.3, linestyle, linecolor, linewidth));
}

void GTrajectories::addNamedPoints(const Path::Points& points, TVector3 tvec, int linestyle, int linecolor,
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

void GTrajectories::addHelix(Path::Ptr path, TVector3 tvec, int linestyle, int linecolor) {

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

void GTrajectories::Draw(const std::string& projection) const {
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