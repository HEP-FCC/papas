//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef DISPLAY_COREDISPLAY_H
#define DISPLAY_COREDISPLAY_H

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "TCanvas.h"
#include "TH1.h"
#include "TH2F.h"
//#include "Enummanager.h"

namespace papas {

class Drawable;

/// Display Class
/** Class to manage the  different display projections

 It contains several viewpanes each of which is a different projection

 */
/// ViewPane Class
/**
 * Class to manage provide a window in which to display graphs
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

class Display {
public:
  /// Constructor using a list of projections names

  Display(std::list<ViewPane::Projection> views = {});

  /// Which things to draw (and which are locked  ie clearable=false)
  void addToRegister(std::shared_ptr<Drawable> obj, int layer, bool clearable = true);

  /// Clear all drawable elements except those that are locked
  void clear();
  // AJRTODO does this need Zoom?
  void unZoom();
  void draw() const;

private:
  /// Map containing the views //TODO make enum
  std::unordered_map<std::string, std::unique_ptr<ViewPane>> m_views;
};

}  // end namespace papas

#endif