//
//  Created by Alice Robson on 09/11/15.
//
//

#include "GTrajectory.h"

namespace papas {

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


}  // end namespace papas