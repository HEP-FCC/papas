//
//  Created by Alice Robson on 29/11/15.
//
//
#ifndef PAPAS_DISPLAY_H
#define PAPAS_DISPLAY_H

#include <string>
#include <unordered_map>

#include "ViewPane.h"


namespace papas {

class Drawable;

/// Display Class
/** Class to manage the  different display projections
*/
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
