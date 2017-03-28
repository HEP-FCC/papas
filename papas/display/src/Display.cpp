//
//  Created by Alice Robson on 09/11/15.
//
//
#include "papas/display/Display.h"
#include "papas/display/ViewPane.h"
#include <iostream>  //temp
#include <algorithm>

namespace papas {

Display::Display(std::list<ViewPane::Projection> views) {
  // TODO consider making views concrete objects
  if (views.size() == 0) {
    views = {ViewPane::Projection::xy, ViewPane::Projection::yz, ViewPane::Projection::xz};
  }
  /// Creates viewpanes
  for (auto& view : views) {
    if ((view == ViewPane::Projection::xy) | (view == ViewPane::Projection::yz) | (view == ViewPane::Projection::xz)) {
      m_views[ViewPane::ProjectionStrings[view]] =
          std::unique_ptr<ViewPane>{new ViewPane(view, 100, -4, 4, 100, -4, 4)};
    } else if (view == ViewPane::Projection::ECAL_thetaphi ||
               view == ViewPane::Projection::HCAL_thetaphi) {
      m_views[ViewPane::ProjectionStrings[view]] =
          std::unique_ptr<ViewPane>{new ViewPane(view, 100, -M_PI / 2., M_PI / 2., 100, -M_PI, M_PI, 500, 1000)};
    }
  }
};

void Display::addToRegister(std::shared_ptr<Drawable> obj, int layer, bool clearable) {
  for (auto& view : m_views) {
    view.second->addToRegister(obj, layer, clearable);
  }
};

void Display::clear() {
  for (auto const& view : m_views) {
    view.second->clear();
  }
}

void Display::unZoom() {
  for (auto const& view : m_views) {
    view.second->unZoom();
  }
}

void Display::draw() const {
  for (auto const& view : m_views) {
    view.second->draw();
  }
}

void Display::saveAs() const {
  int i = 0;
  for (auto const& view : m_views) {
    view.second->saveAs(std::string("file" + std::to_string(i) + ".jpg").c_str());
    i++;
  }
}

}  // end namespace papas
