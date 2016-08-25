//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PAPAS_VIEWPANE_H
#define PAPAS_VIEWPANE_H

#include "Drawable.h"
#include "TCanvas.h"
#include "TH2F.h"
#include <list>
#include <string>
#include <vector>

namespace papas {

/**
 * Class to provide a ROOT canvas window in which to display graphs
 */
class ViewPane {
public:
  enum Projection { xy, yz, xz, ECAL_thetaphi, HCAL_thetaphi };
  /// Constructor
  ViewPane(Projection p, int nx, double xmin, double xmax, int ny, double ymin, double ymax, int dx = 600,
           int dy = 600);

  /// Keeps a register of things to draw and what layer they are
  void addToRegister(std::shared_ptr<Drawable> obj, int layer, bool clearable = true);

  void clear();
  void draw();
  void zoom(double xmin, double xmax, double ymin, double ymax);
  void unZoom();
  void saveAs(const char* fname);

  /// Static counter to say how many viewpanes there are in total
  static int nviews;
  static std::vector<std::string> ProjectionStrings;

private:
  /// Use this vector of pairs structure so that we can sort on the values (=layer)
  /// and on the objects
  std::vector<std::pair<std::shared_ptr<Drawable>, int>> m_registered;

  /// Works alongside m_registered and says which are locked
  std::vector<std::pair<std::shared_ptr<Drawable>, int>> m_locked;

  /// What we will draw onto (ROOT graphics)
  TCanvas m_canvas;

  /// The graph (ROOT graphics)
  TH2F m_hist;

  /// One of "xy", "yz", "zx" and some more not yet implemented
  Projection m_projection;
};

}  // end namespace papas

#endif
